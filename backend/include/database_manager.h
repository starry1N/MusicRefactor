#ifndef MUSICFREE_DATABASE_MANAGER_H
#define MUSICFREE_DATABASE_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "playlist_manager.h"

namespace musicfree {

/**
 * 数据库管理器
 * 处理用户数据持久化：播放列表、收藏、播放历史等
 */
class DatabaseManager {
public:
    static DatabaseManager& getInstance();

    // 禁止拷贝
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    /**
     * 初始化数据库连接
     * @param dbPath 数据库文件路径
     * @return 成功返回 true
     */
    bool initialize(const std::string& dbPath);

    /**
     * 关闭数据库
     */
    void shutdown();

    // ===== 播放列表操作 =====

    /**
     * 创建播放列表
     * @param playlistName 播放列表名称
     * @return 播放列表ID
     */
    std::string createPlaylist(const std::string& playlistName);

    /**
     * 删除播放列表
     * @param playlistId 播放列表ID
     * @return 成功返回 true
     */
    bool deletePlaylist(const std::string& playlistId);

    /**
     * 获取所有播放列表
     * @return 播放列表向量
     */
    std::vector<Playlist> getAllPlaylists() const;

    /**
     * 获取指定播放列表
     * @param playlistId 播放列表ID
     * @return 播放列表
     */
    Playlist getPlaylist(const std::string& playlistId) const;

    /**
     * 向播放列表添加轨道
     * @param playlistId 播放列表ID
     * @param track 轨道信息
     * @return 成功返回 true
     */
    bool addTrackToPlaylist(const std::string& playlistId, const Track& track);

    /**
     * 从播放列表删除轨道
     * @param playlistId 播放列表ID
     * @param trackId 轨道ID
     * @return 成功返回 true
     */
    bool removeTrackFromPlaylist(const std::string& playlistId, const std::string& trackId);

    // ===== 收藏操作 =====

    /**
     * 添加到收藏
     * @param track 轨道信息
     * @return 成功返回 true
     */
    bool addToFavorite(const Track& track);

    /**
     * 从收藏删除
     * @param trackId 轨道ID
     * @return 成功返回 true
     */
    bool removeFromFavorite(const std::string& trackId);

    /**
     * 获取所有收藏
     * @return 收藏的轨道列表
     */
    std::vector<Track> getFavorites() const;

    /**
     * 检查是否已收藏
     * @param trackId 轨道ID
     * @return 已收藏返回 true
     */
    bool isFavorited(const std::string& trackId) const;

    // ===== 播放历史操作 =====

    /**
     * 添加到播放历史
     * @param track 轨道信息
     */
    void addToHistory(const Track& track);

    /**
     * 获取播放历史
     * @param limit 返回最近N条记录
     * @return 历史记录列表
     */
    std::vector<Track> getHistory(int limit = 100) const;

    /**
     * 清空播放历史
     * @return 成功返回 true
     */
    bool clearHistory();

    // ===== 用户设置操作 =====

    /**
     * 保存用户设置
     * @param key 设置键
     * @param value 设置值
     * @return 成功返回 true
     */
    bool setSetting(const std::string& key, const std::string& value);

    /**
     * 获取用户设置
     * @param key 设置键
     * @param defaultValue 默认值
     * @return 设置值
     */
    std::string getSetting(const std::string& key, const std::string& defaultValue = "") const;

private:
    DatabaseManager() = default;

    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace musicfree

#endif  // MUSICFREE_DATABASE_MANAGER_H
