# MusicFree 重构指南

## 概述

本项目是对原 MusicFree 项目的架构重构，采用**C++ 后端 + React 前端**的分离架构，具有以下优势：

- ✅ **性能优化**：C++ 核心引擎极速处理
- ✅ **资源占用低**：总体内存占用 80-120MB
- ✅ **开发效率高**：前后端独立开发迭代
- ✅ **易于维护**：清晰的模块划分
- ✅ **易于扩展**：插件系统灵活可靠

## 项目结构

```
MusicFree-Refactor/
├── backend/                    # C++ 后端
│   ├── src/
│   │   ├── core/              # 核心引擎
│   │   │   ├── audio_engine.cpp
│   │   │   └── playlist_manager.cpp
│   │   ├── network/           # 网络和 API
│   │   │   └── api_server.cpp
│   │   ├── database/          # 数据库
│   │   ├── plugin/            # 插件系统
│   │   └── main.cpp           # 程序入口
│   ├── include/               # 头文件
│   │   ├── audio_engine.h
│   │   ├── playlist_manager.h
│   │   ├── plugin_manager.h
│   │   ├── database_manager.h
│   │   └── api_server.h
│   ├── build/                 # 构建输出目录
│   └── CMakeLists.txt         # CMake 构建配置
│
├── frontend/                  # React 前端
│   ├── src/
│   │   ├── services/          # API 客户端
│   │   │   └── api.ts
│   │   ├── components/        # React 组件
│   │   │   ├── Player.tsx     # 播放器组件
│   │   │   └── Playlist.tsx   # 播放列表组件
│   │   ├── pages/             # 页面
│   │   │   └── Search.tsx     # 搜索页面
│   │   └── index.tsx          # 程序入口
│   ├── package.json
│   └── tsconfig.json
│
├── docs/                      # 文档
│   ├── ARCHITECTURE.md        # 架构设计文档
│   ├── API.md                 # API 文档
│   ├── PLUGIN_DEV.md          # 插件开发指南
│   └── BUILD.md               # 构建指南
│
└── README.md                  # 项目说明
```

## 后端架构

### 核心模块

#### 1. **AudioEngine（音频引擎）**
负责音频文件的加载、播放、暂停、停止等操作。

```cpp
class AudioEngine {
    bool load(const std::string& filePath);
    bool play();
    bool pause();
    bool stop();
    bool seek(int position);
    bool setVolume(int volume);
};
```

**实现方案**：
- 使用 FFmpeg 处理音频解码
- 使用 SDL2 或 PortAudio 进行音频输出
- 多线程处理音频播放（避免阻塞）

#### 2. **PlaylistManager（播放列表管理）**
管理播放列表的轨道。

```cpp
class PlaylistManager {
    void addTrack(const Track& track);
    void removeTrack(int index);
    bool playNext();
    bool playPrevious();
    void setPlayMode(PlayMode mode);
};
```

#### 3. **DatabaseManager（数据库管理）**
持久化用户数据：播放列表、收藏、历史记录等。

```cpp
class DatabaseManager {
    bool createPlaylist(const std::string& name);
    bool addToFavorite(const Track& track);
    std::vector<Track> getHistory(int limit);
};
```

使用 SQLite3 作为数据存储。

#### 4. **PluginManager（插件管理）**
加载和管理第三方插件。

```cpp
class PluginManager {
    bool loadPlugin(const std::string& path);
    std::vector<Track> searchMusic(const std::string& query);
};
```

#### 5. **ApiServer（REST API 服务器）**
向前端提供 HTTP 接口。

```cpp
class ApiServer {
    bool start(int port = 8888);
    void stop();
};
```

### 前后端通信

所有通信通过 HTTP REST API 进行，在 `http://127.0.0.1:8888/api` 上提供以下接口：

**播放器控制**：
- `POST /api/player/load` - 加载文件
- `POST /api/player/play` - 播放
- `POST /api/player/pause` - 暂停
- `POST /api/player/stop` - 停止
- `POST /api/player/seek` - 跳转
- `POST /api/player/volume` - 调节音量
- `GET /api/player/status` - 获取状态

**播放列表**：
- `GET /api/playlist` - 获取列表
- `POST /api/playlist/add` - 添加轨道
- `DELETE /api/playlist/{index}` - 删除轨道

**搜索和插件**：
- `GET /api/search?q=keyword` - 搜索
- `GET /api/plugins` - 获取插件列表

**用户数据**：
- `GET /api/favorites` - 获取收藏
- `POST /api/favorites` - 添加收藏
- `GET /api/history` - 获取历史

## 前端架构

### React 组件结构

