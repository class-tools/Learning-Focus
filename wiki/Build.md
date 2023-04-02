**此页面适合开发人员 / 贡献者阅读。**

_**请注意，由于 `dlib` 的依赖库 `lapack` 在 vcpkg 中存在兼容性问题，故本项目暂时不能在 Linux 平台下编译，仅支持 Windows 平台。**_

_**Macintosh OS ~~机子买不起~~ 暂未测试，故也不支持。**_

_**所有操作系统均仅支持 x64 架构。**_

### 环境

* Windows 10 / 11

* Visual Studio 2022

* vcpkg

### 依赖

vcpkg 依赖库随 CMake Configuration 构建并安装，不需手动安装。

设置环境变量 `VCPKG_ROOT` 指定 vcpkg 路径，便于编译（**不指定则 `CMakeLists.txt` 将会报错**）。

**请注意：若要 WSL 远程构建，请在 WSL 中新建 vcpkg 环境，避免与 Windows 本机本地环境冲突。**

### 配置

在项目文件夹下打开 Visual Studio 后，可选择 WSL 远程或本机本地构建。

### 运行

构建完毕可直接双击运行构建目录中的可执行文件（**程序运行不受运行目录影响，但程序所在目录下必须有所需的依赖文件**）。

运行前务必下载 DLib 人脸关键点检测模型 [(官网)](http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2) [(GitHub)](https://github.com/fenollp/data.shape_predictor_68_face_landmarks/raw/main/shape_predictor_68_face_landmarks.dat)，（解压后）放入源代码目录或程序所在目录。

### 调试

在相应位置标记断点后选择 Visual Studio 工具顶栏的调试器即可。

_**若有任何问题欢迎与项目维护人员联系！**_
