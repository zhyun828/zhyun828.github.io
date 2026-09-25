# CMake 与 CMakeLists.txt 教程

> CMake 不是编译器，而是**构建系统生成器**。它读取项目中的 `CMakeLists.txt`，再生成 Ninja、Makefile、Visual Studio 工程等真正执行编译的构建文件。

本文以现代 CMake 的“目标（target）”思想为主线，适合从零入门，也可作为日常速查手册。

---

## 零、开始使用 CMake 前：先认识编译文件与 gcc/g++

### 1. C/C++ 从源码到程序会产生哪些文件

C/C++ 程序通常要经过**预处理、编译、汇编、链接**四个阶段：

```text
源文件              预处理文件          汇编文件       目标文件          最终产物
.c / .cpp  ───────▶  .i / .ii  ───────▶  .s  ───────▶  .o / .obj  ───────▶  可执行文件或库
             预处理             编译          汇编              链接
```

| 扩展名或文件 | 含义 | 所处阶段或用途 |
| --- | --- | --- |
| `.c` | C 源文件 | 程序员编写的源码 |
| `.cc`、`.cpp`、`.cxx` | C++ 源文件 | 三者都是常见的 C++ 源文件命名方式 |
| `.h` | 头文件 | 可用于 C 或 C++，存放声明、宏、类型等 |
| `.hpp`、`.hxx` | C++ 头文件 | 通常表示其中包含 C++ 代码 |
| `.i` | 预处理后的 C 文件 | 宏已展开，头文件内容已插入 |
| `.ii` | 预处理后的 C++ 文件 | 与 `.i` 类似，但用于 C++ |
| `.s`、`.asm` | 汇编代码 | 编译器产生或程序员手写的汇编源码 |
| `.o` | 目标文件 | GCC/Clang 在 Linux、macOS 等平台生成的机器码文件，尚未完成链接 |
| `.obj` | 目标文件 | Windows/MSVC 常用的目标文件扩展名 |
| `.d` | 依赖文件 | 记录某个源文件依赖哪些头文件，常用于增量构建 |
| `.a` | 静态库 | Linux 等平台上多个 `.o` 的归档集合，链接时复制所需代码 |
| `.lib` | 静态库或导入库 | Windows 上可能是静态库，也可能是 DLL 对应的导入库 |
| `.so` | 动态库 | Linux 的共享库，程序运行时加载 |
| `.dll` | 动态库 | Windows 的动态链接库 |
| `.dylib` | 动态库 | macOS 的动态库 |
| `.exe` | 可执行文件 | Windows 的程序文件；Linux 可执行文件通常没有扩展名 |
| `.pdb` | 调试符号文件 | MSVC 常用，供调试器映射源码、函数和变量 |
| `.dSYM` | 调试符号包 | macOS 常见的调试信息目录 |
| `compile_commands.json` | 编译命令数据库 | 记录每个源文件的实际编译命令，供 clangd、clang-tidy 等工具使用 |
| `CMakeCache.txt` | CMake 配置缓存 | 保存编译器路径、选项和探测结果，不是编译器产生的目标文件 |

其中最容易混淆的是 `.o`：

```text
main.cpp ──编译──▶ main.o ─┐
                            ├──链接──▶ app
math.cpp ──编译──▶ math.o ─┘
```

每个 `.cpp` 通常先单独变成一个 `.o`。`.o` 中已经是机器码，但其中可能仍引用其他文件定义的函数，所以一般不能直接作为完整程序运行。链接器负责解析这些符号引用，并把目标文件和库组合成最终程序。

> `.o`、`.obj`、`.i`、`.ii` 和 `.s` 通常属于构建产物，应放在 `build/` 中并排除在 Git 版本控制之外。

### 2. gcc 与 g++ 的区别

GCC 是 GNU Compiler Collection。日常命令中：

- `gcc` 通常作为 C 编译驱动程序；
- `g++` 通常作为 C++ 编译驱动程序；
- 两者都能根据文件扩展名调用相应的编译前端；
- 最明显的区别在链接阶段：`g++` 会自动链接 C++ 标准库，而 `gcc` 默认不会。

因此推荐：

```bash
# C 程序使用 gcc
gcc main.c -o app

# C++ 程序使用 g++
g++ main.cpp -o app
```

不建议直接使用 `gcc` 链接普通 C++ 程序，否则可能出现 `std::cout`、`std::string` 等符号未定义的问题。

### 3. 使用 gcc/g++ 查看四个编译阶段

以下命令以 C++ 文件 `main.cpp` 为例。

#### 仅预处理：`.cpp` → `.ii`

```bash
g++ -E main.cpp -o main.ii
```

`-E` 表示完成预处理后停止。该阶段会处理：

- `#include` 头文件包含；
- `#define` 宏替换；
- `#if`、`#ifdef` 等条件编译；
- 删除注释。

#### 编译成汇编：`.cpp` → `.s`

```bash
g++ -S main.cpp -o main.s
```

`-S` 表示生成汇编代码后停止。

#### 编译但不链接：`.cpp` → `.o`

```bash
g++ -c main.cpp -o main.o
```

`-c` 表示只生成目标文件，不执行链接。这也是构建系统编译多个源文件时的基本方式。

#### 链接：多个 `.o` → 可执行文件

```bash
g++ main.o math.o -o app
```

也可以用一条命令完成编译和链接：

```bash
g++ main.cpp math.cpp -o app
```

