#ifndef MUSICFREE_AUDIO_ENGINE_H
#define MUSICFREE_AUDIO_ENGINE_H

#include <string>
#include <memory>
#include <functional>
#include <vector>

namespace musicfree {

// 播放状态枚举
enum class PlayState {
    STOPPED = 0,
    PLAYING = 1,
    PAUSED = 2
};

// 音频信息结构
struct AudioInfo {
    std::string title;
    std::string artist;
    std::string album;
    int duration = 0;  // 毫秒
    std::string format;
};

// 播放器事件回调
using PlayStateChangedCallback = std::function<void(PlayState)>;
using PositionChangedCallback = std::function<void(int)>;
using TrackEndedCallback = std::function<void()>;

/**
 * 音频引擎类
 * 负责音乐文件的加载、播放、暂停、停止等核心功能
 */
class AudioEngine {
public:
    AudioEngine();
    ~AudioEngine();

    // 禁止拷贝
    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;

    /**
     * 加载音频文件
     * @param filePath 文件路径（本地或网络URL）
     * @return 成功返回 true，失败返回 false
     */
    bool load(const std::string& filePath);

    /**
     * 开始播放
     * @return 成功返回 true
     */
    bool play();

    /**
     * 暂停播放
     * @return 成功返回 true
     */
    bool pause();

    /**
     * 停止播放
     * @return 成功返回 true
     */
    bool stop();

    /**
     * 设置播放位置（进度）
     * @param position 位置（毫秒）
     * @return 成功返回 true
     */
    bool seek(int position);

    /**
     * 设置音量
     * @param volume 音量等级 0-100
     * @return 成功返回 true
     */
    bool setVolume(int volume);

    /**
     * 获取当前音量
     * @return 音量等级 0-100
     */
    int getVolume() const;

    /**
     * 获取当前播放位置
     * @return 位置（毫秒）
     */
    int getPosition() const;

    /**
     * 获取当前播放状态
     * @return 播放状态
     */
    PlayState getState() const;

    /**
     * 获取音频信息
     * @return 音频信息结构
     */
    AudioInfo getAudioInfo() const;

    // 事件回调注册
    void onPlayStateChanged(PlayStateChangedCallback callback);
    void onPositionChanged(PositionChangedCallback callback);
    void onTrackEnded(TrackEndedCallback callback);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;

    // 回调函数
    PlayStateChangedCallback state_callback_;
    PositionChangedCallback position_callback_;
    TrackEndedCallback track_ended_callback_;
};

}  // namespace musicfree

#endif  // MUSICFREE_AUDIO_ENGINE_H
