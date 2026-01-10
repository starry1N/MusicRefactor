@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

title npm 安装状态监控工具

echo.
echo ================================================
echo   npm 安装监控工具
echo ================================================
echo.
echo 说明：在 npm install 运行时执行此脚本
echo 用于实时监控安装进度
echo.

:monitor_loop
cls
echo ================================================
echo   npm 安装实时监控 - %date% %time%
echo ================================================
echo.

REM 检查 node 进程
echo 【1】进程状态：
tasklist | findstr "node.exe" >nul
if errorlevel 1 (
    echo ❌ node.exe 未运行（npm 可能已停止）
) else (
    echo ✓ node.exe 运行中（npm 进程活跃）
    tasklist | findstr "node"
)
echo.

REM 检查网络连接
echo 【2】网络连接：
netstat -ano | findstr "node.exe.*ESTABLISHED" >temp.txt 2>&1
set CONN_COUNT=0
for /f %%A in (temp.txt) do set /a CONN_COUNT+=1
del temp.txt 2>nul
echo 活跃连接数: %CONN_COUNT%
if %CONN_COUNT% gtr 0 (
    echo ✓ 网络正常（正在下载依赖）
) else (
    echo ⚠ 无网络连接（可能卡住或已完成）
)
echo.

REM 检查磁盘
echo 【3】磁盘空间：
if exist d:\MusicFree-Refactor\frontend\node_modules (
    for /f "tokens=*" %%A in ('dir /s /-c d:\MusicFree-Refactor\frontend\node_modules 2^>nul ^| findstr /C:"字节"') do (
        echo %%A
    )
) else (
    echo node_modules 文件夹不存在
)
echo.

REM 检查最后日志
echo 【4】最近日志：
if exist %APPDATA%\npm-cache\_logs (
    cd /d %APPDATA%\npm-cache\_logs
    for /f %%F in ('dir /o-d /b *.log 2^>nul ^| findstr /v "^$"') do (
        echo 最新日志: %%F
        type %%F 2>nul | findstr /C:"npm" | tail -1
        goto check_done
    )
) else (
    echo npm 日志目录不存在
)

:check_done
echo.

REM 诊断建议
echo 【诊断建议】
tasklist | findstr "node.exe" >nul
if errorlevel 1 (
    echo ⚠ node.exe 已停止
    echo 检查是否成功: npm list electron
) else (
    if %CONN_COUNT% gtr 0 (
        echo ✓ npm 正在正常安装中
        echo 建议：继续等待（预计 5-20 分钟）
    ) else (
        echo ⚠ npm 进程运行但无网络连接
        echo 建议：检查网络或按 Ctrl+C 中止
    )
)
echo.

echo 按任意键继续监控，或 Ctrl+C 退出...
echo （5 秒后自动刷新）
echo.
timeout /t 5 /nobreak
goto monitor_loop