但项目较大时，如果只修改了 `main.cpp`，分开编译只需重新生成 `main.o`，然后重新链接，不必再次编译 `math.cpp`。CMake 生成的构建系统会自动完成这种增量构建。

### 4. gcc/g++ 常用编译选项

| 选项 | 作用 | 示例 |
| --- | --- | --- |
| `-o <文件>` | 指定输出文件 | `g++ main.cpp -o app` |
| `-c` | 只编译，不链接 | `g++ -c main.cpp -o main.o` |
| `-E` | 只进行预处理 | `g++ -E main.cpp -o main.ii` |
| `-S` | 编译到汇编阶段 | `g++ -S main.cpp -o main.s` |
| `-I<目录>` | 添加头文件搜索目录 | `g++ -Iinclude main.cpp -o app` |
| `-L<目录>` | 添加库文件搜索目录 | `g++ main.o -Llib -lmath -o app` |
| `-l<名称>` | 链接指定库 | `-lm` 表示查找 `libm.so` 或 `libm.a` |
| `-lm` | 链接数学库 | 用于使用 `sqrt`、`sin` 等数学函数；等价于链接 `libm.so` 或 `libm.a` |
| `-D<宏>` | 定义预处理宏 | `-DENABLE_LOGGING=1` |
| `-U<宏>` | 取消预处理宏定义 | `-UDEBUG` |
| `-std=c++17` | 指定 C++ 标准 | 也可使用 `c++20`、`c++23` 等 |
| `-std=c11` | 指定 C 标准 | 也可使用 `c17` 等 |
| `-Wall` | 开启一组常用警告 | 建议开发时开启 |
| `-Wextra` | 开启更多警告 | 通常与 `-Wall` 一起使用 |
| `-Wpedantic` | 检查非标准扩展 | 有助于提高可移植性 |
| `-Werror` | 将警告视为错误 | 适合严格的 CI，日常引入旧项目时需谨慎 |
| `-g` | 生成调试信息 | 供 GDB 等调试器使用 |
| `-O0` | 关闭优化 | 适合调试 |
| `-O2` | 开启常用优化 | 常用于发布版本 |
| `-O3` | 更激进地优化 | 不一定总比 `-O2` 更适合 |
| `-MMD -MP` | 生成用户头文件依赖信息 | 常用于 Makefile 增量构建 |
| `-pthread` | 启用 POSIX 线程相关编译和链接选项 | Linux 多线程程序常用 |

一个适合开发调试的 C++ 命令：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -O0 \
    -Iinclude src/main.cpp src/math.cpp -o app
```

一个简单的发布构建命令：

```bash
g++ -std=c++17 -O2 -DNDEBUG \
    -Iinclude src/main.cpp src/math.cpp -o app
```

### 5. 手工创建和使用库

#### 静态库

先编译目标文件，再使用 `ar` 创建静态库：

```bash
g++ -std=c++17 -Iinclude -c src/math.cpp -o math.o
ar rcs libmath.a math.o
```

链接静态库：

```bash
g++ -std=c++17 -Iinclude main.cpp -L. -lmath -o app
```

`-lmath` 会查找名为 `libmath.a` 或 `libmath.so` 的库。参数顺序在部分平台上会影响静态库符号解析，通常把库参数放在使用它的目标文件之后。

#### 动态库

Linux 下可这样创建共享库：

```bash
g++ -std=c++17 -fPIC -Iinclude -c src/math.cpp -o math.o
g++ -shared math.o -o libmath.so
```

链接共享库：

```bash
g++ -std=c++17 -Iinclude main.cpp -L. -lmath -o app
```

运行时，系统还必须能找到 `libmath.so`。实际项目应通过规范的安装目录、RPATH 或系统动态库配置解决，不建议长期依赖临时修改环境变量。

这些手工命令有助于理解构建原理，但跨平台项目一般应让 CMake 根据目标声明自动生成正确命令。

### 6. CMakeLists.txt 的基础格式

下面是一份最常用的基础模板。标记的含义为：

- **【必选】**：规范的独立 CMake 项目应当提供；
- **【目标必选】**：要产生程序或库时，至少需要定义一个构建目标；
- **【二选一】**：根据最终产物选择其中一种；
- **【可选】**：没有对应需求时可以删除；
- **【按需】**：由项目代码和依赖决定。

```cmake
# 【必选】指定项目要求的最低 CMake 版本，并确定相关策略行为。
# 应放在顶层 CMakeLists.txt 开头。
cmake_minimum_required(VERSION 3.20)

# 【必选】声明项目。
# 项目名称 HelloProject 必须填写；VERSION、DESCRIPTION、LANGUAGES 为可选参数。
# 对纯 C++ 项目推荐明确写 LANGUAGES CXX，避免探测不需要的 C 编译器。
project(
    HelloProject                         # 【必选】项目名称
    VERSION 1.0.0                        # 【可选】项目版本
    DESCRIPTION "A simple CMake project" # 【可选】项目描述
    LANGUAGES CXX                        # 【可选】项目语言；省略时默认启用 C 和 CXX
)

# 【可选】定义允许用户通过 -D 修改的构建选项。
option(ENABLE_LOGGING "Enable logging" ON)

# 【目标必选，二选一】创建可执行文件。
# app 是目标名，后面列出生成它所需的源文件。
add_executable(app
    src/main.cpp                         # 【必选】该目标至少要有可编译源码，可在此处或稍后添加
    src/math.cpp                         # 【按需】其他源文件
)

