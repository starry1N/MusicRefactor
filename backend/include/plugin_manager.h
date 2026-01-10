#ifndef MUSICFREE_PLUGIN_MANAGER_H
#define MUSICFREE_PLUGIN_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "playlist_manager.h"

namespace musicfree {

/**
 * 插件接口（基类）
 * 所有插件必须实现这个接口
 */
class IPlugin {
public:
    virtual ~IPlugin() = default;

    /**
     * 获取插件名称
     */
    virtual std::string getName() const = 0;

    /**
     * 获取插件版本
     */
    virtual std::string getVersion() const = 0;

    /**
     * 获取插件描述
     */
    virtual std::string getDescription() const = 0;

    /**
     * 初始化插件
     */
    virtual bool initialize() = 0;

    /**
     * 销毁插件
     */
    virtual void shutdown() = 0;

    /**
     * 搜索音乐
     * @param query 搜索关键词
     * @param limit 返回结果数量限制
     * @return 搜索结果轨道列表
     */
    virtual std::vector<Track> searchMusic(const std::string& query, int limit = 20) = 0;

    /**
     * 获取音乐列表（如新歌、热门等）
     * @param category 分类标识
     * @param offset 偏移量
     * @param limit 返回数量
     * @return 轨道列表
     */
    virtual std::vector<Track> getPlaylist(const std::string& category, int offset = 0, int limit = 20) = 0;

    /**
     * 获取音乐播放链接
     * @param trackId 轨道ID
     * @return 播放URL
     */
    virtual std::string getPlayUrl(const std::string& trackId) = 0;
};

/**
 * 插件管理器
 */
class PluginManager {
public:
    static PluginManager& getInstance();

    // 禁止拷贝
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    /**
     * 加载插件
     * @param pluginPath 插件文件路径（.dll 或 .so）
     * @return 成功返回 true
     */
    bool loadPlugin(const std::string& pluginPath);

    /**
     * 卸载插件
     * @param pluginName 插件名称
     * @return 成功返回 true
     */
    bool unloadPlugin(const std::string& pluginName);

    /**
     * 获取所有已加载的插件
     * @return 插件名称列表
     */
    std::vector<std::string> getLoadedPlugins() const;

    /**
     * 获取指定插件
     * @param pluginName 插件名称
     * @return 插件指针，如果不存在返回 nullptr
     */
    IPlugin* getPlugin(const std::string& pluginName) const;

    /**
     * 搜索音乐（聚合所有插件的搜索结果）
     * @param query 搜索关键词
     * @return 聚合的搜索结果
     */
    std::vector<Track> searchMusic(const std::string& query);

private:
    PluginManager() = default;

    std::map<std::string, std::shared_ptr<IPlugin>> plugins_;
};

}  // namespace musicfree

#endif  // MUSICFREE_PLUGIN_MANAGER_H
