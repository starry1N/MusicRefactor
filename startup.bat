@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

title MusicFree 项目启动

echo.
echo ================================================
echo   MusicFree 音乐播放器 - 启动程序
echo   C++ 后端 + React 前端 分离架构
echo ================================================
echo.

REM 检查环境
echo 【第1步】检查开发环境...
echo.

set HAS_ERROR=0

REM 检查 Node.js
where node >nul 2>&1
if errorlevel 1 (
    echo ❌ Node.js 未安装
    set HAS_ERROR=1
) else (
    for /f "tokens=*" %%i in ('node --version') do set NODE_VERSION=%%i
    echo ✓ Node.js !NODE_VERSION! 已安装
    
    REM 检查版本号 - 提取主版本号（如 v25.2.1 -> 25）
    for /f "tokens=1,2 delims=." %%a in ("!NODE_VERSION:~1!") do set NODE_MAJOR=%%a
    if !NODE_MAJOR! geq 14 (
        echo ✓ Node.js 版本符合要求（>=v14.17.5）
    ) else (
        echo ❌ Node.js 版本过低，需要升级到 v14.17.5+
        set HAS_ERROR=1
    )
)

REM 检查 npm
where npm >nul 2>&1
if errorlevel 1 (
    echo ❌ npm 未安装
    set HAS_ERROR=1
) else (
    for /f "tokens=*" %%i in ('npm --version') do set NPM_VERSION=%%i
    echo ✓ npm !NPM_VERSION! 已安装
)

REM 检查 Git（Electron 可能需要）
where git >nul 2>&1
if errorlevel 1 (
    echo ⚠ Git 未安装（Electron 可能需要）
) else (
    for /f "tokens=*" %%i in ('git --version') do set GIT_VERSION=%%i
    echo ✓ !GIT_VERSION! 已安装
)

REM 检查 CMake
where cmake >nul 2>&1
if errorlevel 1 (
    echo ⚠ CMake 未安装（后端编译需要）
) else (
    for /f "tokens=*" %%i in ('cmake --version ^| findstr cmake') do set CMAKE_VERSION=%%i
    echo ✓ CMake 已安装
)

echo.

if %HAS_ERROR% equ 1 (
    echo 【错误】缺少必要工具，无法启动项目
    echo.
    echo 解决方案：
    echo 1. 访问 https://nodejs.org/ 安装 Node.js v16+
    echo 2. 访问 https://git-scm.com/download/win 安装 Git
    echo 3. 重启命令行
    echo.
    pause
    exit /b 1
)

REM 进入前端目录
cd /d %~dp0frontend

echo 【第2步】前端启动...
echo.

REM 检查 node_modules
if not exist node_modules (
    echo 首次运行，安装 npm 依赖...
    call npm install
    if errorlevel 1 (
        echo ❌ npm install 失败
        pause
        exit /b 1
    )
)

echo.
echo 【第3步】启动前端开发服务器...
echo.
echo 前端启动信息：
echo   • 框架: React 18 + Electron 25
echo   • 入口: http://localhost:3000
echo   • 热重载: 启用
echo   • 调试: F12 打开开发工具
echo.
echo 【重要】后端服务需在另一个窗口启动：
echo   1. 打开新的命令行窗口
echo   2. 运行: cd %~dp0backend && mkdir build && cd build
echo   3. 运行: cmake -G "Visual Studio 17 2022" -A x64 ..
echo   4. 运行: cmake --build . --config Release
echo   5. 运行: .\bin\Release\musicfree_server.exe 8888
echo.
echo 或直接运行启动脚本中的后端选项
echo.
echo ================================================
echo 启动 Electron 应用...
echo ================================================
echo.

call npm run dev

pause