# 【目标必选，二选一】如果项目要生成库，则使用 add_library。
# STATIC 可替换为 SHARED；如果已经创建 app 且不需要库，可删除下面这一段。
# add_library(math STATIC
#     src/math.cpp
# )

# 【推荐】声明目标需要的 C++ 标准。
# PRIVATE 表示该要求只用于编译 app，不向其他目标传递。
target_compile_features(app PRIVATE cxx_std_17)

# 【按需】添加头文件搜索目录。
target_include_directories(app
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 【按需】添加预处理宏，相当于编译命令中的 -D。
if(ENABLE_LOGGING)
    target_compile_definitions(app PRIVATE ENABLE_LOGGING=1)
endif()

# 【可选但推荐】开启常用编译警告，并处理 MSVC 与 GCC/Clang 的差异。
if(MSVC)
    target_compile_options(app PRIVATE /W4 /permissive-)
else()
    target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 【按需】链接项目内部库或 find_package 找到的第三方库。
# target_link_libraries(app PRIVATE math)

# 【可选】定义安装规则。
# install(TARGETS app RUNTIME DESTINATION bin)
```

模板最精简后只需：

```cmake
cmake_minimum_required(VERSION 3.20)  # 【必选】最低版本
project(Hello LANGUAGES CXX)          # 【必选】项目声明
add_executable(hello main.cpp)        # 【目标必选】创建构建目标
```

对应命令：

```bash
# 1. 配置：读取 CMakeLists.txt，在 build 中生成构建系统
cmake -S . -B build # -S 指定源码目录，-B 指定构建目录, . 代表当前目录 build, 代表在 `build/` 目录内部生成**构建文件**

# 2. 构建：让 CMake 调用生成器，再由生成器调用编译器
cmake --build build #--build 告诉cmake去执行编译 build就是上一步生成的构建系统所在目录
```

> 严格来说，CMake 对某些缺失命令可能会采用默认行为并给出警告，但一个清晰、可维护的顶层项目应显式写出 `cmake_minimum_required()` 和 `project()`。产生实际构建结果时，还必须通过 `add_executable()`、`add_library()` 或子目录中的等价声明创建目标。

### 7. 常用 CMake 命令与“是否必选”

| 命令 | 是否必选 | 说明 |
| --- | --- | --- |
| `cmake_minimum_required()` | 顶层项目必选 | 指定最低版本，并采用对应版本的策略行为 |
| `project()` | 顶层项目必选 | 声明项目名称，可同时声明版本和语言 |
| `add_executable()` | 按产物选择 | 创建可执行目标；程序项目通常需要 |
| `add_library()` | 按产物选择 | 创建库目标；库项目或分层项目常用 |
| `add_subdirectory()` | 多目录项目按需 | 处理子目录中的 `CMakeLists.txt` |
| `target_sources()` | 按需 | 创建目标后继续添加源文件 |
| `target_compile_features()` | 推荐 | 以跨平台方式声明 C++ 标准或语言特性 |
| `target_include_directories()` | 有自定义头文件目录时需要 | 添加头文件搜索路径 |
| `target_compile_definitions()` | 有编译宏时需要 | 添加宏定义 |
| `target_compile_options()` | 可选 | 添加警告、优化等目标专属编译选项 |
| `target_link_libraries()` | 有库依赖时需要 | 链接库，并传播相应使用要求 |
| `find_package()` | 使用外部包时通常需要 | 查找已安装或工具链提供的依赖包 |
| `option()` | 可选 | 向用户暴露开关 |
| `enable_testing()` / `include(CTest)` | 有测试时需要 | 启用 CTest 测试功能 |
| `install()` | 需要安装或发布时使用 | 声明安装规则 |

---

## 一、先理解完整构建流程

一个 C++ 项目从源码到可执行文件，通常经历：

```text
CMakeLists.txt
      │  cmake -S . -B build
      ▼
构建系统（Ninja / Makefile / Visual Studio 工程）
      │  cmake --build build
      ▼
编译 .cpp → 目标文件 .o/.obj → 链接 → 可执行文件或库
```

常见工具的职责：

| 工具 | 职责 |
| --- | --- |
| GCC、Clang、MSVC | 编译和链接 C/C++ 代码 |
| Ninja、Make | 按依赖关系执行具体构建命令 |
| CMake | 根据统一配置生成上述构建系统 |
| CTest | 运行由 CMake 注册的测试 |
| CPack | 将构建结果制作成安装包 |

使用 CMake 的主要价值是：

- 同一份项目配置可以适配 Windows、Linux 和 macOS；
- 可以管理源文件、头文件、库和它们之间的依赖；
- 可以切换编译器、构建类型和编译选项；
- 能与测试、安装、打包和 IDE 集成。

---

## 二、第一个 CMake 项目

### 1. 项目结构

```text
hello_cmake/
├── CMakeLists.txt
└── main.cpp
```

`main.cpp`：

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, CMake!\n";
    return 0;
}
```

`CMakeLists.txt`：

```cmake
# 声明所需的最低 CMake 版本
cmake_minimum_required(VERSION 3.20)

# 定义项目名称、版本和使用的语言
project(HelloCMake VERSION 1.0.0 LANGUAGES CXX)

# 创建可执行目标 hello，并指定它的源文件
add_executable(hello main.cpp)

# 让目标使用 C++17；不必手写 -std=c++17 或 /std:c++17
target_compile_features(hello PRIVATE cxx_std_17)
```

### 2. 配置、构建和运行

在项目根目录执行：

```bash
# -S 指定源码目录，-B 指定构建目录
cmake -S . -B build

# 调用生成的构建系统进行编译
cmake --build build
```

运行程序：

```bash
# Linux / macOS，以及 Ninja 等单配置生成器
./build/hello

# Windows 常见情况
.\build\Debug\hello.exe
```

建议始终使用独立的 `build/` 目录，不要让缓存和中间文件污染源码目录。这种方式称为 **out-of-source build**。

清理项目时通常直接删除 `build/` 即可，源码不受影响。

---

## 三、CMakeLists.txt 基础语法

### 1. 命令与参数

CMake 命令不区分大小写，但现代项目通常统一使用小写命令：

```cmake
add_executable(app main.cpp)
```

参数以空格或换行分隔，圆括号内不需要逗号或分号：

```cmake
add_executable(app
    src/main.cpp
    src/config.cpp
)
```

### 2. 注释

```cmake
# 单行注释

#[[
多行注释
可以跨越多行
]]
```

### 3. 变量

```cmake
set(APP_NAME demo)
set(APP_SOURCES
    src/main.cpp
    src/config.cpp
)

add_executable(${APP_NAME} ${APP_SOURCES})
```

读取变量时使用 `${变量名}`。列表在 CMake 内部以分号分隔，因此下面两种写法等价：

```cmake
set(SOURCES a.cpp b.cpp c.cpp)
set(SOURCES "a.cpp;b.cpp;c.cpp")
```

取消变量：

```cmake
unset(APP_NAME)
```

### 4. 字符串与引号

没有空格的普通参数通常可以不加引号；文件路径或字符串可能包含空格时，应加引号：

```cmake
set(MESSAGE_TEXT "hello cmake")
message(STATUS "message = ${MESSAGE_TEXT}")
```

常用消息等级：

```cmake
message(STATUS "普通状态信息")
message(WARNING "警告，但继续配置")
message(FATAL_ERROR "错误，并立即终止配置")
```

### 5. 条件判断

```cmake
if(WIN32)
    message(STATUS "Windows platform")
elseif(APPLE)
    message(STATUS "Apple platform")
elseif(UNIX)
    message(STATUS "Unix-like platform")
endif()
```

常见判断：

```cmake
if(TARGET mylib)                 # 目标是否存在
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/config.json")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
endif()

if(MSVC)                         # 是否使用 Microsoft 编译器
endif()
```

布尔运算使用 `AND`、`OR` 和 `NOT`：

```cmake
if(ENABLE_TESTS AND NOT WIN32)
    # ...
endif()
```

### 6. 循环

```cmake
foreach(source IN ITEMS main.cpp config.cpp log.cpp)
    message(STATUS "source: ${source}")
endforeach()

set(MODULES core network storage)
foreach(module IN LISTS MODULES)
    message(STATUS "module: ${module}")
endforeach()
```

### 7. 函数

```cmake
function(enable_project_warnings target_name)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

enable_project_warnings(app)
```

函数有自己的变量作用域，更适合封装可复用逻辑。

---

## 四、最重要的概念：目标 target

现代 CMake 的核心不是“设置一堆全局变量”，而是：

1. 创建目标；
2. 给目标附加属性；
3. 建立目标之间的依赖关系。

常见目标类型：

```cmake
# 可执行文件
add_executable(app src/main.cpp)

# 静态库：Linux 通常生成 libmath.a，Windows 通常生成 math.lib
add_library(math STATIC src/math.cpp)

# 动态库：Linux 通常生成 libnetwork.so，Windows 通常生成 .dll 和导入库
add_library(network SHARED src/network.cpp)

# 由 BUILD_SHARED_LIBS 决定生成静态库还是动态库
add_library(common src/common.cpp)

# 只有使用要求、不直接编译源码的接口库
add_library(project_options INTERFACE)
```

`app`、`math`、`network` 都是逻辑目标名。后续应围绕目标设置头文件目录、编译选项、宏和依赖。

---

## 五、PRIVATE、PUBLIC 与 INTERFACE

这三个关键字描述“使用要求”是否传递：

| 关键字 | 当前目标使用 | 依赖当前目标的目标使用 |
| --- | :---: | :---: |
| `PRIVATE` | 是 | 否 |
| `PUBLIC` | 是 | 是 |
| `INTERFACE` | 否 | 是 |

假设项目结构如下：

```text
project/
├── CMakeLists.txt
├── include/
│   └── math/add.hpp
└── src/
    ├── add.cpp
    └── main.cpp
```

配置：

```cmake
add_library(math STATIC src/add.cpp)

# math 自己和使用 math 的目标都需要看到公开头文件，所以用 PUBLIC
target_include_directories(math
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE math)
```

因为 `math` 将 `include/` 声明为 `PUBLIC`，`app` 链接 `math` 后会自动继承该头文件搜索路径。不要再给 `app` 重复添加同一路径。

判断方法：

- 只在 `.cpp` 内部使用的依赖：`PRIVATE`；
- 出现在公开头文件中、使用者也必须知道的依赖：`PUBLIC`；
- 当前目标本身不使用，只要求使用者使用：`INTERFACE`。

例如纯头文件库：

```cmake
add_library(vector_math INTERFACE)
target_include_directories(vector_math
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(vector_math INTERFACE cxx_std_20)
```

---

## 六、常用的 target_* 命令

### 1. 添加源文件

```cmake
add_executable(app)

target_sources(app
    PRIVATE
        src/main.cpp
        src/config.cpp
)
```

优先显式列出源码。这样新增、删除文件时，构建配置的变化更清楚。

若确实需要自动收集文件，可使用：

```cmake
file(GLOB APP_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(app ${APP_SOURCES})
```

`CONFIGURE_DEPENDS` 会要求 CMake 在文件集合变化时重新检查，但大型项目中仍通常推荐显式维护列表。

### 2. 添加头文件搜索路径

```cmake
target_include_directories(app
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
```

使用库时，可分别声明构建阶段和安装阶段的路径：

```cmake
target_include_directories(math
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
```

### 3. 指定 C++ 标准

推荐写法：

```cmake
target_compile_features(app PRIVATE cxx_std_20)
```

也可以用属性强制指定：

```cmake
set_target_properties(app PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
```

其中 `CXX_EXTENSIONS OFF` 表示尽量使用标准模式，例如 GCC 使用 `-std=c++20`，而不是 `-std=gnu++20`。

### 4. 添加编译宏

```cmake
target_compile_definitions(app
    PRIVATE
        APP_VERSION="${PROJECT_VERSION}"
        ENABLE_LOGGING
)
```

C++ 代码中即可使用：

```cpp
#ifdef ENABLE_LOGGING
// 日志代码
#endif
```

不要在 CMake 中写 `-DENABLE_LOGGING`，`target_compile_definitions` 会负责添加平台对应的参数。

### 5. 添加编译选项

```cmake
if(MSVC)
    target_compile_options(app PRIVATE /W4 /permissive-)
else()
    target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic)
endif()
```

`target_compile_options` 用于编译选项，而链接选项应使用：

```cmake
target_link_options(app PRIVATE ...)
```

### 6. 链接库

```cmake
target_link_libraries(app
    PRIVATE
        math
        network
)
```

只要有对应的 CMake 目标，优先链接目标名，不要手工拼接 `.a`、`.so`、`.lib` 或 `.dll` 路径。

### 7. 设置输出名称和目录

```cmake
set_target_properties(app PROPERTIES
    OUTPUT_NAME "my_app"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
```

库还可使用：

```cmake
set_target_properties(math PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
```

---

## 七、多目录项目

### 1. 推荐结构

```text
calculator/
├── CMakeLists.txt
├── app/
│   ├── CMakeLists.txt
│   └── main.cpp
├── include/
│   └── calculator/calculator.hpp
├── src/
│   ├── CMakeLists.txt
│   └── calculator.cpp
└── tests/
    ├── CMakeLists.txt
    └── calculator_test.cpp
```

根目录 `CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.20)
project(Calculator VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_TESTING "Build project tests" ON)

add_subdirectory(src)
add_subdirectory(app)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
```

`src/CMakeLists.txt`：

```cmake
add_library(calculator
    calculator.cpp
)

target_include_directories(calculator
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(calculator PUBLIC cxx_std_17)
```

`app/CMakeLists.txt`：

```cmake
add_executable(calculator_app main.cpp)
target_link_libraries(calculator_app PRIVATE calculator)
```

`add_subdirectory()` 会立即处理子目录中的 `CMakeLists.txt`。应先添加定义库的目录，再添加使用该库的目录。

### 2. 常用路径变量

| 变量 | 含义 |
| --- | --- |
| `CMAKE_SOURCE_DIR` | 最顶层项目的源码根目录 |
| `CMAKE_BINARY_DIR` | 最顶层项目的构建根目录 |
| `CMAKE_CURRENT_SOURCE_DIR` | 当前 `CMakeLists.txt` 所在源码目录 |
| `CMAKE_CURRENT_BINARY_DIR` | 当前配置文件对应的构建目录 |
| `PROJECT_SOURCE_DIR` | 最近一次 `project()` 对应的源码根目录 |
| `PROJECT_BINARY_DIR` | 最近一次 `project()` 对应的构建根目录 |

编写可被其他项目引入的子项目时，通常优先使用 `PROJECT_SOURCE_DIR` 或 `CMAKE_CURRENT_SOURCE_DIR`，避免错误依赖最外层工程路径。

---

## 八、构建类型与生成器

### 1. 单配置生成器

Ninja 和 Unix Makefiles 通常在配置时选择构建类型：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

常见构建类型：

| 类型 | 一般用途 |
| --- | --- |
| `Debug` | 带调试信息，通常关闭优化 |
| `Release` | 开启优化，通常关闭断言 |
| `RelWithDebInfo` | 优化并保留调试信息 |
| `MinSizeRel` | 优先减小产物体积 |

### 2. 多配置生成器

Visual Studio、Xcode 和 Ninja Multi-Config 通常在构建时选择配置：

```bash
cmake -S . -B build
cmake --build build --config Release
```

不要在 `CMakeLists.txt` 中强制覆盖 `CMAKE_BUILD_TYPE`，否则会妨碍调用者、IDE 和 CI 自由选择配置。

### 3. 指定生成器

```bash
cmake -S . -B build -G Ninja
```

Windows 上也可选择 Visual Studio 生成器：

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
```

同一个构建目录不能随意更换生成器或编译器。需要切换时，应使用新的构建目录或清理原构建目录。

---

## 九、选项、缓存变量与配置头文件

### 1. 定义可选功能

```cmake
option(ENABLE_LOGGING "Enable application logging" ON)
option(BUILD_EXAMPLES "Build example programs" OFF)

if(ENABLE_LOGGING)
    target_compile_definitions(app PRIVATE ENABLE_LOGGING)
endif()
```

命令行修改选项：

```bash
cmake -S . -B build -DENABLE_LOGGING=OFF -DBUILD_EXAMPLES=ON
```

### 2. 缓存变量

```cmake
set(APP_BACKEND "mock" CACHE STRING "Backend implementation")
set_property(CACHE APP_BACKEND PROPERTY STRINGS mock sqlite network)
```

缓存变量保存在构建目录的 `CMakeCache.txt` 中，可被命令行、GUI 和 IDE 修改。

查看常用缓存变量：

```bash
cmake -S . -B build -LAH
```

### 3. 生成配置头文件

`config.hpp.in`：

```cpp
#pragma once

#define APP_VERSION "@PROJECT_VERSION@"
#cmakedefine01 ENABLE_LOGGING
```

`CMakeLists.txt`：

```cmake
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/config.hpp
)

