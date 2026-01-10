#ifndef MUSICFREE_PLAYLIST_MANAGER_H
#define MUSICFREE_PLAYLIST_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace musicfree {

/**
 * 音乐轨道信息
 */
struct Track {
    std::string id;
    std::string title;
    std::string artist;
    std::string album;
    std::string url;
    int duration = 0;  // 毫秒
    std::string source;  // 来源（网易云、QQ音乐等）
    std::string coverUrl;
};

/**
 * 播放列表
 */
struct Playlist {
    std::string id;
    std::string name;
    std::vector<Track> tracks;
    int64_t createdAt = 0;
    int64_t updatedAt = 0;
};

// 播放模式
enum class PlayMode {
    ORDER = 0,      // 顺序播放
    REPEAT_ALL = 1, // 循环播放全部
    REPEAT_ONE = 2, // 单曲循环
    SHUFFLE = 3     // 随机播放
};

using PlaylistChangedCallback = std::function<void(const Playlist&)>;

/**
 * 播放列表管理器
 */
class PlaylistManager {
public:
    PlaylistManager();
    ~PlaylistManager();

    // 禁止拷贝
    PlaylistManager(const PlaylistManager&) = delete;
    PlaylistManager& operator=(const PlaylistManager&) = delete;

    /**
     * 添加轨道到当前播放列表
     * @param track 轨道信息
     */
    void addTrack(const Track& track);

    /**
     * 删除指定索引的轨道
     * @param index 轨道索引
     */
    void removeTrack(int index);

    /**
     * 删除所有轨道
     */
    void clear();

    /**
     * 获取当前播放列表
     * @return 播放列表
     */
    Playlist getCurrentPlaylist() const;

    /**
     * 获取当前轨道索引
     * @return 索引
     */
    int getCurrentTrackIndex() const;

    /**
     * 设置当前轨道
     * @param index 轨道索引
     */
    void setCurrentTrackIndex(int index);

    /**
     * 播放下一首
     * @return 是否成功
     */
    bool playNext();

    /**
     * 播放上一首
     * @return 是否成功
     */
    bool playPrevious();

    /**
     * 获取播放模式
     * @return 当前播放模式
     */
    PlayMode getPlayMode() const;

    /**
     * 设置播放模式
     * @param mode 播放模式
     */
    void setPlayMode(PlayMode mode);

    /**
     * 获取总轨道数
     * @return 轨道数
     */
    int getTrackCount() const;

    /**
     * 获取指定索引的轨道
     * @param index 轨道索引
     * @return 轨道信息
     */
    Track getTrackAt(int index) const;

    /**
     * 注册播放列表变化回调
     * @param callback 回调函数
     */
    void onPlaylistChanged(PlaylistChangedCallback callback);

private:
    Playlist current_playlist_;
    int current_track_index_ = -1;
    PlayMode play_mode_ = PlayMode::ORDER;
    PlaylistChangedCallback playlist_changed_callback_;
};

}  // namespace musicfree

#endif  // MUSICFREE_PLAYLIST_MANAGER_H
