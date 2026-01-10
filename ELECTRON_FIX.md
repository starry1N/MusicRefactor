# Electron 依赖修复指南

## 问题描述

启动 Electron 时出现错误：
```
It looks like you are missing some dependencies you need to get Electron running.
Make sure you have git installed and Node.js version >= 14.17.5
```

---

## 🔧 快速修复（推荐）

执行自动修复脚本：
```cmd
cd d:\MusicFree-Refactor
fix-electron.bat
```

脚本会自动：
1. ✓ 检查 Node.js 版本
2. ✓ 检查 Git 安装
3. ✓ 清理旧的依赖
4. ✓ 重新安装 npm 包
5. ✓ 验证 Electron

---

## 📋 手动修复步骤

如果自动脚本不工作，按以下步骤手动修复：

### 步骤1：验证 Node.js 版本

```cmd
node --version
```

**要求**：v14.17.5 或更高版本

如果版本过低：
- 访问 https://nodejs.org/
- 下载 LTS 版本（16.x 或 18.x）
- 重新安装

### 步骤2：验证 Git 安装

```cmd
git --version
```

如果显示"git 不是内部或外部命令"：
- 访问 https://git-scm.com/download/win
- 下载并安装 Git for Windows
- 安装时勾选 "Add Git to PATH"
- 重启命令行

### 步骤3：清理旧的依赖

```cmd
cd d:\MusicFree-Refactor\frontend

REM 删除 node_modules
rmdir /s /q node_modules

REM 删除锁定文件
del package-lock.json
```

### 步骤4：清除 npm 缓存

```cmd
npm cache clean --force
```

### 步骤5：重新安装依赖

```cmd
npm install
```

**耐心等待**，这可能需要 5-10 分钟

### 步骤6：验证安装

```cmd
npm list electron
```

应该显示：
```
musicfree-desktop-refactored@0.1.0
└── @electron-forge/cli@6.4.1
    └── electron@25.3.0
```

---

## 🐛 常见问题解决

### 问题1：npm install 卡住或超时

**症状**：npm 命令长时间无响应

**解决方案**：
```cmd
REM 增加超时时间
npm config set fetch-timeout 120000
npm config set fetch-retry-mintimeout 20000
npm config set fetch-retry-maxtimeout 120000

REM 重试安装
npm install
```

### 问题2：磁盘空间不足

**症状**：`ENOSPC: no space left on device`

**解决方案**：
```cmd
REM 检查磁盘空间（需要至少 1GB）
REM 删除不需要的文件
REM 或更改 npm 缓存位置

npm config set cache C:\npm-cache
```

### 问题3：网络连接问题

**症状**：`ECONNREFUSED` 或 `ETIMEDOUT`

**解决方案**：
```cmd
REM 使用国内镜像（如果在中国）
npm config set registry https://registry.npmmirror.com

REM 重新安装
npm install

REM 恢复官方镜像
npm config set registry https://registry.npmjs.org/
```

### 问题4：权限错误

**症状**：`EACCES: permission denied`

**解决方案**：
- 以管理员身份运行命令行
- 或更改 npm 全局目录：
```cmd
mkdir %APPDATA%\npm
npm config set prefix %APPDATA%\npm
```

### 问题5：Electron 下载失败

**症状**：`electron ERR! fetch failed`

**解决方案**：
```cmd
REM 手动指定 Electron 镜像
set ELECTRON_MIRROR=https://github.com/electron/electron/releases/download/

REM 重新安装
npm install
```

---

## ✅ 验证修复成功

运行以下命令确认：

```cmd
REM 检查 Node.js
node --version
REM 应显示：v16.x 或更高

REM 检查 npm
npm --version
REM 应显示：8.x 或更高

REM 检查 Git
git --version
REM 应显示：git version 2.x

REM 检查 Electron
cd frontend
npm list electron
REM 应显示：electron@25.3.0

REM 测试启动
npm run dev
REM 应打开 Electron 应用窗口
```

---

## 📊 系统要求检查

完整的系统配置检查：

```cmd
REM 检查所有要求
echo Node.js 版本:
node --version

echo npm 版本:
npm --version

echo Git 版本:
git --version

echo Python（某些模块需要）:
python --version

echo 磁盘空间（需要至少 1GB）:
dir C:\

echo 内存（需要至少 2GB）:
systeminfo | findstr /C:"Total Physical Memory"
```

---

## 🌐 代理设置（公司网络）

如果在公司网络中，可能需要配置代理：

```cmd
REM 设置 HTTP 代理
npm config set proxy [http://proxy.company.com]:[port]
npm config set https-proxy [https://proxy.company.com]:[port]

REM 设置认证信息（如需要）
npm config set proxy-user [username]
npm config set proxy-pass [password]

REM 查看当前配置
npm config list

REM 清除代理设置
npm config delete proxy
npm config delete https-proxy
```

---

## 📝 完整的依赖重置

如果上述方法都不行，执行完整的重置：

```cmd
cd d:\MusicFree-Refactor

REM 删除所有临时文件
rmdir /s /q frontend\node_modules
del frontend\package-lock.json
rmdir /s /q frontend\.webpack
rmdir /s /q frontend\out

REM 清除所有 npm 缓存
npm cache clean --force

REM 清除 npm 配置
npm config delete proxy
npm config delete https-proxy
npm config set registry https://registry.npmjs.org/

REM 重新安装
cd frontend
npm install --verbose

REM 验证
npm list electron
npm run dev
```

---

## 🔧 高级诊断

如果问题持续，运行诊断：

```cmd
REM 检查 npm 日志
cd frontend
npm install 2>&1 | tee npm-install.log

REM 检查 Electron 下载日志
set npm_config_loglevel=verbose
npm install electron@25.3.0 --verbose

REM 生成诊断报告
npm doctor

REM 检查磁盘使用
du -sh node_modules
```

---

## 🎯 预防措施

为了避免今后出现此问题：

1. **定期更新 Node.js**
   ```cmd
   node --version
   REM 每季度检查一次，确保版本最新
   ```

2. **使用 nvm（Node Version Manager）**
   - 便于切换 Node 版本
   - 自动管理依赖
   - 下载：https://github.com/coreybutler/nvm-windows

3. **使用 CI/CD 自动化**
   - GitHub Actions 自动测试依赖
   - 自动检测版本不兼容

4. **维护 .nvmrc 文件**
   ```
   16.14.0
   ```
   指定项目使用的 Node 版本

---

## 📞 获取帮助

如果问题仍未解决：

1. **查看 npm 日志**
   ```cmd
   cat %APPDATA%\npm-debug.log
   ```

2. **检查网络连接**
   ```cmd
   ping registry.npmjs.org
   ```

3. **查看 Electron 官方文档**
   https://www.electronjs.org/docs

4. **提交 Issue**
   - 附上完整的错误日志
   - 操作系统版本
   - Node.js 版本
   - npm 版本

---

## ✨ 修复完成后

依赖修复完成后，可以：

```cmd
REM 启动前端应用
npm run dev

REM 或启动后端
cd ..\backend
cmake --build . --config Release
.\bin\Release\musicfree_server.exe 8888
```

---

**最后更新**：2026年1月9日  
**相关文档**：QUICK_START.md, STARTUP_GUIDE.md
