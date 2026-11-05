# Makefile 与 GNU Make

## Makefile 的作用与规则结构

Makefile 用来描述“要生成什么、依赖什么、怎样生成”。GNU Make 是读取这些规则、判断执行顺序的构建工具，通常通过 `make` 命令调用；Makefile 本身不是编译器，真正的编译工作由规则中的 GCC、G++ 等命令完成。其他 Make 实现的语法可能不同，本文使用 GNU Make。

在项目根目录保存名为 `Makefile` 的构建文件后，运行 `make` 即可读取它。规则的基本结构是：

```makefile
目标: 依赖文件列表
	命令
```

| 部分 | 英文名称 | 含义 |
| --- | --- | --- |
| 目标 | Target | 要生成的文件，或要执行的操作名称 |
| 依赖 | Prerequisites | 构建目标前需要准备好的文件或其他目标 |
| 命令 | Recipe | 更新目标时交给 Shell 执行的命令 |

```makefile
app: main.o math.o
	gcc main.o math.o -o app
```

这是一条**显式规则（Explicit Rule）**：明确列出 `app` 依赖 `main.o` 和 `math.o`，两者准备好后才链接。依赖列表表达构建关系；如果使用并行构建，彼此独立的依赖可以同时生成，列表顺序不表示必须依次执行。

### Tab、注释、续行与路径引用

- 默认情况下，Recipe 的每条命令行必须以一个真正的 **Tab** 开头，不能用几个空格代替，否则常见错误是 `missing separator`。变量定义和规则头通常顶格写。本文代码块中的命令行使用 Tab。
- `#` 在 Makefile 普通语法中开始注释；Recipe 中则由 Shell 解释。变量行尾注释前的空格可能成为变量值的一部分，路径变量的说明适合单独写一行。
- `\` 放在行尾表示续行，后面不要再加空格。在普通变量定义中通常合并为空格；Recipe 的续行会交给 Shell，行为与普通变量行不同。
- `"路径"` 是 Shell 的路径引用方式，用于把展开后的单个路径作为一个参数。它不能解决 Make 自身以空白分隔文件列表的问题；下面示例假设项目路径和文件名不含空格。
- Make 通常为每条 Recipe 命令行启动独立 Shell；某一行的 `cd` 不会自动影响下一行，需要时在同一行写 `cd 目录 && 命令`。

```makefile
SOURCES := src/main.c \
           src/math.c
```

### 默认目标与伪目标

```makefile
.DEFAULT_GOAL := all
.PHONY: all run clean

all: app

run: app
	./app

clean:
	rm -f main.o math.o app
```

`.DEFAULT_GOAL := all` 明确指定不带目标执行 `make` 时构建 `all`。未指定时，通常选择第一条普通规则的第一个目标，因此将默认目标写清楚也能避免包含依赖文件后默认目标意外改变。

`all`、`run`、`clean` 都是约定名称，没有自动附带的行为。这里 `all` 汇总构建任务；`run` 先确保 `app` 最新再运行；`clean` 删除产物。

`.PHONY` 声明这些名称是伪目标，不用同名文件的修改时间判断是否执行。否则目录中若已有名为 `clean` 的文件，`make clean` 可能不运行清理命令。`all` 本身没有 Recipe，构建动作来自它的依赖；真实产物 `app`、`.o` 不应声明为伪目标，以免失去增量构建效果。

## 变量与赋值方式

变量引用写作 `$(VAR)`，例如 `$(CC)` 展开为编译器命令。`BUILD_DIR`、`TARGET`、`SOURCES` 等是用户自定义变量，名称可以自行约定；GNU Make 不会自动理解它们的用途，必须在规则中引用才生效。

| 写法 | 名称 | 展开时机与行为 |
| --- | --- | --- |
| `VAR = 内容` | 递归展开赋值 | 保存右侧文本，引用 `VAR` 时再展开其中的变量 |
| `VAR := 内容` | 立即展开赋值 | 定义时就展开右侧内容，保存结果 |
| `VAR += 内容` | 追加赋值 | 以空格分隔追加；已有 `:=` 变量立即展开追加内容，已有 `=` 变量延迟展开；未定义时按 `=` 处理 |
| `VAR ?= 内容` | 条件赋值 | 仅在变量尚未定义时赋值；已经定义为空字符串也算已定义 |

```makefile
NAME = first
LATE = $(NAME)
EARLY := $(NAME)
NAME = second
# $(LATE) 为 second，$(EARLY) 为 first