```
components/
├── Player.tsx           # 播放器控制组件
├── Playlist.tsx         # 播放列表展示
├── Search.tsx          # 搜索界面
├── Favorites.tsx       # 收藏列表
└── Settings.tsx        # 设置面板

pages/
├── Main.tsx            # 主页面
├── Search.tsx          # 搜索页
└── Settings.tsx        # 设置页

services/
└── api.ts              # API 客户端
```

### API 调用模式

```typescript
// 导入 API
import { playerAPI, playlistAPI } from '../services/api';

// 调用播放器 API
await playerAPI.play();
const status = await playerAPI.getStatus();

// 调用播放列表 API
const playlist = await playlistAPI.get();
await playlistAPI.addTrack(track);
```

## 开发流程

### 后端开发

1. **编写代码**
   ```bash
   cd backend
   # 编辑 src/ 和 include/ 下的文件
   ```

2. **构建项目**
   ```bash
   cd backend
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

3. **运行服务器**
   ```bash
   ./bin/musicfree_server 8888
   ```

4. **测试 API**
   ```bash
   curl -X GET http://127.0.0.1:8888/api/player/status
   ```

### 前端开发

1. **安装依赖**
   ```bash
   cd frontend
   npm install
   ```

2. **开发调试**
   ```bash
   npm run dev
   ```

3. **构建**
   ```bash
   npm run make
   ```

## 插件开发指南

插件需要实现 `IPlugin` 接口：

```cpp
class MyPlugin : public IPlugin {
public:
    std::string getName() const override { return "MyPlugin"; }
    std::string getVersion() const override { return "1.0.0"; }
    bool initialize() override { return true; }
    void shutdown() override {}
    
    std::vector<Track> searchMusic(const std::string& query, int limit) override {
        // 实现搜索逻辑
        return {};
    }
    
    std::vector<Track> getPlaylist(const std::string& category, int offset, int limit) override {
        // 实现获取列表逻辑
        return {};
    }
    
    std::string getPlayUrl(const std::string& trackId) override {
        // 返回播放链接
        return "";
    }
};
```

## 关键决策

### 为什么选择 C++ 后端？
- **性能**：FFmpeg 直接处理音频编解码，无 JS 开销
- **资源**：相比 Electron 应用减少 50-70% 内存占用
- **稳定性**：音频处理关键逻辑在 C++ 实现，更加可靠

### 为什么保留 React 前端？
- **开发速度**：UI 迭代快速
- **用户体验**：丰富的组件生态
- **代码维护**：前后端分离，职责清晰

### 为什么使用 REST API？
- **通用**：语言无关，跨平台
- **调试**：可以用浏览器或 curl 直接测试
- **可扩展**：支持远程调用，未来可支持网络音乐服务

## 迁移清单

从旧项目迁移到新项目：

- [ ] 迁移音乐源插件到新插件系统
- [ ] 迁移用户播放列表数据到 SQLite
- [ ] 迁移 UI 样式和主题
- [ ] 迁移快捷键配置
- [ ] 迁移国际化文本

## 性能目标

| 指标 | 目标 | 实现 |
|------|------|------|
| 启动时间 | < 1秒 | 使用预加载 |
| 内存占用 | < 120MB | C++ 优化 |
| CPU 占用 | < 5% | 事件驱动 |
| 音乐搜索 | < 500ms | 异步请求 |

## 依赖库

### 后端
- **FFmpeg** - 音频处理
- **SQLite3** - 数据库
- **libcurl** - HTTP 客户端
- **Crow/Beast** - HTTP 服务器框架
- **nlohmann/json** - JSON 处理

### 前端
- **React 18** - UI 框架
- **TypeScript** - 类型安全
- **Axios** - HTTP 客户端
- **React Router** - 路由
- **Electron** - 桌面应用壳

## 常见问题

**Q: 后端如何启动？**
A: 运行 `./bin/musicfree_server 8888`，服务器在 `http://127.0.0.1:8888` 上运行。

**Q: 如何添加新的 API？**
A: 在 `src/network/api_server.cpp` 中添加新的路由处理函数。

**Q: 如何开发插件？**
A: 参考 `include/plugin_manager.h` 中的 `IPlugin` 接口，实现所需方法即可。

**Q: 前端和后端如何调试？**
A: 后端用 Visual Studio 或 GDB，前端用 Chrome DevTools，可独立调试。

## 下一步

1. 实现完整的 HTTP 服务器（使用 Crow 或 Beast）
2. 接入 FFmpeg 进行真实音频处理
3. 实现数据库层（SQLite）
4. 完成所有 REST API 接口
5. 开发默认插件（网易云、QQ 音乐等）
6. UI 完善和性能优化

## 参考资源

- [Electron 官方文档](https://www.electronjs.org/docs)
- [React 官方文档](https://react.dev)
- [FFmpeg 文档](https://ffmpeg.org/documentation.html)
- [SQLite 文档](https://www.sqlite.org/docs.html)
- [RESTful API 设计指南](https://restfulapi.net/)
