@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

title MusicFree - npm 安装修复脚本

echo.
echo ================================================
echo   MusicFree - npm 安装修复脚本
echo ================================================
echo.

set PROJECT_ROOT=%~dp0

REM 进入前端目录
cd /d "%PROJECT_ROOT%frontend"

echo 【第1步】清理旧的 npm 文件...
echo.

REM 关闭所有 npm 进程
taskkill /F /IM node.exe >nul 2>&1

REM 删除 node_modules（可能有权限问题的文件）
echo 正在删除 node_modules...
if exist node_modules (
    REM 尝试强制删除
    rmdir /s /q node_modules 2>nul
    
    REM 如果失败，尝试用另一种方法
    if exist node_modules (
        echo ⚠ 权限问题，使用管理员删除...
        REM 尝试删除顽固目录
        for /d %%X in (node_modules) do (
            takeown /r /d Y /f "%%X" >nul 2>&1
            icacls "%%X" /grant:r "%USERNAME%:F" /t >nul 2>&1
            rmdir /s /q "%%X" 2>nul
        )
    )
)

REM 删除锁定文件
echo 正在删除 package-lock.json...
if exist package-lock.json del /f /q package-lock.json

REM 清除 npm 缓存
echo 正在清除 npm 缓存...
call npm cache clean --force

echo ✓ 清理完成
echo.

echo 【第2步】配置 npm 加速...
echo.

REM 增加超时时间
call npm config set fetch-timeout 120000
call npm config set fetch-retry-mintimeout 20000
call npm config set fetch-retry-maxtimeout 120000

REM 设置 npm 日志级别
call npm config set loglevel warn

echo ✓ 配置完成
echo.

echo 【第3步】重新安装依赖...
echo.
echo 这可能需要 5-10 分钟，请耐心等待...
echo.

REM 重试 3 次
set RETRY=0

:retry_install
set /a RETRY=%RETRY%+1
echo 尝试 %RETRY%/3...

call npm install

if errorlevel 1 (
    if %RETRY% lss 3 (
        echo.
        echo ⚠ 安装失败，%RETRY% 秒后重试...
        timeout /t 5
        goto retry_install
    ) else (
        echo.
        echo ❌ 安装失败（已重试 3 次）
        echo.
        echo 【尝试其他方法】
        echo 1. 检查网络连接
        echo 2. 禁用防火墙或代理
        echo 3. 更新 npm: npm install -g npm@latest
        echo 4. 清除所有缓存: npm cache clean --force
        echo 5. 重新运行此脚本
        echo.
        pause
        exit /b 1
    )
) else (
    echo.
    echo ✓ npm 安装成功！
    goto install_success
)

:install_success
echo.
echo 【第4步】验证安装...
echo.

call npm list electron

if errorlevel 1 (
    echo ❌ 验证失败
    pause
    exit /b 1
)

echo.
echo ================================================
echo ✅ 安装完成！
echo ================================================
echo.
echo 现在可以启动应用：
echo   npm run dev
echo.

pause