CFLAGS := -O0 -g
CFLAGS += -Wall -Wextra
BUILD_DIR ?= build/make
```

### GNU Make 内置变量与命令行覆盖

GNU Make 预定义了一些变量，并在内置隐式规则中按惯例使用。手写 Recipe 时也应该引用这些变量，便于统一配置；变量不会自动加到任意手写命令中。

| 变量 | 用途 | 常见值或内容 |
| --- | --- | --- |
| `CC` | C 编译器 | GNU Make 通常内置为 `cc`，示例显式设为 `gcc` |
| `CXX` | C++ 编译器 | 通常为 `g++` |
| `CPPFLAGS` | 预处理选项，C/C++ 都可使用 | `-Isrc`、`-DDEBUG`；名字中的 CPP 指预处理器 |
| `CFLAGS` | C 编译选项 | `-std=gnu11 -O0 -g -Wall -Wextra` |
| `CXXFLAGS` | C++ 编译选项 | `-std=c++17 -g -Wall -Wextra` |
| `LDFLAGS` | 链接器选项 | 库搜索路径 `-Llib` 等 |
| `LDLIBS` | 链接库选项 | `-lm` 等，通常放在目标文件之后 |
| `RM` | 删除文件的命令 | GNU Make 默认是 `rm -f` |

`CC`、`CXX`、`RM` 有常见内置默认值；其他选项变量默认通常为空。由于 `CC` 已经内置为 `cc`，`CC ?= gcc` 通常不会把它改成 `gcc`。想明确使用 GCC，可以写 `CC = gcc`。

```bash
make CC=clang
make CFLAGS="-std=gnu11 -O2 -Wall -Wextra"
make BUILD_DIR=build/make-release
```

命令行赋值通常覆盖 Makefile 中的普通赋值，包括普通 `+=`；环境变量通常可被 Makefile 普通赋值覆盖，除非使用 `make -e`。Makefile 的 `override` 指令是命令行覆盖规则的显式例外。上面的第二条命令替换整个 `CFLAGS`，需要的标准和警告选项应一起给出。

## GCC：编译与链接

```text
main.c → 预处理 → 编译为汇编 → 汇编为 main.o ─┐
                                              ├→ 链接 → app
