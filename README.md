# RSA 签名与验证工具 (C++ & OpenSSL)

这是一个使用 C++ 和 OpenSSL 库实现的简单命令行工具，用于执行 RSA 数字签名和验证操作。

## 功能特性

* **RSA 算法:** 使用 RSA 算法进行非对称加密操作。
* **密钥长度:** 强制使用 4096 位的 RSA 密钥长度，提供较高的安全性。
* **签名:** 使用私钥对用户输入的消息生成数字签名。
* **验证:** 使用公钥验证用户提供的签名是否与原始消息匹配。
* **哈希算法:** 签名和验证过程中默认使用 SHA-256 哈希算法。
* **用户交互:** 通过命令行菜单与用户交互，引导完成签名或验证流程。
* **结果显示:** 清晰地打印签名（Base64 编码）和验证结果（成功/失败）。
* **循环操作:** 每次操作完成后清除屏幕并返回主菜单，直到用户选择退出。
* **OpenSSL 库:** 核心加密功能依赖于 OpenSSL 库。
* **CMake 构建:** 提供 `CMakeLists.txt` 文件，方便跨平台编译。
* **代码注释:** C++ 源代码包含中文注释，方便理解。
* **MIT 许可证:** 项目代码基于 MIT 许可证发布。

**注意:** 当前版本的工具在每次运行时会**在内存中生成一个新的 4096 位 RSA 密钥对**。签名和验证操作都使用这个临时的内存密钥对。它**不会**将密钥保存到文件，也不会从文件加载现有密钥。

## 项目结构


rsa-sign-verify-tool/
├── CMakeLists.txt         # CMake 构建配置文件
├── LICENSE                # MIT 许可证文件
├── README.md              # 本说明文件
├── include/
│   └── rsa_utils.h        # RSA 工具函数的头文件
└── src/
├── main.cpp           # 主程序入口和用户交互逻辑
└── rsa_utils.cpp      # RSA 工具函数的实现

## 环境依赖

在编译和运行此项目之前，请确保你的系统已安装以下软件：

1.  **C++ 编译器:** 支持 C++11 或更高标准的编译器 (例如 GCC, Clang, MSVC)。
2.  **CMake:** 版本 3.10 或更高。 ([https://cmake.org/](https://cmake.org/))
3.  **OpenSSL 开发库:** 需要包含头文件和库文件。
    * **Debian/Ubuntu:** `sudo apt-get update && sudo apt-get install build-essential cmake libssl-dev`
    * **Fedora/CentOS/RHEL:** `sudo dnf groupinstall "Development Tools" && sudo dnf install cmake openssl-devel`
    * **macOS (使用 Homebrew):** `brew install cmake openssl` (如果 OpenSSL 未链接到标准路径，可能需要在 CMake 配置时指定路径)
    * **Windows:** 推荐使用包管理器如 [vcpkg](https://github.com/microsoft/vcpkg) (`vcpkg install openssl:x64-windows`) 或 MSYS2/MinGW，或者从 [OpenSSL 官网](https://www.openssl.org/)下载预编译库并手动配置。

## 编译与运行

1.  **克隆仓库 (如果已上传到 Git):**
    ```bash
    git clone <your-repository-url>
    cd rsa-sign-verify-tool
    ```
    或者直接将代码文件放置在 `rsa-sign-verify-tool` 目录下。

2.  **创建构建目录:**
    ```bash
    mkdir build
    cd build
    ```

3.  **运行 CMake 配置:**
    ```bash
    # Linux/macOS/Windows (标准路径)
    cmake ..

    # 如果 OpenSSL 安装在非标准路径，可能需要指定:
    # cmake .. -DOPENSSL_ROOT_DIR=/path/to/your/openssl

    # 如果使用 vcpkg (Windows):
    # cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
    ```

4.  **编译项目:**
    ```bash
    cmake --build .
    # 或者在 Linux/macOS 上直接运行:
    # make
    ```

5.  **运行程序:**
    编译成功后，可执行文件 `rsa_tool` (或 `rsa_tool.exe`) 会出现在 `build` 目录下。
    ```bash
    # Linux/macOS
    ./rsa_tool

    # Windows (PowerShell)
    .\rsa_tool.exe

    # Windows (CMD)
    rsa_tool.exe
    ```

## 使用说明

程序启动后会首先生成一个临时的 4096 位 RSA 密钥对，然后显示主菜单：


========================================
RSA 签名与验证工具 (OpenSSL x.x.x)
请选择操作:
 * 签名消息
 * 验证签名
 * 退出程序
请输入选项 (1-3):

1.  **签名消息:**
    * 输入 `1` 并按 Enter。
    * 程序会提示你输入要签名的消息。
    * 输入消息后按 Enter。
    * 程序将使用内存中的私钥对消息进行签名，并显示 Base64 编码的签名结果。
2.  **验证签名:**
    * 输入 `2` 并按 Enter。
    * 程序会提示你输入原始消息。
    * 输入原始消息后按 Enter。
    * 程序会提示你输入 Base64 格式的签名。
    * 粘贴或输入签名后按 Enter。
    * 程序将使用内存中的公钥验证签名是否与消息匹配，并显示验证结果（成功或失败）。
3.  **退出程序:**
    * 输入 `3` 并按 Enter，程序将退出。

每次签名或验证操作完成后，按 Enter 键即可清除屏幕并返回主菜单。

## 许可证

本项目采用 **MIT 许可证**。详情请参阅 `LICENSE` 文件。
