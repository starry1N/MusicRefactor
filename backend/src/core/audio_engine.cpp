#include "../include/audio_engine.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace musicfree {

class AudioEngine::Impl {
public:
    PlayState state = PlayState::STOPPED;
    std::string current_file;
    int position = 0;
    int volume = 50;
    AudioInfo audio_info;
    
    std::thread playback_thread;
    std::mutex mutex;
    std::condition_variable cv;
    bool should_stop = false;

    void playbackLoop() {
        while (!should_stop) {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this] { return state == PlayState::PLAYING || should_stop; });

            if (should_stop) break;

            // 模拟音频播放
            while (state == PlayState::PLAYING && !should_stop) {
                lock.unlock();
                
                // 每100ms更新一次位置
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                position += 100;

                lock.lock();
                if (position >= audio_info.duration && audio_info.duration > 0) {
                    state = PlayState::STOPPED;
                    // 触发轨道结束事件
                }
            }
        }
    }
};

AudioEngine::AudioEngine() : impl_(std::make_unique<Impl>()) {
    impl_->playback_thread = std::thread([this] { impl_->playbackLoop(); });
}

AudioEngine::~AudioEngine() {
    impl_->should_stop = true;
    impl_->cv.notify_one();
    if (impl_->playback_thread.joinable()) {
        impl_->playback_thread.join();
    }
}

bool AudioEngine::load(const std::string& filePath) {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    
    impl_->current_file = filePath;
    impl_->position = 0;
    impl_->state = PlayState::STOPPED;
    
    // TODO: 使用 FFmpeg 加载文件并获取元数据
    impl_->audio_info.duration = 180000;  // 模拟：3分钟
    impl_->audio_info.title = "Sample Track";
    impl_->audio_info.artist = "Unknown Artist";
    
    return true;
}

bool AudioEngine::play() {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    
    if (impl_->current_file.empty()) {
        return false;
    }
    
    impl_->state = PlayState::PLAYING;
    impl_->cv.notify_one();
    
    if (state_callback_) {
        state_callback_(PlayState::PLAYING);
    }
    
    return true;
}

bool AudioEngine::pause() {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    
    if (impl_->state != PlayState::PLAYING) {
        return false;
    }
    
    impl_->state = PlayState::PAUSED;
    
    if (state_callback_) {
        state_callback_(PlayState::PAUSED);
    }
    
    return true;
}

bool AudioEngine::stop() {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    
    impl_->state = PlayState::STOPPED;
    impl_->position = 0;
    
    if (state_callback_) {
        state_callback_(PlayState::STOPPED);
    }
    
    return true;
}

bool AudioEngine::seek(int position) {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    
    if (position < 0 || position > impl_->audio_info.duration) {
        return false;
    }
    
    impl_->position = position;
    
    if (position_callback_) {
        position_callback_(position);
    }
    
    return true;
}

bool AudioEngine::setVolume(int volume) {
    if (volume < 0 || volume > 100) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(impl_->mutex);
    impl_->volume = volume;
    return true;
}

int AudioEngine::getVolume() const {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    return impl_->volume;
}

int AudioEngine::getPosition() const {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    return impl_->position;
}

PlayState AudioEngine::getState() const {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    return impl_->state;
}

AudioInfo AudioEngine::getAudioInfo() const {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    return impl_->audio_info;
}

void AudioEngine::onPlayStateChanged(PlayStateChangedCallback callback) {
    state_callback_ = callback;
}

void AudioEngine::onPositionChanged(PositionChangedCallback callback) {
    position_callback_ = callback;
}

void AudioEngine::onTrackEnded(TrackEndedCallback callback) {
    track_ended_callback_ = callback;
}

}  // namespace musicfree