math.c → 预处理 → 编译为汇编 → 汇编为 math.o ─┘
```

日常说“编译为 `.o`”通常合指预处理、编译、汇编三个阶段。`-c` 让 GCC 在生成目标文件后停止，不执行链接；`.o` 中可能存在尚未解析的外部符号，通常不能单独运行。链接把多个目标文件及需要的库组合成可执行文件。

```bash
gcc -std=gnu11 -Isrc -O0 -g -MMD -MP -c src/main.c -o build/make/src/main.o
gcc -std=gnu11 -Isrc -O0 -g -MMD -MP -c src/math.c -o build/make/src/math.o
gcc build/make/src/main.o build/make/src/math.o -o build/make/app -lm
```

执行前需创建输出目录。上面的流程就是 `.c → .o → 可执行文件`，每个源文件可独立编译，最后统一链接。

| 参数 | 作用 |
| --- | --- |
| `-std=gnu11` | 使用带 GNU 扩展的 C11 方言；它是 C 选项，不能作为 C++ 标准选项 |
| `-Isrc` | 将 `src` 加入头文件搜索路径；相对路径从编译命令的工作目录计算 |
| `-O0` | 关闭常规优化，方便观察源码与执行步骤的对应关系 |
| `-g` | 生成调试信息，供 GDB 等调试器使用；它本身不表示关闭优化 |
| `-Wall`、`-Wextra` | 启用常见警告和额外警告，并不表示所有警告 |
| `-c` | 编译为目标文件，不链接 |
| `-o 文件` | 指定输出文件，可用于编译和链接阶段 |
| `-MMD` | 编译时生成 `.d` 文件，记录源文件及非系统头文件依赖 |
| `-MP` | 为依赖中的头文件生成无 Recipe 的空规则，缓解旧 `.d` 引用已删除头文件时的 Make 错误 |
| `-lm` | 链接数学库 `libm`，某些平台需要显式指定 |

链接时通常使用 `$(CC) $(LDFLAGS) 目标文件列表 -o 输出 $(LDLIBS)`。库参数的位置有意义，尤其是静态库：通常先放引用符号的目标文件，再放提供符号的库，例如 `main.o -lm`。库之间也可能存在顺序依赖，不能认为所有链接参数都可以任意交换位置。

## 综合示例：源码与构建目录分离

下面以 C 项目为例，使用 GNU Make、GCC 和 POSIX 风格 Shell 命令。Linux、WSL 或配置了这些工具的 MSYS2 等环境可使用；原生 Windows `cmd` / PowerShell 不直接兼容 `mkdir -p`、`rm -f` 的全部语法。

```text
项目根目录/
├── Makefile
├── CMakeLists.txt       # 若同时使用 CMake
├── src/
│   ├── main.c
│   ├── math.c
│   └── math.h
└── build/
    ├── make/           # 手写 Makefile 的产物
    └── cmake/          # CMake 生成文件与产物
```

将下面内容保存为项目根目录中的 `Makefile`：

```makefile
.DEFAULT_GOAL := all

CC = gcc
CPPFLAGS := -Isrc
CFLAGS := -std=gnu11 -O0 -g -Wall -Wextra
LDFLAGS :=
LDLIBS := -lm

BUILD_DIR := build/make
TARGET := $(BUILD_DIR)/app
SOURCES := src/main.c \
           src/math.c
OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJECTS:.o=.d)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p "$(dir $@)"
	$(CC) $(LDFLAGS) $^ -o "$@" $(LDLIBS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p "$(dir $@)"
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c "$<" -o "$@"

run: $(TARGET)
	"./$(TARGET)"

clean:
	$(RM) $(OBJECTS) $(DEPS) "$(TARGET)"

-include $(DEPS)
```

### 自定义变量与路径替换

| 变量 | 在示例中的含义 |
| --- | --- |
| `BUILD_DIR` | 构建目录，默认是 `build/make` |
| `TARGET` | 最终目标可执行文件路径，默认是 `build/make/app` |
| `SOURCES` | 需要编译的源文件列表，新源文件应加入这里 |
| `OBJECTS` | 根据源文件列表推导出的目标文件列表 |
| `DEPS` | 与目标文件对应的 `.d` 依赖文件列表 |

`$(SOURCES:%.c=$(BUILD_DIR)/%.o)` 是**模式替换**：对每个匹配 `%.c` 的单词，保留 `%` 匹配的部分再生成新路径。

```text
src/main.c → build/make/src/main.o
src/math.c → build/make/src/math.o
```

保留源目录层级，可以避免不同目录中同名源文件的 `.o` 冲突。`$(OBJECTS:.o=.d)` 是**后缀替换**，将每个 `.o` 后缀改成 `.d`，例如 `build/make/src/main.d`。

`$(dir 路径列表)` 提取每个文件的目录部分，并保留末尾 `/`。例如 `$(dir build/make/src/main.o)` 得到 `build/make/src/`；没有目录部分时得到 `./`。

### 模式规则与自动变量

`$(BUILD_DIR)/%.o: %.c` 是**模式规则（Pattern Rule）**。在模式规则中，`%` 匹配主干部分，并用同一主干构造依赖；它不是 Shell 的 `*` 通配符。目标为 `build/make/src/main.o` 时，主干是 `src/main`，第一个依赖是 `src/main.c`。

自动变量由 Make 在每次执行规则的 Recipe 时根据当前目标提供，不能把它们当作普通全局变量使用：

| 自动变量 | 含义 | 上面编译规则中的例子 |
| --- | --- | --- |
| `$@` | 当前规则的目标文件 | `build/make/src/main.o` |
| `$<` | 第一个依赖文件 | `src/main.c` |
| `$^` | 所有普通依赖文件，去重 | 还可能包含从 `.d` 读入的头文件 |
| `$?` | 比目标更新的普通依赖文件；目标不存在时为所有普通依赖 | 此次触发更新的源文件或头文件 |
| `$*` | 模式规则匹配的主干部分 | `src/main` |

因此编译 Recipe 用 `$<` 取得源文件，而不是用 `$^` 把头文件也作为独立输入交给 GCC；链接规则用 `$^` 取得所有 `.o`。`$?` 常用于只处理有变化的输入，但重新链接可执行文件通常仍需要完整目标文件列表。`$*` 主要用于模式规则，不建议在普通显式规则中依赖它的历史后缀推导行为。

### 创建目录与清理文件

`mkdir -p "$(dir $@)"` 递归创建目标文件的父目录；目录已存在时不报错，因此独立编译任务并行执行时也可以各自准备目录。

`rm -f` 删除文件，文件不存在时不报错；`-f` 不表示递归删除目录。`$(RM)` 默认展开为 `rm -f`，因此示例的 `clean` 删除 `.o`、`.d` 与可执行文件，保留空目录和 `build/cmake`。它只清理当前变量列表中的产物；删除或改名源文件后遗留的旧产物可能需要另行清理。

## 增量构建与自动头文件依赖

Make 会先更新依赖，再比较文件修改时间：目标不存在，或者某个普通依赖比目标新时，通常需要执行目标的 Recipe；目标已存在且没有较新的依赖时，跳过更新。这就是**增量编译（Incremental Build）**的基础，Make 默认不比较文件内容。

- 第一次构建：生成全部 `.o`，再链接可执行文件。
- 修改 `src/main.c`：只重编译对应的 `.o`，然后重新链接。
- 修改多个源文件共同包含的头文件：重编译依赖该头文件的各个 `.o`，然后重新链接。
- 没有修改：不重新编译和链接；`make run` 仍执行运行命令，因为 `run` 是伪目标。

仅写 `%.o: %.c` 无法告诉 Make 源文件包含了哪些头文件。示例用 GCC 的 `-MMD -MP` 在编译时生成依赖文件，使用 `-o build/make/src/main.o` 时，通常得到同目录的 `main.d`，内容类似：

```makefile
build/make/src/main.o: src/main.c src/math.h
src/math.h:
```

`-include $(DEPS)` 在读取 Makefile 时将这些规则合并到依赖图中，前面的 `-` 允许首次构建时 `.d` 尚不存在。它不是“忽略所有语法错误”；损坏的依赖文件仍可能报错。`-MP` 生成的头文件空规则也不同于显式 `.PHONY` 声明，不会让所有目标每次都重编译；若源码仍包含已经删除的头文件，编译器依然会报错。

Make 通常不会仅因为修改了 `CFLAGS` 或编译命令就自动重编译。切换标准、编译器或优化选项后，应清理重建，或者为不同配置使用不同构建目录。系统时钟和文件时间戳异常也可能干扰增量判断。

## 常用命令与构建配置

在示例项目的根目录执行：

| 命令 | 行为 |
| --- | --- |
| `make` | 执行 `.DEFAULT_GOAL` 指定的 `all` |
| `make all` | 构建程序，复用未过期的产物 |
| `make run` | 先构建或更新程序，再运行 |
| `make clean` | 清理当前配置的目标文件、依赖文件和可执行文件 |
| `make -j` | 并行执行独立任务；不带数字时不限制任务数 |
| `make -j4` | 最多同时执行 4 个任务 |

并行是否正确取决于依赖关系是否完整。不要用 `make -j clean all` 表达“清理后构建”，两者可能同时执行；应依次运行 `make clean` 和 `make all`。

调试构建通常用 `-O0 -g`，更容易单步调试；优化构建可用 `-O2`，需要时仍可保留 `-g`，但优化可能改变变量和执行顺序的可观察性。可以使用两个独立目录：

```bash
make BUILD_DIR=build/make-debug CFLAGS="-std=gnu11 -O0 -g -Wall -Wextra"
make BUILD_DIR=build/make-release CFLAGS="-std=gnu11 -O2 -Wall -Wextra"
```

**增量构建**复用当前配置仍有效的产物；**完全重新构建**先清除该配置的产物，再重新编译和链接。例如依次执行 `make clean`、`make all`。使用自定义 `BUILD_DIR` 时，清理和构建必须传入同一目录；同一目录内切换编译选项也需要清理。

## C++ 项目的写法

原基础笔记中的 C++ 示例使用 `main.cpp` 与 `Surcharge.cpp` 生成 `test`。规则结构不变，将编译器换为 `CXX`、编译选项换为 `CXXFLAGS`，源文件后缀换为 `.cpp` 即可：

```makefile
.DEFAULT_GOAL := all
CXX = g++
CPPFLAGS :=
CXXFLAGS := -std=c++17 -Wall -Wextra -g
LDFLAGS :=
LDLIBS :=
TARGET := test
SRCS := main.cpp Surcharge.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o "$@" $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c "$<" -o "$@"

-include $(DEPS)

clean:
	$(RM) $(OBJS) $(DEPS) "$(TARGET)"
```

这里 `SRCS` / `OBJS` 与前面的 `SOURCES` / `OBJECTS` 都是自定义名称。`main.cpp → main.o`、`Surcharge.cpp → Surcharge.o`，依赖文件为 `main.d`、`Surcharge.d`，链接得到 `./test`。增加源文件时只需扩展 `SRCS`。这个简短示例在源码旁生成产物，项目变大后可采用前面的构建目录分离方式。

编译和链接 C++ 通常都用 `g++` 驱动，以便链接时自动加入 C++ 标准库；`-std=gnu11` 应替换成 `-std=c++17` 等 C++ 标准选项。

## Makefile 与 CMakeLists.txt

| 对比 | 手写 Makefile | CMakeLists.txt |
| --- | --- | --- |
| 读取工具 | GNU Make 等 Make 实现 | CMake |
| 描述内容 | 具体目标、文件依赖和命令规则 | 项目目标、源文件、库与构建要求 |
| 后续执行 | `make` 按规则调用编译器等工具 | CMake 生成 Makefile、Ninja 或 IDE 工程，再由对应工具构建 |
| 产物目录 | 本文约定 `build/make` | 本文约定 `build/cmake` |

CMake 也可能生成名为 `Makefile` 的文件，但它属于生成的构建系统，应通过 `CMakeLists.txt` 修改项目要求。手写 Makefile 与 CMake 可以使用同一份源码，构建文件和产物则分别存放：

```bash
# 手写 Makefile 的构建
make

# CMake 的配置与构建；要求项目已有 CMakeLists.txt
cmake -S . -B build/cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build build/cmake
```

`CMAKE_BUILD_TYPE` 主要用于 Makefiles、Ninja 等单配置生成器；Visual Studio 等多配置生成器应在构建时使用 `--config Debug`。保持 `build/make` 与 `build/cmake` 隔离，可以避免 `.o`、依赖文件和配置缓存混用，也便于分别清理。更多 CMake 内容见 [CMake 与 CMakeLists.txt 教程](cmake教程.md)。

## 官方参考

- [GNU Make 手册](https://www.gnu.org/software/make/manual/make.html)：规则、变量、自动变量、伪目标与并行构建。
- [GCC 编译阶段与输出选项](https://gcc.gnu.org/onlinedocs/gcc/Overall-Options.html)：编译流程、`-c` 与 `-o`。
- [GCC 预处理与依赖生成选项](https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html)：`-MMD`、`-MP` 与依赖文件。
- [GCC 链接选项](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)：`-l` 与库顺序。
- [CMake 命令行手册](https://cmake.org/cmake/help/latest/manual/cmake.1.html)：源码目录、构建目录及 `--build`。