target_include_directories(app
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/generated
)
```

生成的头文件会类似：

```cpp
#define APP_VERSION "1.0.0"
#define ENABLE_LOGGING 1
```

---

## 十、查找与使用第三方库

### 1. find_package

以 Threads 为例：

```cmake
find_package(Threads REQUIRED)
target_link_libraries(app PRIVATE Threads::Threads)
```

以系统已安装的 fmt 为例：

```cmake
find_package(fmt CONFIG REQUIRED)
target_link_libraries(app PRIVATE fmt::fmt)
```

现代包通常提供带命名空间的导入目标，例如 `fmt::fmt`。目标中已经记录了头文件路径、编译定义和传递依赖，比直接使用若干 `*_INCLUDE_DIRS`、`*_LIBRARIES` 变量更可靠。

如果包安装在非标准位置：

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="/path/to/prefix"
```

### 2. FetchContent

可在配置阶段下载并引入依赖：

```cmake
include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        11.0.2
)

FetchContent_MakeAvailable(fmt)
target_link_libraries(app PRIVATE fmt::fmt)
```

实际项目应固定到明确版本或提交，避免依赖上游变化导致构建不可复现。离线开发、企业内网或大型依赖通常更适合包管理器、系统包或预先下载的源码。

### 3. add_subdirectory

