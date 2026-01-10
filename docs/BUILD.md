# 构建指南

## 系统要求

### 后端（C++ 部分）
- **Windows**: Visual Studio 2019 或更新版本
- **macOS**: Xcode 12 或更新版本  
- **Linux**: GCC 9+ 或 Clang 10+

### 前端（React 部分）
- **Node.js**: 16.0 或更新版本
- **npm** 或 **yarn**

### 依赖库
- CMake 3.16+
- FFmpeg 开发库
- SQLite3
- OpenSSL（可选，用于 HTTPS）

## 后端构建

### Windows (MSVC)

```powershell
# 进入后端目录
cd backend

# 创建构建目录
mkdir build
cd build

# 生成 Visual Studio 项目
cmake -G "Visual Studio 16 2019" -A x64 ..

# 构建
cmake --build . --config Release

# 输出文件在 bin/Release/ 目录
ls bin/Release/
```

### macOS/Linux (Unix Makefiles)

```bash
# 进入后端目录
cd backend

# 创建构建目录
mkdir build
cd build

# 生成 Makefile
cmake -DCMAKE_BUILD_TYPE=Release ..

# 编译（使用 4 个线程）
cmake --build . --parallel 4

# 输出文件在 bin/ 目录
ls bin/
```

### 高级构建选项

```bash
# 启用调试符号
cmake -DCMAKE_BUILD_TYPE=Debug ..

# 启用 FFmpeg 支持
cmake -DENABLE_FFMPEG=ON ..

# 启用单元测试
cmake -DENABLE_TESTING=ON ..

# 自定义安装目录
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..

# 生成 Ninja 构建系统（更快）
cmake -G Ninja ..
```

### 验证构建

```bash
# 运行服务器
./bin/musicfree_server 8888

# 另一个终端测试 API
curl http://127.0.0.1:8888/api/health

# 应该返回：
# {"status":"ok","version":"0.1.0"}
```

## 前端构建

### 安装依赖

```bash
cd frontend

# 使用 npm
npm install

# 或使用 yarn
yarn install
```

### 开发模式

```bash
# 启动开发服务器和 Electron
npm run dev

# 或仅启动 Electron
npm start
```

### 生产构建

```bash
# 构建应用包
npm run make

# 输出文件在 out/ 目录
# - out/MusicFree Desktop-win32-x64/ (Windows)
# - out/MusicFree Desktop-darwin-arm64/ (macOS ARM)
# - out/MusicFree Desktop-linux-x64/ (Linux)
```

### 打包分发

```bash
# 生成安装程序
# Windows: NSIS 安装程序
# macOS: DMG 磁盘映像
# Linux: AppImage / deb / rpm
npm run make

# 生成便携版本 ZIP
npm run package
```

## 集成构建脚本

### build.sh (macOS/Linux)

```bash
#!/bin/bash

set -e

echo "=========================================="
echo "MusicFree Desktop - Full Build"
echo "=========================================="

# 后端构建
echo ""
echo "Building C++ Backend..."
cd backend
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel 4
cd ../..

# 前端构建
echo ""
echo "Building React Frontend..."
cd frontend
npm install
npm run make
cd ..

echo ""
echo "=========================================="
echo "Build Complete!"
echo "=========================================="
echo ""
echo "Backend: ./backend/build/bin/musicfree_server"
echo "Frontend: ./frontend/out/"
```

### build.bat (Windows)

```batch
@echo off
setlocal enabledelayedexpansion

echo ==========================================
echo MusicFree Desktop - Full Build
echo ==========================================

:: Backend build
echo.
echo Building C++ Backend...
cd backend
if not exist build mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release
cd ..\..

:: Frontend build
echo.
echo Building React Frontend...
cd frontend
call npm install
call npm run make
cd ..

echo.
echo ==========================================
echo Build Complete!
echo ==========================================
echo.
echo Backend: .\backend\build\bin\Release\musicfree_server.exe
echo Frontend: .\frontend\out\
```

## Docker 构建

### Dockerfile

```dockerfile
FROM node:16 AS frontend-builder
WORKDIR /app/frontend
COPY frontend/package*.json ./
RUN npm ci
COPY frontend/ ./
RUN npm run build

FROM ubuntu:22.04 AS backend-builder
RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libffmpeg-dev \
    libsqlite3-dev \
    libcurl4-openssl-dev

WORKDIR /app/backend
COPY backend/ ./
RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --parallel 4

FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
    libffmpeg4 \
    libsqlite3-0 \
    libcurl4 && \
    apt-get clean

COPY --from=backend-builder /app/backend/build/bin/musicfree_server /app/
COPY --from=frontend-builder /app/frontend/build /app/frontend

WORKDIR /app
EXPOSE 8888
CMD ["./musicfree_server", "8888"]
```

构建：
```bash
docker build -t musicfree:latest .
docker run -p 8888:8888 musicfree:latest
```

## 持续集成 (GitHub Actions)

### .github/workflows/build.yml

```yaml
name: Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]

    steps:
      - uses: actions/checkout@v3

      - name: Setup Node.js
        uses: actions/setup-node@v3
        with:
          node-version: '16'

      - name: Install C++ Build Tools (Linux)
        if: runner.os == 'Linux'
        run: |
          sudo apt-get update
          sudo apt-get install -y cmake build-essential

      - name: Install C++ Build Tools (macOS)
        if: runner.os == 'macOS'
        run: brew install cmake

      - name: Build Backend
        working-directory: ./backend
        run: |
          mkdir build
          cd build
          cmake -DCMAKE_BUILD_TYPE=Release ..
          cmake --build . --parallel 4

      - name: Build Frontend
        working-directory: ./frontend
        run: |
          npm ci
          npm run make

      - name: Upload Artifacts
        uses: actions/upload-artifact@v3
        with:
          name: musicfree-${{ matrix.os }}
          path: ./frontend/out/
```

## 故障排查

### 问题：CMake 找不到依赖库

```bash
# 手动指定库路径
cmake -DFFMPEG_DIR=/path/to/ffmpeg ..
cmake -DSQLITE3_DIR=/path/to/sqlite3 ..
```

### 问题：编译错误（Windows MSVC）

```powershell
# 清除旧的构建文件
rmdir /s /q build
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release
```

### 问题：npm 模块版本冲突

```bash
cd frontend
rm -rf node_modules package-lock.json
npm install
```

### 问题：端口已被占用

```bash
# 使用不同的端口
./bin/musicfree_server 9999

# 或查找占用 8888 的进程
# Linux/macOS
lsof -i :8888

# Windows
netstat -ano | findstr :8888
```

## 发布清单

- [ ] 更新版本号 (backend/CMakeLists.txt, frontend/package.json)
- [ ] 更新 CHANGELOG
- [ ] 创建 Release 标签
- [ ] 构建所有平台的应用
- [ ] 上传到 GitHub Release
- [ ] 更新官方网站下载链接
- [ ] 发布更新通知
