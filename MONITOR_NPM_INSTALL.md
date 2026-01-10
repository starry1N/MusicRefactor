# 📊 如何判断 npm 安装状态

## 🟢 正常安装中 - 5 个特征

### 1️⃣ 命令行持续输出（最重要）
```
npm notice created a lockfile as package-lock.json
npm notice installing dependencies...
npm WARN deprecated glob@7.2.3: Glob versions prior to v9 are no longer supported
```
✅ **特征**：不断有新行输出，不停顿

### 2️⃣ 磁盘持续读写
在另一个 cmd 窗口执行：
```cmd
powershell -Command "Get-Process | Where-Object {$_.ProcessName -eq 'node'}"
```
✅ **结果**：显示 node.exe 进程运行中

### 3️⃣ 网络连接活跃
在另一个 cmd 窗口执行：
```cmd
netstat -ano | findstr "node.exe"
```
✅ **结果**：显示多条 ESTABLISHED 连接

### 4️⃣ 内存持续增长
在另一个 cmd 窗口执行：
```cmd
tasklist | findstr "node"
```
✅ **结果**：显示 node.exe 进程正在运行

### 5️⃣ 进度百分比显示
```
npm notice fetching packages: [====>        ] 35% (1234/3500 packages)
```
✅ **特征**：显示进度条或百分比

---

## 🔴 安装卡住/停止 - 5 个征兆

### ❌ 征兆 1：输出完全停止 > 5 分钟
```
上一条输出在 15:05:00
现在时间是 15:10:05
中间没有任何新输出
```
**判断**：可能卡住了

### ❌ 征兆 2：进程消失
```cmd
tasklist | findstr "node"
REM 没有输出 = 进程已停止
```
**判断**：npm 进程已退出（可能出错）

### ❌ 征兆 3：网络连接断开
```cmd
netstat -ano | findstr "ESTABLISHED"
REM 没有连接 = 网络断了
```
**判断**：无法下载，需要检查网络

### ❌ 征兆 4：CPU 持续为 0%
```cmd
tasklist /v | findstr "node"
REM 看 CPU 列，如果是 0% 且输出停止
```
**判断**：进程运行但无任何操作

### ❌ 征兆 5：错误消息出现
```
npm error ECONNRESET
npm error ETIMEDOUT
npm error ENETUNREACH
```
**判断**：网络问题，安装失败

---

## 📈 实时监控 npm 安装

### 方法 1：通过进程监控（实时）

**打开新窗口 1**：运行 npm install
```cmd
cd d:\MusicFree-Refactor\frontend
npm install
```

**打开新窗口 2**：监控进程
```cmd
REM 每 2 秒刷新一次
tasklist /v /FI "IMAGENAME eq node.exe" /S . /U %USERNAME%
```
或者：
```cmd
REM 连续监控（按 Ctrl+C 停止）
wmic process list brief /every:2 | findstr "node"
```

### 方法 2：通过网络监控

**打开新窗口**：监控网络连接
```cmd
REM 每秒显示 node 的网络连接
:loop
netstat -ano | findstr "node"
timeout /t 1
goto loop
```

### 方法 3：通过日志文件监控

**打开新窗口**：实时查看 npm 日志
```cmd
REM 监控最新日志文件（Windows）
tail -f %APPDATA%\npm-cache\_logs\*-debug-0.log

REM 或用 type 命令
type %APPDATA%\npm-cache\_logs\2026-01-09T13_05_18_415Z-debug-0.log
```

### 方法 4：通过磁盘监控

**打开新窗口**：监控 node_modules 大小
```cmd
REM 每 5 秒显示一次大小
:loop
dir /s /-c d:\MusicFree-Refactor\frontend\node_modules | findstr "字节"
timeout /t 5
goto loop
```

---

## ⏱️ 安装时间预期

| 网络速度 | 预期时间 | 迹象 |
|---------|---------|------|
| 快速（>10 MB/s） | 3-5 分钟 | 进度快速推进 |
| 正常（5-10 MB/s） | 5-10 分钟 | 进度中等 |
| 缓慢（<5 MB/s） | 10-20 分钟 | 进度缓慢但稳定 |
| 很慢（<1 MB/s） | 20+ 分钟 | 需要检查网络 |

---

## 🔍 诊断卡住的 5 步法

### 步骤 1：检查进程是否存在（立即）
```cmd
tasklist | findstr "node"
```
- **有输出**：进程运行中
- **无输出**：进程已停止 → npm install 已结束（可能失败）

### 步骤 2：检查最后输出时间（5 分钟后）
```cmd
REM 如果 10 分钟没有新输出 → 卡住了
type %APPDATA%\npm-cache\_logs\*-debug-0.log | tail -5
```