若第三方库源码已包含在仓库中并提供自己的 `CMakeLists.txt`：

```cmake
add_subdirectory(external/somelib)
target_link_libraries(app PRIVATE somelib)
```

### 4. 包管理器

CMake 本身不是通用包管理器。常见组合有：

- CMake + vcpkg；
- CMake + Conan；
- CMake + 系统包管理器；
- CMake + FetchContent。

无论依赖来自哪里，理想结果都是获得一个可供 `target_link_libraries()` 使用的 CMake 目标。

---

## 十一、生成器表达式

生成器表达式形如 `$<...>`，在生成或构建阶段求值，可按配置、平台和语言设置属性。

按构建配置定义宏：

```cmake
target_compile_definitions(app PRIVATE
    $<$<CONFIG:Debug>:APP_DEBUG>
)
```

按编译器设置警告：

```cmake
target_compile_options(app PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall;-Wextra;-Wpedantic>
)
```

按语言设置选项：

```cmake
target_compile_options(mixed_target PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
)
```

生成器表达式很强大，但过度嵌套会降低可读性。复杂逻辑可考虑先用普通 `if()` 或封装函数整理。

---

## 十二、单元测试与 CTest

### 1. 最小测试配置

```cmake
include(CTest)  # 定义 BUILD_TESTING，并在启用时调用 enable_testing()

if(BUILD_TESTING)
    add_executable(calculator_test tests/calculator_test.cpp)
    target_link_libraries(calculator_test PRIVATE calculator)

    add_test(
        NAME calculator.unit
        COMMAND calculator_test
    )
endif()
```

