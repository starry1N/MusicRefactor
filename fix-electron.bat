@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

title 修复 Electron 依赖

echo.
echo ================================================
echo   MusicFree - Electron 依赖修复脚本
echo ================================================
echo.

REM 检查 Node.js 版本
echo 【第1步】检查 Node.js 版本...
for /f "tokens=*" %%i in ('node --version') do set NODE_VERSION=%%i
echo 当前版本: %NODE_VERSION%

REM 检查最低版本要求（v14.17.5+）
REM 提取主版本号（如 v25.2.1 -> 25）
for /f "tokens=1,2 delims=." %%a in ("%NODE_VERSION:~1%") do set NODE_MAJOR=%%a

REM 检查主版本号是否 >= 14
if %NODE_MAJOR% geq 14 (
    echo ✓ Node.js 版本符合要求（>=v14.17.5）
    goto version_ok
)

echo ❌ Node.js 版本过低，需要 v14.17.5+
echo.
pause
exit /b 1

:version_ok
echo ✓ Node.js 版本符合要求
echo.

REM 检查 Git
echo 【第2步】检查 Git 安装...
where git >nul 2>&1
if errorlevel 1 (
    echo ❌ Git 未安装
    echo 请从 https://git-scm.com/download/win 下载安装
    echo.
    pause
    exit /b 1
)
for /f "tokens=*" %%i in ('git --version') do set GIT_VERSION=%%i
echo ✓ %GIT_VERSION% 已安装
echo.

REM 进入前端目录
cd /d %~dp0frontend

echo 【第3步】清理旧的依赖...
if exist node_modules (
    echo 删除 node_modules...
    rmdir /s /q node_modules
)
if exist package-lock.json (
    echo 删除 package-lock.json...
    del package-lock.json
)
echo ✓ 清理完成
echo.

echo 【第4步】重新安装依赖...
echo 这可能需要几分钟，请耐心等待...
echo.

call npm install

if errorlevel 1 (
    echo ❌ npm install 失败
    echo.
    echo 尝试修复：
    echo 1. 清除 npm 缓存: npm cache clean --force
    echo 2. 更新 npm: npm install -g npm@latest
    echo 3. 重新运行此脚本
    echo.
    pause
    exit /b 1
)

echo.
echo ✓ 依赖安装成功
echo.

echo 【第5步】验证 Electron...
call npm list electron

echo.
echo ================================================
echo 修复完成！
echo ================================================
echo.
echo 现在可以启动应用：
echo   npm run dev
echo.

pause