### 步骤 3：检查网络连接（即时）
```cmd
netstat -ano | findstr "node.*ESTABLISHED"
```
- **有多条连接**：正在下载
- **无连接**：网络断了或下载完成

### 步骤 4：检查磁盘写入（即时）
```cmd
REM 每秒检查 node_modules 大小变化
dir /s /-c d:\MusicFree-Refactor\frontend\node_modules
```
- **大小增长**：正在下载
- **大小不变**：可能卡住

### 步骤 5：查看错误日志（最终）
```cmd
type %APPDATA%\npm-cache\_logs\*-debug-0.log | findstr "error"
```

---

## 🟡 卡住了怎么办？

### 立即行动（60 秒）

```cmd
REM 1. 记住当前时间

REM 2. 按 Ctrl+C 停止 npm install

REM 3. 运行诊断
tasklist | findstr "node"
netstat -ano | findstr "ESTABLISHED"

REM 4. 清理并重试
taskkill /F /IM node.exe
npm cache clean --force
npm install
```

### 长期卡住（>10 分钟无进度）

```cmd
REM 1. 停止 npm
taskkill /F /IM node.exe

REM 2. 完全清理
rmdir /s /q node_modules
del /f /q package-lock.json
npm cache clean --force

REM 3. 检查网络
ping registry.npmjs.org

REM 4. 使用镜像重试
npm config set registry https://registry.npmmirror.com
npm install
```

---

## 📝 监控脚本（自动）

创建 `monitor-npm.bat` 用来监控：

```batch
@echo off
setlocal enabledelayedexpansion

:monitor_loop
cls
echo ================================================
echo   npm 安装监控 - %date% %time%
echo ================================================
echo.

echo 【进程状态】
tasklist | findstr "node.exe"
if errorlevel 1 (
    echo ❌ node.exe 未运行
) else (
    echo ✓ node.exe 运行中
)
echo.

echo 【网络连接】
netstat -ano | findstr "node.exe.*ESTABLISHED" | find /c "ESTABLISHED" > temp.txt
set /p CONN=<temp.txt
del temp.txt
echo 活跃连接数: %CONN%
if %CONN% gtr 0 (
    echo ✓ 网络连接正常
) else (
    echo ⚠ 无网络连接
)
echo.

echo 【磁盘大小】
for /f "tokens=*" %%A in ('dir /s /-c d:\MusicFree-Refactor\frontend\node_modules 2^>nul ^| findstr "字节"') do (
    echo %%A
)
echo.

echo 【最后输出】
type %APPDATA%\npm-cache\_logs\*-debug-0.log 2>nul | findstr /C:"npm" | tail -3
echo.

echo 【建议】
if %CONN% equ 0 (
    echo 警告：无网络连接，npm 可能卡住了
    echo 执行：taskkill /F /IM node.exe
) else (
    echo 提示：网络连接正常，继续等待...
)
echo.

echo 5 秒后刷新...
timeout /t 5

goto monitor_loop
```

---

## 🎯 判断标准总结

| 标志 | 含义 | 状态 |
|------|------|------|
| 输出不断 + node 进程存在 + 网络连接活跃 | ✅ 正常安装中 | 🟢 继续等待 |
| 输出停止 + node 进程消失 + 完整日志 | ✅ 安装完成 | 🟢 检查结果 |
| 输出停止 + node 进程存在 + 无网络连接 | ❌ 网络断线 | 🔴 检查网络 |
| 输出停止 >10分钟 + node 进程存在 | ❌ 安装卡住 | 🔴 停止并重试 |
| 错误消息显示 | ❌ 安装失败 | 🔴 查看错误日志 |

---

## 🧪 快速测试（判断网络是否正常）

```cmd
REM 测试 npm 源连接
ping registry.npmjs.org

REM 测试 GitHub 连接
ping github.com

REM 测试电子镜像
ping ghproxy.com

REM 测试下载速度
curl -o nul -s -w "%{speed_download} bytes/sec\n" https://registry.npmmirror.com
```

---

## 💡 专业建议

1. **第一次等待 15 分钟**
   - 网络慢时 npm 安装可能很久
   - 不要急着按 Ctrl+C

2. **用上述监控工具实时查看**
   - 网络连接活跃 = 在下载
   - 磁盘在写入 = 正常

3. **如果真的卡住了**
   ```cmd
   taskkill /F /IM node.exe
   npm cache clean --force
   npm install --verbose
   ```

4. **出错后用国内镜像**
   ```cmd
   npm config set registry https://registry.npmmirror.com
   npm install
   ```

---

**关键记住：命令行持续输出 + 进程存在 + 有网络连接 = 正常安装中！** ✅