构建和运行测试：

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

多配置生成器需指定配置：

```bash
ctest --test-dir build -C Debug --output-on-failure
```

### 2. 测试属性

```cmake
add_test(NAME network.integration COMMAND network_test)

set_tests_properties(network.integration PROPERTIES
    TIMEOUT 30
    LABELS "integration;network"
)
```

只运行带某标签的测试：

```bash
ctest --test-dir build -L unit
```

---

## 十三、安装项目

假设项目提供库 `calculator` 和程序 `calculator_app`：

```cmake
include(GNUInstallDirs)

install(
    TARGETS calculator calculator_app
    EXPORT CalculatorTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
```

执行安装：

```bash
cmake --install build --prefix ./install
```

多配置生成器：

```bash
cmake --install build --config Release --prefix ./install
```

导出目标，供其他 CMake 项目导入：

```cmake
install(
    EXPORT CalculatorTargets
    FILE CalculatorTargets.cmake
    NAMESPACE Calculator::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Calculator
)
```

要让别人能够完整使用 `find_package(Calculator)`，通常还需要生成并安装 `CalculatorConfig.cmake` 和版本文件。这属于库发布阶段的内容，应用项目一般不必配置。

---

## 十四、CMake Presets

`CMakePresets.json` 可以把常用配置保存为可复用预设，避免每个人记忆不同的长命令。

项目根目录创建 `CMakePresets.json`：

