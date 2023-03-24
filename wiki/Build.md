**此页面适合开发人员 / 贡献者阅读。**

### 环境

* Windows 10 / 11

* Visual Studio 2022

* vcpkg

### 依赖

vcpkg 依赖库安装流程如下：

```
vcpkg install fmt:${arch}-${os} jsoncpp:${arch}-${os} spdlog:${arch}-${os}
```

其中 `${arch}` 为构建的程序架构，可为 `x86` / `x64`；`${os}` 为构建的平台名称，可为 `windows-static` / `linux`。

**请注意：若要 WSL 远程构建，请在 WSL 中新建 vcpkg 环境，避免与 Windows 本机本地环境冲突。**

### 配置

在项目文件夹下打开 Visual Studio 后，可选择 WSL 远程或本机本地构建。

### 运行

构建完毕会自动运行可执行文件，之后还可直接双击运行构建目录中的可执行文件（**程序运行不受运行目录影响，但程序所在目录下必须有所需的依赖文件**）。

### 调试

在相应位置标记断点后选择 Visual Studio 工具顶栏的调试器即可。

_**若有任何问题欢迎与项目维护人员联系！**_
