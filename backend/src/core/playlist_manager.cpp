#include "../include/playlist_manager.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace musicfree {

PlaylistManager::PlaylistManager() {
    // 初始化播放列表
    current_playlist_.id = "default";
    current_playlist_.name = "Default Playlist";
    current_playlist_.createdAt = std::time(nullptr);
}

PlaylistManager::~PlaylistManager() = default;

void PlaylistManager::addTrack(const Track& track) {
    current_playlist_.tracks.push_back(track);
    current_playlist_.updatedAt = std::time(nullptr);
    
    if (playlist_changed_callback_) {
        playlist_changed_callback_(current_playlist_);
    }
}

void PlaylistManager::removeTrack(int index) {
    if (index < 0 || index >= static_cast<int>(current_playlist_.tracks.size())) {
        return;
    }
    
    current_playlist_.tracks.erase(current_playlist_.tracks.begin() + index);
    current_playlist_.updatedAt = std::time(nullptr);
    
    if (current_track_index_ >= static_cast<int>(current_playlist_.tracks.size())) {
        current_track_index_ = current_playlist_.tracks.size() - 1;
    }
    
    if (playlist_changed_callback_) {
        playlist_changed_callback_(current_playlist_);
    }
}

void PlaylistManager::clear() {
    current_playlist_.tracks.clear();
    current_playlist_.updatedAt = std::time(nullptr);
    current_track_index_ = -1;
    
    if (playlist_changed_callback_) {
        playlist_changed_callback_(current_playlist_);
    }
}

Playlist PlaylistManager::getCurrentPlaylist() const {
    return current_playlist_;
}

int PlaylistManager::getCurrentTrackIndex() const {
    return current_track_index_;
}

void PlaylistManager::setCurrentTrackIndex(int index) {
    if (index >= 0 && index < static_cast<int>(current_playlist_.tracks.size())) {
        current_track_index_ = index;
    }
}

bool PlaylistManager::playNext() {
    if (current_playlist_.tracks.empty()) {
        return false;
    }
    
    if (play_mode_ == PlayMode::SHUFFLE) {
        current_track_index_ = rand() % current_playlist_.tracks.size();
    } else {
        current_track_index_++;
        
        if (current_track_index_ >= static_cast<int>(current_playlist_.tracks.size())) {
            if (play_mode_ == PlayMode::REPEAT_ALL) {
                current_track_index_ = 0;
            } else {
                current_track_index_ = current_playlist_.tracks.size() - 1;
                return false;
            }
        }
    }
    
    return true;
}

bool PlaylistManager::playPrevious() {
    if (current_playlist_.tracks.empty()) {
        return false;
    }
    
    current_track_index_--;
    
    if (current_track_index_ < 0) {
        if (play_mode_ == PlayMode::REPEAT_ALL) {
            current_track_index_ = current_playlist_.tracks.size() - 1;
        } else {
            current_track_index_ = 0;
            return false;
        }
    }
    
    return true;
}

PlayMode PlaylistManager::getPlayMode() const {
    return play_mode_;
}

void PlaylistManager::setPlayMode(PlayMode mode) {
    play_mode_ = mode;
}

int PlaylistManager::getTrackCount() const {
    return current_playlist_.tracks.size();
}

Track PlaylistManager::getTrackAt(int index) const {
    if (index >= 0 && index < static_cast<int>(current_playlist_.tracks.size())) {
        return current_playlist_.tracks[index];
    }
    return Track();
}

void PlaylistManager::onPlaylistChanged(PlaylistChangedCallback callback) {
    playlist_changed_callback_ = callback;
}

}  // namespace musicfree