```json
{
  "version": 6,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 25,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "debug",
      "displayName": "Debug build",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/debug",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "BUILD_TESTING": "ON"
      }
    },
    {
      "name": "release",
      "displayName": "Release build",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/release",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "BUILD_TESTING": "OFF"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "debug",
      "configurePreset": "debug"
    },
    {
      "name": "release",
      "configurePreset": "release"
    }
  ],
  "testPresets": [
    {
      "name": "debug",
      "configurePreset": "debug",
      "output": {
        "outputOnFailure": true
      }
    }
  ]
}
```

使用预设：

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

通常将团队共享的 `CMakePresets.json` 提交到版本库，将开发者个人使用的 `CMakeUserPresets.json` 加入 `.gitignore`。

---

## 十五、交叉编译

为 STM32、ARM Linux 等其他平台构建时，需要通过工具链文件描述目标环境。

示例 `cmake/toolchains/arm-none-eabi.cmake`：

```cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# 裸机程序通常无法在配置机器上运行测试程序
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
```

配置时指定工具链文件：

```bash
cmake -S . -B build-arm \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/arm-none-eabi.cmake
cmake --build build-arm
```

工具链文件必须在第一次配置构建目录时指定。已有缓存的构建目录不应再切换工具链。

嵌入式项目往往还需要：

- 芯片相关编译参数，如 `-mcpu`、`-mthumb`；
- 启动文件和链接脚本；
- 生成 `.hex`、`.bin` 的构建后命令；
- 烧录与调试目标。

生成额外固件格式的示意写法：

```cmake
add_custom_command(
    TARGET firmware POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex
            $<TARGET_FILE:firmware>
            $<TARGET_FILE_DIR:firmware>/firmware.hex
    COMMENT "Generating firmware.hex"
)
```

这里使用 `$<TARGET_FILE:firmware>`，比手写输出文件路径更稳健。

---

## 十六、自定义命令与代码生成

### 1. 生成文件

```cmake
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp
    COMMAND version_generator
            ${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp
    DEPENDS version_generator version.txt
    COMMENT "Generating version.cpp"
    VERBATIM
)

target_sources(app PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp
)
```

`OUTPUT` 声明命令生成什么，`DEPENDS` 声明何时需要重新生成。`VERBATIM` 可让 CMake 更安全地处理命令参数中的转义。

### 2. 自定义目标

```cmake
add_custom_target(format
    COMMAND clang-format -i ${PROJECT_SOURCE_DIR}/src/main.cpp
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Formatting source files"
)
```

执行：

```bash
cmake --build build --target format
```

如果目标之间只存在构建顺序关系，可使用：

```cmake
add_dependencies(app generated_resources)
```

如果本质是链接依赖，应优先使用 `target_link_libraries()`，它会同时表达链接关系和构建顺序。

---

## 十七、调试 CMake

### 1. 查看变量

```cmake
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Binary dir: ${CMAKE_CURRENT_BINARY_DIR}")
```

### 2. 查看实际编译命令

```bash
cmake --build build --verbose
```

Ninja 也可使用：

```bash
ninja -C build -v
```

### 3. 导出 compile_commands.json

对 Ninja 和 Makefile 生成器，可在配置时启用：

```bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

生成的 `build/compile_commands.json` 可供 clangd、clang-tidy 和多种 IDE 使用。也可以写入预设：

```json
"CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
```

### 4. 跟踪配置过程

```bash
cmake -S . -B build --trace-expand
```

该输出通常很多，适合在普通 `message()` 无法定位问题时使用。

### 5. 常见缓存问题

如果移动了源码目录、切换了编译器或生成器，旧的 `CMakeCache.txt` 可能仍保存过时信息。最可靠的处理方式是使用新的构建目录重新配置。

---

## 十八、常见错误与改进方式

### 1. 全局 include_directories

不推荐：

```cmake
include_directories(include)
```

推荐：

```cmake
target_include_directories(app PRIVATE include)
```

原因是全局命令会隐式影响当前目录及其子目录，项目变大后很难判断路径来自哪里。

### 2. 全局 add_definitions 或 CXX_FLAGS

不推荐：

```cmake
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
add_definitions(-DENABLE_LOGGING)
```

推荐：

```cmake
target_compile_features(app PRIVATE cxx_std_17)
target_compile_options(app PRIVATE -Wall)
target_compile_definitions(app PRIVATE ENABLE_LOGGING)
```

这样能保证设置只作用于需要它的目标，也便于跨平台处理。

### 3. 使用相对路径导致定位混乱

在子目录配置中，明确使用：

```cmake
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
```

不要假设 CMake 总从项目根目录解析所有路径。

### 4. 手工填写库文件名

不推荐：

```cmake
target_link_libraries(app PRIVATE /usr/lib/libfoo.so)
```

推荐：

```cmake
find_package(foo CONFIG REQUIRED)
target_link_libraries(app PRIVATE foo::foo)
```

导入目标能够携带平台差异、依赖和头文件路径。

### 5. 修改 CMakeLists.txt 后仍使用旧结果

`cmake --build build` 通常会自动触发重新配置，但如果修改了编译器、工具链或生成器，应该重新创建构建目录。

### 6. Visual Studio 下找不到程序

Visual Studio 是多配置生成器，可执行文件通常在：

```text
build/Debug/
build/Release/
```

构建和测试时也要使用 `--config Debug` 或 `-C Debug`。

### 7. 链接阶段出现 undefined reference

常见原因：

- 只添加了头文件目录，没有链接实现库；
- 某个 `.cpp` 没有加入目标；
- 静态库依赖没有正确通过 `PUBLIC` 或 `INTERFACE` 传递；
- 函数声明与定义的签名不一致；
- C 与 C++ 代码混合时缺少 `extern "C"`。

先用 `cmake --build build --verbose` 检查实际链接命令。

---

## 十九、一份可直接复用的项目模板

项目结构：

```text
modern_cpp_project/
├── CMakeLists.txt
├── CMakePresets.json
├── app/
│   └── main.cpp
├── include/
│   └── myproject/greeting.hpp
├── src/
│   └── greeting.cpp
└── tests/
    └── greeting_test.cpp
