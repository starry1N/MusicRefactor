# 🔧 npm install 失败 - 快速修复

## ❌ 问题分析

您遇到了两个问题：
1. **网络连接问题**：`ECONNRESET` - Electron 下载中断
2. **文件权限问题**：某些文件无法删除

---

## ✅ 快速修复（推荐）

### 方式 1：自动修复脚本
```cmd
cd d:\MusicFree-Refactor
fix-npm-install.bat
```

脚本会自动：
- ✓ 关闭 npm 进程
- ✓ 清理所有旧文件
- ✓ 清除 npm 缓存
- ✓ 配置加速设置
- ✓ 自动重试 3 次
- ✓ 验证安装

### 方式 2：手动修复

#### 步骤 1：停止所有 Node 进程
```cmd
taskkill /F /IM node.exe
```

#### 步骤 2：以管理员身份删除 node_modules
```cmd
cd d:\MusicFree-Refactor\frontend

REM 删除 node_modules
rmdir /s /q node_modules

REM 删除锁定文件
del /f /q package-lock.json
```

如果还是删除不了，以**管理员身份**运行以下命令：
```cmd
REM 获取文件夹权限
takeown /r /d Y /f node_modules

REM 赋予权限
icacls node_modules /grant:r "%USERNAME%:F" /t

REM 删除
rmdir /s /q node_modules
```

#### 步骤 3：清除 npm 缓存
```cmd
npm cache clean --force
```

#### 步骤 4：配置 npm 加速
```cmd
REM 增加超时时间
npm config set fetch-timeout 120000
npm config set fetch-retry-mintimeout 20000
npm config set fetch-retry-maxtimeout 120000

REM 使用更稳定的镜像（中国）
npm config set registry https://registry.npmmirror.com
```

#### 步骤 5：重新安装
```cmd
cd d:\MusicFree-Refactor\frontend
npm install
```

#### 步骤 6：恢复原始镜像（中国用户）
```cmd
npm config set registry https://registry.npmjs.org/
```

---

## 🌐 网络相关解决方案

### 原因：网络连接不稳定

**症状**：
- `ECONNRESET`
- `ETIMEDOUT`
- `EHOSTUNREACH`

### 解决方案

#### 1. 增加超时时间
```cmd
npm config set fetch-timeout 120000
npm config set fetch-retry-mintimeout 20000
npm config set fetch-retry-maxtimeout 120000
```

#### 2. 使用国内镜像（如果在中国）
```cmd
REM 使用淘宝镜像（更快）
npm config set registry https://registry.npmmirror.com

REM 恢复官方镜像
npm config set registry https://registry.npmjs.org/
```

#### 3. 禁用代理
```cmd
npm config delete proxy
npm config delete https-proxy
npm config delete strict-ssl
```

#### 4. 检查网络连接
```cmd
REM 测试网络
ping registry.npmjs.org

REM 测试 GitHub
ping github.com

REM 如果都超时，说明网络有问题
```

#### 5. 检查防火墙和代理
- 禁用公司代理（如适用）
- 检查防火墙设置
- 关闭 VPN（可能导致超时）

---

## 📁 文件权限问题解决

### 原因：某些文件被锁定

**症状**：
```
EPERM: operation not permitted, rmdir
```

### 解决方案

#### 1. 关闭所有 Node 进程
```cmd
taskkill /F /IM node.exe
taskkill /F /IM npm.cmd
taskkill /F /IM npm
```

#### 2. 以管理员身份运行
- 右键点击 cmd 选择"以管理员身份运行"
- 或在 PowerShell 中运行

#### 3. 改变文件权限
```cmd
REM 获取 node_modules 的所有权
takeown /r /d Y /f node_modules

REM 赋予当前用户完全控制权
icacls node_modules /grant:r "%USERNAME%:F" /t

REM 删除
rmdir /s /q node_modules
```

#### 4. 使用 Windows Cleanup
```cmd
REM 强制删除顽固文件
del /f /s /q node_modules
rmdir /s /q node_modules
```

---

## 🚀 完整修复步骤（综合方案）

```cmd
REM 1. 以管理员身份运行命令行

REM 2. 进入项目
cd d:\MusicFree-Refactor\frontend

REM 3. 停止所有 Node 进程
taskkill /F /IM node.exe

REM 4. 删除旧文件（强制）
takeown /r /d Y /f node_modules 2>nul
icacls node_modules /grant:r "%USERNAME%:F" /t 2>nul
rmdir /s /q node_modules 2>nul
del /f /q package-lock.json 2>nul

REM 5. 清空 npm 缓存
npm cache clean --force

REM 6. 配置 npm
npm config set fetch-timeout 120000
npm config set fetch-retry-mintimeout 20000
npm config set fetch-retry-maxtimeout 120000

REM 7. 如果在中国，使用国内镜像
REM npm config set registry https://registry.npmmirror.com

REM 8. 重新安装（重试 3 次）
npm install
if errorlevel 1 (
    echo 重试 1...
    timeout /t 5
    npm install
)
if errorlevel 1 (
    echo 重试 2...
    timeout /t 5
    npm install
)

REM 9. 验证
npm list electron

REM 10. 恢复官方镜像（如果改过）
REM npm config set registry https://registry.npmjs.org/

echo 完成！
pause
```

---

## ⚙️ npm 配置参考

### 查看当前配置
```cmd
npm config list
```

### 重要配置
```cmd
REM 超时时间（毫秒）
npm config set fetch-timeout 120000

REM 重试间隔
npm config set fetch-retry-mintimeout 20000
npm config set fetch-retry-maxtimeout 120000

REM 日志级别
npm config set loglevel warn

REM 注册表
npm config set registry https://registry.npmjs.org/

REM 严格 SSL（有些镜像需要关闭）
npm config set strict-ssl false
```

### 重置配置
```cmd
npm config reset
```

---

## 🆘 仍然失败？

### 检查清单

- [ ] 以管理员身份运行命令行？
- [ ] node_modules 完全删除了？
- [ ] package-lock.json 删除了？
- [ ] npm 缓存清除了？
- [ ] 网络连接正常？
- [ ] 防火墙/代理已禁用？
- [ ] Node.js 版本 v14+？

### 高级诊断

```cmd
REM 查看详细日志
npm install --verbose

REM 生成诊断报告
npm doctor

REM 查看 npm 日志
type %APPDATA%\npm-cache\_logs\*-debug-0.log

REM 测试网络
curl -v https://registry.npmjs.org/

REM 检查磁盘空间
dir C:\ /-c
```

---

## 📝 推荐方案

### 对于大多数用户
```cmd
fix-npm-install.bat
```

### 如果在公司网络
```cmd
REM 配置代理（如需要）
npm config set proxy http://proxy.company.com:port
npm config set https-proxy https://proxy.company.com:port

REM 或禁用代理
npm config delete proxy
npm config delete https-proxy
npm config set strict-ssl false
```

### 如果在中国
```cmd
npm config set registry https://registry.npmmirror.com
npm install
npm config set registry https://registry.npmjs.org/
```

---

## ✅ 验证修复成功

```cmd
REM 检查 Electron 安装
npm list electron
REM 应显示: electron@25.3.0

REM 测试启动
npm run dev
REM 应打开 Electron 应用
```

---

**问题已解决！现在可以继续开发了！** 🎉

**相关文档**：
- ELECTRON_FIX.md - 详细的 Electron 故障排查
- CLEANUP_GUIDE.md - 项目清理指南
- QUICK_START.md - 快速启动指南