```

根目录 `CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.20)

project(
    ModernCppProject
    VERSION 1.0.0
    DESCRIPTION "A small modern CMake example"
    LANGUAGES CXX
)

option(BUILD_TESTING "Build tests" ON)

add_library(project_warnings INTERFACE)
if(MSVC)
    target_compile_options(project_warnings INTERFACE /W4 /permissive-)
else()
    target_compile_options(project_warnings INTERFACE
        -Wall -Wextra -Wpedantic
    )
endif()

add_library(myproject
    src/greeting.cpp
)
add_library(MyProject::myproject ALIAS myproject)

target_compile_features(myproject PUBLIC cxx_std_17)
target_include_directories(myproject
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(myproject PRIVATE project_warnings)

add_executable(my_app app/main.cpp)
target_link_libraries(my_app
    PRIVATE
        MyProject::myproject
        project_warnings
)

if(BUILD_TESTING)
    enable_testing()

    add_executable(greeting_test tests/greeting_test.cpp)
    target_link_libraries(greeting_test
        PRIVATE
            MyProject::myproject
            project_warnings
    )

    add_test(NAME greeting.unit COMMAND greeting_test)
endif()
```

`include/myproject/greeting.hpp`：

```cpp
#pragma once

#include <string>

namespace myproject {
std::string greeting(const std::string& name);
}
```

`src/greeting.cpp`：

```cpp
#include "myproject/greeting.hpp"

namespace myproject {

std::string greeting(const std::string& name) {
    return "Hello, " + name + "!";
}

}  // namespace myproject
```

`app/main.cpp`：

```cpp
#include "myproject/greeting.hpp"

#include <iostream>

int main() {
    std::cout << myproject::greeting("CMake") << '\n';
    return 0;
}
```

`tests/greeting_test.cpp`：

```cpp
#include "myproject/greeting.hpp"

#include <cassert>

int main() {
    assert(myproject::greeting("CMake") == "Hello, CMake!");
    return 0;
}
```

构建并测试：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## 二十、常用命令速查

| 目的 | 命令 |
| --- | --- |
| 配置项目 | `cmake -S . -B build` |
| 指定 Ninja | `cmake -S . -B build -G Ninja` |
| Debug 配置 | `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug` |
| 构建全部目标 | `cmake --build build` |
| 并行构建 | `cmake --build build --parallel` |
| 构建指定目标 | `cmake --build build --target app` |
| 多配置构建 | `cmake --build build --config Release` |
| 显示详细命令 | `cmake --build build --verbose` |
| 运行测试 | `ctest --test-dir build --output-on-failure` |
| 安装 | `cmake --install build --prefix ./install` |
| 查看 CMake 版本 | `cmake --version` |
| 查看帮助 | `cmake --help` |
| 列出所有目标 | `cmake --build build --target help` |

常用 CMake 命令速查：

| CMake 命令 | 作用 |
| --- | --- |
| `cmake_minimum_required()` | 指定最低 CMake 版本并设置策略 |
| `project()` | 定义项目名称、版本和语言 |
| `add_executable()` | 创建可执行目标 |
| `add_library()` | 创建库目标 |
| `add_subdirectory()` | 引入子目录 |
| `target_sources()` | 给目标添加源文件 |
| `target_include_directories()` | 给目标添加头文件搜索路径 |
| `target_compile_features()` | 声明语言特性或 C++ 标准 |
| `target_compile_definitions()` | 添加编译宏 |
| `target_compile_options()` | 添加编译选项 |
| `target_link_libraries()` | 链接目标或库 |
| `find_package()` | 查找外部包 |
| `configure_file()` | 从模板生成配置文件 |
| `add_test()` | 注册测试 |
| `install()` | 定义安装规则 |

---

## 二十一、推荐实践清单

- 使用源码目录外构建：`cmake -S . -B build`；
- 围绕目标使用 `target_*` 命令，尽量减少全局状态；
- 正确区分 `PRIVATE`、`PUBLIC` 和 `INTERFACE`；
- 用 `target_compile_features()` 表达 C++ 标准要求；
- 链接 CMake 目标，而不是手写库文件绝对路径；
- 不在项目配置里强制固定编译器和构建类型；
- 对团队常用配置使用 `CMakePresets.json`；
- 使用 `CTest` 统一运行测试；
- 第三方依赖固定版本，保证构建可复现；
- 把 `build/`、`CMakeUserPresets.json` 和本地安装目录加入 `.gitignore`。

掌握 CMake 的关键不是记住所有命令，而是建立“目标及其使用要求”的思维：**谁需要源码、谁需要头文件、谁需要编译宏、谁依赖谁，以及这些要求是否应该继续传递。**
