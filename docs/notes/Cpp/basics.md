# C++ 基础知识点总结
## 头文件
| 头文件               | 主要内容    | 常用函数 / 对象                                    | 典型用途  |
| ----------------- | ------- | -------------------------------------------- | ----- |
| `<iostream>`      | 标准输入输出  | `cin` `cout` `cerr` `endl`                   | 基础 IO |
| `<iomanip>`       | 输出格式控制  | `setprecision` `setw` `setfill` `fixed`      | 浮点格式  |
| `<fstream>`       | 文件读写    | `ifstream` `ofstream`                        | 文件操作  |
| `<vector>`        | 动态数组    | `push_back` `size` `begin`                   | 刷题最常用 |
| `<string>`        | 字符串     | `substr` `size` `find`                       | 文本处理  |
| `<deque>`         | 双端队列    | `push_front` `push_back`                     | 单调队列  |
| `<list>`          | 双向链表    | `push_back` `erase`                          | 频繁插删  |
| `<queue>`         | 队列      | `push` `pop` `front`                         | BFS   |
| `<stack>`         | 栈       | `push` `pop` `top`                           | 括号匹配  |
| `<set>`           | 有序集合    | `insert` `find`                              | 去重+排序 |
| `<unordered_set>` | 无序集合    | `insert` `count`                             | 哈希查找  |
| `<map>`           | 有序键值对   | `insert` `find`                              | 排序映射  |
| `<unordered_map>` | 无序键值对   | `[]` `find`                                  | 高频统计  |
| `<algorithm>`     | 算法库     | `sort` `reverse` `max` `min` `binary_search` | 排序+查找 |
| `<numeric>`       | 数值算法    | `accumulate` `iota`                          | 求和    |
| `<functional>`    | 函数对象    | `greater` `less`                             | 自定义排序 |
| `<cmath>`         | 数学函数    | `sqrt` `pow` `abs` `sin`                     | 数学运算  |
| `<cstdlib>`       | C标准库    | `rand` `abs`                                 | 随机数   |
| `<cstring>`       | C字符串/内存 | `memset` `memcpy` `strlen`                   | 嵌入式常用 |
| `<cstdint>`       | 固定宽度整数  | `uint32_t` `int64_t`                         | 嵌入式   |
| `<utility>`       | 工具函数    | `pair` `make_pair` `move` `forward`          | 对组、移动语义 |
| `<tuple>`         | 多元组     | `tuple`                                      | 多返回值  |
| `<bitset>`        | 位集合     | `bitset<32>`                                 | 位运算题  |
| `<memory>`        | 智能指针    | `shared_ptr` `unique_ptr`                    | 现代C++ |
| `<thread>`        | 多线程     | `thread`                                     | 并发    |
| `<mutex>`         | 互斥锁     | `mutex` `lock_guard`                         | 线程安全  |
| `<chrono>`        | 时间      | `steady_clock`                               | 计时    |
| `<filesystem>`    | 文件系统    | `path` `exists`                              | 路径管理  |
| `<cstdio>`        | C输入输出   | `printf` `scanf`                             | 串口调试  |
| `<bits/stdc++.h>` | 全部STL   | —                                            | 竞赛专用  |

## 类型转换
| 转换方式               | 头文件          | 作用                     | 示例                                |
| ------------------ | ------------ | ---------------------- | --------------------------------- |
| `stoi`             | `<string>`   | string → int           | `int x = stoi("123");`            |
| `stol`             | `<string>`   | string → long          | `long x = stol("123");`           |
| `stoll`            | `<string>`   | string → long long     | `long long x = stoll("123");`     |
| `stof`             | `<string>`   | string → float         | `float x = stof("1.23");`         |
| `stod`             | `<string>`   | string → double        | `double x = stod("1.23");`        |
| `to_string`        | `<string>`   | 数字 → string            | `string s = to_string(123);`      |
| `atoi`             | `<cstdlib>`  | C字符串 → int             | `int x = atoi("123");`            |
| `atol`             | `<cstdlib>`  | C字符串 → long            |                                   |
| `atoll`            | `<cstdlib>`  | C字符串 → long long       |                                   |
| `strtol`           | `<cstdlib>`  | C字符串 → long（更安全）       |                                   |
| `strtod`           | `<cstdlib>`  | C字符串 → double          |                                   |
| `from_chars`       | `<charconv>` | string → 数字（高性能）       |                                   |
| `to_chars`         | `<charconv>` | 数字 → char数组            |                                   |
| `static_cast`      | 无需头文件        | 编译期安全转换                | `int x = static_cast<int>(3.14);` |
| `dynamic_cast`     | `<typeinfo>` | 多态类型安全转换               |                                   |
| `const_cast`       | 无需           | 去除 const               |                                   |
| `reinterpret_cast` | 无需           | 强制类型转换（危险）             |                                   |
| C风格 `(int)x`       | 无需           | 强制转换                   |                                   |
| `abs`              | `<cmath>`    | 数值类型转换为正               |                                   |
| `floor`            | `<cmath>`    | 向下取整                   |                                   |
| `ceil`             | `<cmath>`    | 向上取整                   |                                   |
| `round`            | `<cmath>`    | 四舍五入                   |                                   |
| `lround`           | `<cmath>`    | 四舍五入为 long             |                                   |
| `toupper`          | `<cctype>`   | char → 大写              |                                   |
| `tolower`          | `<cctype>`   | char → 小写              |                                   |
| `bitset::to_ulong` | `<bitset>`   | bitset → unsigned long |                                   |
| `to_integer`       | `<cstddef>`  | byte → 整数              |                                   |

### 类型转换的选择与注意事项

#### 1. 隐式转换

编译器会自动进行部分转换，但从范围大的类型转向范围小的类型时可能丢失数据：

```cpp
double pi = 3.14;
int value = pi;          // 隐式转换，结果为 3，小数部分被截断
int safe{42};            // 列表初始化会阻止明显的窄化转换
// int error{3.14};      // 编译错误：double 窄化为 int
```

#### 2. 四种 C++ 显式转换

```cpp
int n = static_cast<int>(3.14);  // 常规数值转换，最常用

Base* base = getObject();
Derived* child = dynamic_cast<Derived*>(base); // 多态向下转换，失败返回 nullptr

const int* p = &n;
int* writable = const_cast<int*>(p);           // 改变 const 属性，需谨慎

std::uintptr_t address = reinterpret_cast<std::uintptr_t>(p); // 底层位模式转换
```

- 普通常规转换优先使用 `static_cast`；
- `dynamic_cast` 用于含虚函数的多态类型，指针转换失败得到 `nullptr`；
- `const_cast` 只改变 `const/volatile` 属性，修改原本真正为常量的对象会产生未定义行为；
- `reinterpret_cast` 主要用于底层编程，应尽量少用；
- C++ 中不推荐 C 风格强制转换，因为它不容易看出实际采用了哪种转换。

#### 3. 字符串转数字

```cpp
try {
    int value = std::stoi("123");
} catch (const std::invalid_argument&) {
    // 字符串不是有效数字
} catch (const std::out_of_range&) {
    // 数值超出 int 范围
}
```

`std::stoi` 使用方便但会抛异常；对性能敏感或不希望使用异常时，可选择 `<charconv>` 中的 `std::from_chars`，并检查其返回的错误码。

> `floor`、`ceil` 和 `round` 是取整运算，但返回值仍是浮点类型；需要整数时还要进行范围检查并显式转换。

## 异常处理：`try`、`throw` 与 `catch`

C++ 异常用于处理函数无法在本地正常解决的错误：`throw` 抛出异常，`try` 包围可能失败的代码，`catch` 根据异常类型进行处理。

```cpp
#include <iostream>
#include <stdexcept>

double divide(double a, double b) {
    if (b == 0.0) {
        throw std::invalid_argument("除数不能为 0");
    }
    return a / b;
}

int main() {
    try {
        std::cout << divide(10.0, 0.0) << '\n';
    } catch (const std::invalid_argument& e) {
        // 优先捕获更具体的异常类型
        std::cerr << "参数错误：" << e.what() << '\n';
    } catch (const std::exception& e) {
        // 标准异常的通用基类，what() 返回错误说明
        std::cerr << "程序异常：" << e.what() << '\n';
    } catch (...) {
        // 捕获其他所有类型的异常，但无法直接取得异常信息
        std::cerr << "未知异常\n";
    }
}
```

异常抛出后，程序会沿函数调用栈向外寻找第一个匹配的 `catch`。查找过程中，已经创建的局部对象会自动析构，这称为**栈展开**；因此资源应交给 `std::string`、容器和智能指针等 RAII 对象管理。

常用标准异常包括 `std::invalid_argument`（参数无效）、`std::out_of_range`（超出范围）和 `std::runtime_error`（运行时错误）。通常按值 `throw`、按 `const` 引用捕获，避免复制和对象切片：

```cpp
throw std::runtime_error("读取配置失败");

try {
    loadConfig();
} catch (const std::exception& e) {
    logError(e.what());
    throw;  // 不写对象，原样重新抛出当前异常
}
```

不要使用 `throw e;` 代替重新抛出的 `throw;`，前者可能复制异常并丢失派生类型信息。具体类型的 `catch` 应放在基类之前；如果异常一直没有被捕获，程序最终会调用 `std::terminate()`。标记为 `noexcept` 的函数承诺异常不会逃出函数，一旦违背同样会终止程序，所以只应在确实不会向外抛出异常时使用。

## 字符串

```cpp
string s1;                 // 创建一个空字符串
string s2 = "hello";       // 创建一个字符串并初始化
s1.reserve(s2.size());     // 预分配 s2.size() 个字符的空间
s1.push_back('h');         // 在字符串末尾添加一个字符
s1 += "ello";              // 在字符串末尾添加一个字符串
cout << sizeof(s) << endl; // 输出字符串对象占用的内存大小

string s = "ABCDEFG";
string first3 = s.substr(0, 3);              // "ABC"
string last3 = s.substr(s.size() - 3, 3);    // "EFG"
s.erase(2, 3);                               // 从下标 2 开始删 3 个，结果 "ABFG"
s.erase(n);                                  // 保留前 n 个字符，删除剩余的
s.empty();                                   // 判断字符串是否为空
s.clear();                                   // 清空字符串
reverse(s.begin(), s.end());                 // 反转字符
reverse(words.begin(), words.end());         // 反转字符串顺序，words 是 vector<string>

auto it = s.begin();                         // 获取字符串迭代器，指向第一个字符
std::string::iterator it2;                   // 等价写法

s.insert(it, 'X');                           // 在字符串开头插入字符 'X'
s.insert(pos, count, 'c');                   // 在 pos 位置插入 count 个字符 c
s.find('o', pos);                            // 从 pos 开始查找字符 'o'
```

补充说明：

- `for (char c : s)` 可以逐个遍历字符串里的字符。
- `insert(pos, count, 'c')` 这种重载是 `string` 自带的，其他容器不一定有。
- `find()` 没找到时会返回 `string::npos`。

## 输入输出控制

`<iostream>` 是头文件；`std::istream`、`std::ostream` 和 `std::iostream` 是流类型，分别表示输入流、输出流和可同时输入输出的流。`std::cin`、`std::cout` 则是标准库预先创建的对象：`cin` 是 `istream` 对象，`cout` 是 `ostream` 对象。

```cpp
std::istream& input = std::cin;    // 类型是 istream，对象是 cin
std::ostream& output = std::cout;  // 类型是 ostream，对象是 cout
```

| 控制符 | 作用 |
| ---- | ---- |
| `fixed` | 使用固定小数格式 |
| `setprecision(n)` | 保留 `n` 位小数 |

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double x = 3.1415926;

    cout << fixed << setprecision(2) << x << endl;
}
```

补充：

- 如果没有 `fixed`

  ```cpp
  cout << setprecision(2) << x;
  ```

  表示保留的是 `2` 位有效数字，而不是 `2` 位小数。

- 科学计数法输出：

  ```cpp
  cout << scientific << setprecision(3) << x;
  ```

- 控制宽度：

  ```cpp
  cout << setw(10) << x << endl;
  ```

  表示字段宽度为 `10`。

- 设置填充字符：

  ```cpp
  cout << setfill('*') << setw(10) << x << endl;
  ```

## 引用

引用可以理解成“别名”。

- 必须在定义时初始化
- 一旦绑定，不能再绑定到其他变量
- 引用不能为空

```cpp
int a = 1, b = 2;
int& r = a;
r = b;   // ❌ 不是改引用对象
// 等价于
a = b;
```

### 引用的基本用法

引用不是一个独立的对象，而是已有对象的另一个名字。通过引用读写变量，实际操作的仍然是被引用的对象。

```cpp
int n = 10;
int& ref = n;

cout << ref << endl;  // 读取 n
ref = 20;             // 修改 n
cout << n << endl;    // 20
```

引用必须绑定到一个对象，不能直接绑定到 `nullptr`，也不能像指针一样通过赋值改变绑定关系。可以通过 `&` 获取对象地址，验证引用和原对象指向同一位置：

```cpp
cout << &n << endl;
cout << &ref << endl; // 与 &n 相同
```

### 常量引用

在引用前加 `const`，表示不能通过该引用修改对象：
被引用对象的生命周期必须比引用长。
```cpp
int n = 10;
const int& ref = n;
// ref = 20;           // ❌ 不能通过 const 引用修改 n
n = 20;                // ✅ 仍然可以直接修改 n
```

常量引用可以绑定到常量和临时对象，因此常用于函数参数，既避免复制，又保证函数不会修改实参：

```cpp
void print(const string& text) {
    cout << text << endl;
}

print("hello"); // 可以绑定到临时的 string 对象
```

### 引用作为函数参数

使用普通引用作为参数，可以让函数直接修改调用者提供的变量：

```cpp
void swapValue(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int x = 1, y = 2;
swapValue(x, y); // x == 2，y == 1
```

如果函数不需要修改参数，优先使用 `const` 引用；如果参数很小且复制成本低，也可以直接按值传递。

### 引用作为返回值

函数可以返回引用，以便直接操作原对象：

```cpp
int& maxValue(int& a, int& b) {
    return a > b ? a : b;
}

int x = 1, y = 2;
maxValue(x, y) = 10; // y 被修改为 10
```

返回的非 `const` 引用是一个左值（lvalue），因此可以放在赋值号左边，直接修改它所引用的原对象；如果返回类型是 `const T&`，则不能通过该引用赋值。

返回引用时，不能返回局部变量的引用，因为局部变量在函数结束后已经销毁，会产生悬空引用：

```cpp
int& wrong() {
    int local = 1;
    return local; // ❌ 返回局部变量的引用
}
```

### 引用与指针的区别

| 特性 | 引用 | 指针 |
| --- | --- | --- |
| 是否必须初始化 | 是 | 否 |
| 是否可以为空 | 否 | 可以为 `nullptr` |
| 是否可以重新绑定 | 不可以 | 可以修改指向 |
| 使用对象 | 直接使用 | 通常需要解引用 `*` |
| 是否支持算术运算 | 不支持 | 支持指针运算 |

引用适合表示“必定存在的对象别名”；需要表示空值、改变指向或进行指针运算时，应使用指针。

### 左值、右值与两类引用

值类别描述的是**表达式如何表示对象或值**，不是另一套数据类型。左值（lvalue）表示有身份的对象或函数；右值（rvalue）包括纯右值（prvalue，如整数值 `3`）和将亡值（xvalue，如 `std::move(value)`）。将亡值仍表示有身份的对象，只是允许它作为移动来源，并不意味着对象马上被销毁。

```cpp
int value = 10;
int& leftRef = value;       // int&：非常量左值引用，只能绑定到左值
int&& rightRef = 20;        // T&&：右值引用，可以绑定到临时右值
```

`T&` 常用于修改已有对象，`T&&` 是移动语义和完美转发的基础。需要注意：变量一旦有名字，表达式本身就是左值，所以 `rightRef` 虽然类型是 `int&&`，直接使用 `rightRef` 时仍是左值；若要再次把它当作可移动的右值，需要使用 `std::move(rightRef)`。

## Makefile
普通编译

```bash
g++ test.cpp -o test -std=c++17 -Wall -Wextra
```

```makefile
CXX = g++ # 定义编译器CXX 是 make 的惯例变量名（C++ compiler）
CXXFLAGS = -Wall -Wextra -g -MMD -MP 
#编译选项
# | 参数      | 作用              |
# | -Wall   | 打开常见警告          |
# | -Wextra | 打开更多警告          |
# | -g      | 生成调试信息（给 gdb 用） |
# | -MMD    | 自动生成 .d 依赖文件    |
# | -MP     | 避免删除头文件时报错      |
TARGET = test # 最终生成的可执行文件名最终会生成：./test
SRCS = main.cpp Surcharge.cpp # 所有源文件如果你再加一个文件：SRCS = main.cpp Surcharge.cpp utils.cpp 后面会自动适配。
OBJS = $(SRCS:.cpp=.o) # 变量替换意思是：main.cpp → main.o Surcharge.cpp → Surcharge.o
# $(变量:旧后缀=新后缀)
DEPS = $(SRCS:.cpp=.d) # 生成依赖文件名main.d Surcharge.d .d 文件是 -MMD 自动生成的依赖文件。
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
# 等价于g++ -Wall -Wextra -g -MMD -MP -o test main.o Surcharge.o
# | 符号 | 意义              |
# | -- | --------------- |
# | $@ | 当前目标名（这里是 test） |
# | $< | 第一个依赖文件         |
# | $^ | 所有依赖文件          |
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@  
# 模式匹配规则 意思是：任何 .o 文件 都可以由 对应的 .cpp 文件 生成
# 等价于g++ -Wall -Wextra -g -MMD -MP -c main.cpp -o main.o  其中 -c 表示只编译，不链接

-include $(DEPS) # 自动包含依赖，意思是：包含所有 .d 文件。前面的 - 表示：如果 .d 文件不存在，不报错

.PHONY: clean
# 伪目标 clean 的意思是：告诉 make clean 不是一个文件名，而是一个命令。否则如果目录里有个叫 clean 的文件就会冲突。
clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
```

补充说明：

- `-Wall`：打开常见警告
- `-Wextra`：打开更多警告
- `-g`：生成调试信息，给 `gdb` 用
- `-MMD`：自动生成 `.d` 依赖文件
- `-MP`：避免删除头文件时报错
- `$@`：当前目标名
- `$<`：第一个依赖文件
- `$^`：所有依赖文件
- `-include $(DEPS)`：自动包含依赖文件；前面的 `-` 表示即使文件不存在也不报错
- `.PHONY: clean`：声明伪目标，避免和同名文件冲突

## 数据类型

| 数据类型 | 说明           | 内存大小（字节） |
| -------- | -------------- | -------------- |
| `int`    | 整数           | 4              |
| `long`   | 长整数         | 4 或 8         |  
| `float`  | 单精度浮点数     | 4              |
| `double` | 双精度浮点数     | 8              |
| `char`   | 字符           | 1              |
| `bool`   | 布尔值（true/false） | 1              |
| `long long` | 长长整数(至少-2^63~2^63 - 1)       | 8              |

```cpp
int number = 1; // 整数
(char)number; // 强制类型转换为 char，结果是 '\x01'（ASCII 码为 1 的字符）
char c = '0'+number; // 结果是 '1'（ASCII 码为 48 的字符）
char c2 = to_string(number)[0]; // 结果也是 '1'，to_string(number) 将整数转换为字符串 "1"，然后取第一个字符
```

## `const` 的用法

```cpp
bool stackArray::isEmpty() const {
    return head == -1;
}
```

这里的 `const` 保证函数不修改 `head` 的数值。

## Attributes、annotations 与编译器扩展

C++ 标准属性使用 `[[attr]]` 语法，为编译器提供额外信息，但不改变普通类型语法。属性可以带参数，也可以放在函数、类型或变量等特定位置：

```cpp
[[deprecated("请改用 newApi")]]
void oldApi();

[[noreturn]]
void fatalError(); // 承诺函数不会正常返回
```

- `[[deprecated]]` 表示实体仍可使用，但编译器通常会给出弃用警告。
- `[[noreturn]]` 表示函数不会返回调用者；若它实际正常返回，程序行为未定义。
- `[[carries_dependency]]` 曾用于描述底层原子操作的依赖链，实际支持和标准状态取决于语言版本与编译器；新代码通常不应依赖它。

下面几种写法也能向编译器提供额外指令，但可移植性不同：

| 写法 | 来源 | 示例 |
| --- | --- | --- |
| `[[attr]]` | C++ 标准属性语法 | `[[deprecated]]` |
| `#pragma` | 编译器指令，不同实现支持项不同 | `#pragma once` |
| `__attribute__((...))` | GCC / Clang 扩展 | `__attribute__((unused))` |
| `__declspec(...)` | MSVC 扩展 | `__declspec(dllexport)` |

优先使用标准 `[[...]]` 属性；必须使用平台扩展时，通常用条件编译或宏封装，避免把编译器专用语法散布到整个项目。

## 判断

- 可以写成 `if (条件) return; else return;`
- 也可以写成三目运算符：`return (条件) ? 正确结果 : 错误结果;`
- 有时还可以直接 `return (条件);`，这样返回值就是 `1` 或 `0`

## 循环

```cpp
vector<int> candies(5, 10);  // 创建一个长度为 5 的数组，每个元素初始为 10

for (int x : candies) {
    // 依次把 candies 里的每个元素取出来赋给 x
}
```

## 默认赋值

例如：

```cpp
stackArray(int size = default_size);
```

如果这段出现在 `.h` 文件中，表示调用时如果没有给 `size` 传值，就自动使用 `default_size`。

## 命名空间

`namespace` 用于封装同一领域的一组实体，本质上就是给名字“分组”，用来避免命名冲突。使用 `::` 运算符可以访问命名空间中的成员。

## 全局变量

全局变量是在所有函数外定义的变量，整个程序都能访问。它的内存在程序启动时分配，在程序结束时释放。

## 静态变量

静态变量只创建一次，生命周期贯穿整个程序。例如 `static int cpt = 0;` 在某函数中一旦被创建，即使后续再次调用该函数，也不会重新初始化。它的内存同样在程序启动时分配，在程序结束时释放。

## `this` `other`

`this` 是一个指针，指向“当前正在使用这个成员函数的那个对象”。`other` 没有什么特殊语法意义，它只是程序员自己起的参数名字。

## 构造函数

构造函数是类的一种特殊成员函数，用于在创建对象时初始化对象。构造函数名称与类名相同，没有返回类型。

### 类内声明与类外定义

成员函数可以只在头文件的类中声明，再到 `.cpp` 文件中定义。类外定义时需要使用作用域解析运算符 `::`，说明这个函数属于哪个类：

```cpp
// Sensor.h
class Sensor {
public:
    Sensor(double initialValue);

private:
    double value;
    bool active;
};
```

```cpp
// Sensor.cpp
#include "Sensor.h"

Sensor::Sensor(double initialValue)
    : value(initialValue), active(false)
{
}
```

这段构造函数定义可以按三部分理解：

| 语法 | 含义 |
| --- | --- |
| `Sensor::Sensor(double initialValue)` | `::` 指定右侧的构造函数属于 `Sensor` 类 |
| `: value(initialValue)` | 单个冒号开始构造函数的成员初始化列表 |
| `, active(false)` | 逗号分隔多个成员初始化项 |

`::` 用于限定名字所属的作用域，不只用于类，也用于命名空间，例如 `std::string`。类内已经处于该类的作用域，所以在类内直接写 `Sensor(...)`；到了类外，则要写成 `Sensor::Sensor(...)`。

### 成员初始化列表

初始化列表会在进入构造函数体之前直接初始化成员，比先默认初始化、再在函数体中赋值更准确：

```cpp
// 推荐：直接初始化
Sensor::Sensor(double initialValue)
    : value(initialValue), active(false)
{
}

// 这是先初始化、后赋值，并不完全等价
Sensor::Sensor(double initialValue)
{
    value = initialValue;
    active = false;
}
```

`const` 成员、引用成员、没有默认构造函数的对象成员，以及基类部分，必须通过初始化列表初始化。成员真正的初始化顺序只由它们在类中的**声明顺序**决定，而不是由初始化列表的书写顺序决定，因此最好让两者保持一致：

```cpp
class Record {
    const int id;
    std::string name;

public:
    Record(int value, const std::string& text)
        : id(value), name(text) {}
};
```

需要注意，同一个符号可能在不同语法位置承担不同职责：`public:` 中的冒号标记访问控制区域，构造函数参数列表后的冒号开始初始化列表，而初始化项之间使用逗号分隔。判断含义时应结合它所在的语法位置。

### 构造技巧：委托构造、`= default` 与 `= delete`

一个构造函数可以调用同一个类的另一个构造函数，这称为**委托构造**，可以避免重复初始化代码：

```cpp
class Device {
    int id;

public:
    Device() : Device(0) {}            // 委托给下面的构造函数
    explicit Device(int value) : id(value) {}

    Device(const Device&) = default;   // 要求编译器生成默认拷贝构造
};
```

`= default` 表示使用编译器生成的默认实现；`= delete` 表示明确禁止某种调用。例如，不允许对象被复制：

```cpp
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};
```

### `explicit`：禁止隐式转换

只有一个参数的构造函数有时会被编译器当作“类型转换规则”。例如：

```cpp
class Temperature {
public:
    Temperature(double value) {}
};

void printTemperature(Temperature value) {}

printTemperature(36.5); // 编译器自动把 36.5 转成 Temperature
```

如果不希望发生这种不明显的自动转换，就在构造函数前加 `explicit`：

```cpp
class Temperature {
public:
    explicit Temperature(double value) {}
};

Temperature a(36.5);                  // 正确：直接初始化
Temperature b{36.5};                  // 正确：直接列表初始化
// Temperature c = 36.5;              // 错误：拷贝初始化不使用 explicit 构造函数
// printTemperature(36.5);             // 错误：不再允许隐式转换
printTemperature(Temperature(36.5));   // 正确：明确创建对象
```

直接初始化会考虑 `explicit` 构造函数，拷贝初始化则不会。简单记忆：`explicit` 表示“必须明确地构造对象”。能接收单个其他类型实参的构造函数通常建议加上它，除非确实希望两种类型可以自动转换。

### 拷贝构造函数

拷贝构造函数的作用是：**根据一个已有对象，创建一个内容相同的新对象**。典型签名是 `类名(const 类名& other)`：

```cpp
class Record {
    std::string name;
    int score;

public:
    Record(const std::string& n, int s) : name(n), score(s) {}

    Record(const Record& other)
        : name(other.name), score(other.score) {}

    void setScore(int value) { score = value; }
    int getScore() const { return score; }
};

Record a("Alice", 90);
Record b = a;               // 写法一：调用拷贝构造函数
Record c(a);                // 写法二：同样调用拷贝构造函数
b.setScore(100);            // b 是独立的新对象，不影响 a
std::cout << a.getScore();  // 仍然输出 90

Record d("Bob", 80);
d = a;                      // 对象 d 已经存在，这是拷贝赋值
```

拷贝构造（copy construction）发生在“创建新对象”时；拷贝赋值（copy assignment）发生在“修改已有对象”时。**有没有创建新对象，是判断 copy constructor 和 `operator=` 的关键。**按值传参以及按值返回对象时也可能发生拷贝，但返回对象的拷贝经常会被 RVO/NRVO 省略。

赋值（assignment）还可分为两种：

```cpp
T& operator=(const T& other); // copy assignment：复制内容，保留源对象
T& operator=(T&& other);      // move assignment：转移资源
```

移动赋值通常接收右值，把源对象持有的资源转交给目标对象；移动后源对象仍可析构和重新赋值，但不应依赖它原来的内容。

参数使用 `const Record&` 有三个原因：引用避免为了传参再次拷贝，`const` 保证不修改原对象，并且还能接收常量对象。拷贝构造函数不能按值接收同类型参数，因为初始化这个参数本身就需要调用拷贝构造函数。

**如果没有自己写拷贝构造函数，编译器通常会自动生成一个。**如果成员都是 `int`、`std::string`、`std::vector` 等可正常复制的类型，这个默认版本通常就够了。若类直接管理裸指针，默认行为只会复制地址，两个对象可能指向同一块内存，这称为**浅拷贝**，容易导致修改互相影响或重复释放；为了避免这些风险，需要让新对象拥有独立资源，也就是**深拷贝**。现代 C++ 应优先让标准容器或智能指针管理资源（Rule of Zero）：容器可直接复制，而 `unique_ptr` 会明确禁止复制，避免意外共享资源。

下面用裸指针直观展示深拷贝。关键是 `new int(*other.value)`：先申请一块新内存，再把原对象保存的值复制进去。

```cpp
class Box {
    int* value;

public:
    Box(int n) : value(new int(n)) {}

    Box(const Box& other)
        : value(new int(*other.value)) {} // 深拷贝：地址不同，数值相同

    Box& operator=(const Box& other) {
        if (this != &other) {              // 防止 a = a 这种自赋值
            int* newValue = new int(*other.value);
            delete value;
            value = newValue;
        }
        return *this;
    }

    ~Box() { delete value; }

    void setValue(int n) { *value = n; }
    int getValue() const { return *value; }
};

Box a(10);
Box b = a;
b.setValue(20);

std::cout << a.getValue(); // 10，说明 a 和 b 拥有独立的数据
```

`if (this != &other)` 比较当前对象和源对象的地址，避免 `a = a;` 这种 self-assignment（自赋值）破坏自己的资源。这个例子用于说明资源复制的原理，实际项目通常优先使用标准容器和智能指针。

### 移动语义、移动构造与 `std::move`

拷贝构造函数（copy constructor）会复制资源；移动构造函数（move constructor）则接收 `T&&`，尽量把资源所有权转移给新对象，避免昂贵的深拷贝：

```cpp
class Buffer {
    std::vector<int> data;

public:
    Buffer(std::size_t size) : data(size) {}
    Buffer(const Buffer& other) : data(other.data) {}              // 拷贝
    Buffer(Buffer&& other) noexcept : data(std::move(other.data)) {} // 移动

    Buffer& operator=(const Buffer&) = default; // copy assignment
    Buffer& operator=(Buffer&&) noexcept = default; // move assignment
};

Buffer source(1000);
Buffer copied = source;              // source 是左值，调用拷贝构造
Buffer moved = std::move(source);    // 转为右值，调用移动构造
```

移动语义（move semantics）的核心是转移资源而不是逐项复制。`std::move()` 本身不搬运任何数据，它只是把表达式转换为右值，是否真正移动取决于类型是否提供移动构造或移动赋值。被移动对象仍可析构或重新赋值，但不要依赖它原来的内容。

### RVO 与 NRVO

RVO（返回值优化）会省略返回临时对象时的拷贝/移动；NRVO（命名返回值优化）针对返回函数内的具名局部对象。对象会直接在调用方的存储位置构造：

```cpp
Record makeRvo() {
    return Record("RVO", 1);  // 临时对象：C++17 起保证省略拷贝/移动
}

Record makeNrvo() {
    Record result("NRVO", 2);
    return result;             // 具名局部对象：NRVO 允许但不保证
}

Record choose(bool first) {
    Record a("A", 3);
    Record b("B", 4);
    if (first) return a;
    return b;                  // 可能返回不同对象，通常无法进行 NRVO
}

Record x = makeRvo();
Record y = makeNrvo();
```

要点：

- C++17 起，`return T{...};` 这类同类型纯右值会直接构造结果对象，即使拷贝/移动构造函数被删除也可以成立。
- 返回局部对象时的一般优先关系是 **RVO/NRVO → move → copy**：先尝试省略构造；不能省略时，通常优先移动；不能移动时才复制。
- NRVO 仍是可选优化，因此被返回类型仍应提供可用的移动或拷贝构造函数。
- 不要写 `return std::move(result);` 来“帮助”优化：它会把表达式变成右值，通常反而阻止 NRVO。
- 不应依赖拷贝/移动构造函数中的副作用，因为是否省略可能改变它们的调用次数。

## 析构函数

析构函数用于在对象生命周期结束时执行清理操作。析构函数名称与类名相同，但前面加波浪号 `~`，并且没有返回类型和参数。

如果没有手写析构函数，编译器会自动生成一个默认析构函数，负责做基础销毁工作。

一定要写析构函数的典型情况：

类里有 new 
```cpp
class A {
    int* p;
public:
    A() {
        p = new int(5);
    }
};
// 如果你不写析构函数：
// ❌ 内存泄漏
// 必须写：
~A() {
    delete p;
}
```

`new[]` 创建的动态数组必须使用 `delete[]` 释放；若错误地使用单对象的 `delete`，行为未定义：

```cpp
Image* image = new Image[10];
// 使用 image[0] 到 image[9]
delete[] image;
image = nullptr;
```

`new` 对应 `delete`，`new[]` 对应 `delete[]`，两组不能混用。现代 C++ 通常优先使用 `std::vector<Image>` 或 `std::make_unique<Image[]>(10)` 自动管理数组生命周期。

## 成员函数和友元函数
成员函数就是“属于这个类的函数”；友元函数不是这个类的成员，但被这个类授权，可以访问它的 private / protected。
```cpp
成员函数格式
class ClassName
{
public:
    ReturnType functionName(ParameterList)
    {
        // function body
    }
};
友元函数格式
class ClassName
{
public:
    friend ReturnType functionName(ParameterList);
};
```
例子：
```cpp
class Vector2D
{
public:
    Vector2D(int x, int y) : x_(x), y_(y) {
        std::cout << "构造Vector2D" << std::endl;
    }

    Vector2D operator+(const Vector2D& other) const
    {
        return Vector2D(x_ + other.x_, y_ + other.y_);
    }

private:
    int x_;
    int y_;
};
```
这里：
```cpp
Vector2D operator+(const Vector2D& other) const
```
就是成员函数。

它其实等价于：
```cpp
Vector2D operator+(const Vector2D& other) const
{
    return Vector2D(this->x_ + other.x_,
                    this->y_ + other.y_);
}
```
这里的：

this就是当前对象。

比如：
```cpp
Vector2D first(1, 2);
Vector2D second(3, 4);

first + second;
// 实际上调用的是：first.operator+(second);
```
所以：
this指向 first
other就是 second

也就是说：

x_其实就是：this->x_

成员函数最常见的写法有两种。

第一种，直接写在类里面：
```cpp
class Student
{
public:
    void hello()
    {
        std::cout << "hello" << std::endl;
    }
};

调用：

Student s;
s.hello();
```

第二种，类里面只声明，类外定义：
```cpp
class Student
{
public:
    void hello();
};

然后类外：

void Student::hello()
{
    std::cout << "hello" << std::endl;
}
```
这里：

Student::

表示：hello() 是 Student 类的成员函数。

成员函数有几个重要特点。

第一，它属于类。

所以调用方式通常是：obj.function();比如：first.operator+(second);

第二，它有隐含的 this 指针。

所以成员函数里面可以直接访问：x_,y_。而不需要写：first.x_

第三，成员函数天然可以访问本类的：public、protected、private

再看友元函数。

例如：
```cpp
class Vector2D
{
public:
    Vector2D(int x, int y) : x_(x), y_(y) {}

    friend std::ostream& operator<<(std::ostream& os,
                                    const Vector2D& vector)
    {
        return os << "("
                  << vector.x_
                  << ", "
                  << vector.y_
                  << ")";
    }

private:
    int x_;
    int y_;
};
```
这个：

operator<< 

虽然写在类里面，但它不是成员函数。

所以不能写：

this->x_

因为它根本没有 this。

它必须通过参数：
vector.x_
vector.y_
访问对象。

但是因为前面有：

friend

所以即使：

x_,y_是 private，它也可以访问。

比如成员函数：
```cpp
class A
{
private:
    int x_;

public:
    void show()
    {
        std::cout << x_;
    }
};
```
调用：

A a;
a.show();

本质上：

a.show();

里面：

this == &a

友元函数：
```cpp
class A
{
private:
    int x_;

public:
    friend void show(const A& a);
};

类外定义：

void show(const A& a)
{
    std::cout << a.x_;
}
```
调用：

A a;
show(a);

注意这里不是：a.show();

因为 show 不是成员函数。

为什么有时候要用友元函数，而不全写成员函数？

最经典就是：

std::cout << obj;

因为左边是：

std::cout

不是你的对象。

## 运算符重载

基本语法：
返回类型 operator运算符(参数)
{
    // 操作
}

```cpp
class Point {
public:
    int x, y;

    Point(int x, int y) : x(x), y(y) {}

    Point operator+(const Point& other) const {//最后一个const表示这个成员函数不会修改当前对象this指向的对象
        return Point(x + other.x, y + other.y);
    }
};
//使用：
Point a(1, 2);
Point b(3, 4);

Point c = a + b;

//等价于：
Point c = a.operator+(b);
```
```cpp
常见运算符

==：

bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
}

+=：

Point& operator+=(const Point& other) {
    x += other.x;
    y += other.y;
    return *this;
}

前置 ++：

Point& operator++() {
    ++x;
    ++y;
    return *this;
}

后置 ++：

Point operator++(int) {
    Point temp = *this;
    ++x;
    ++y;
    return temp;
}

区别：

++a   -> a.operator++()
a++   -> a.operator++(0)

输出运算符 << 通常写成非成员函数：

friend std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.x << ", " << p.y;
    return os;
} 


使用：

cout << p;

可以理解为：

operator<<(cout, p);
```

C++ 里面没有“幂运算符” `^`。

```cpp
2 ^ 31   // ❌ 不是 2 的 31 次方，而是按位异或
```

例如：

```text
 2  = 00010
31  = 11111
-----------
XOR = 11101 = 29
```

真正表示 `2` 的 `31` 次方，常见写法是：

```cpp
pow(2, 31);   // 需要 <cmath>
1 << 31;      // 也是常见位运算写法
```

运算符重载的核心是：允许程序员为自定义类型定义运算符行为，让它们像内置类型一样使用。

1. `a + b` 本质上可以写成 `operator+(a, b)`，或者成员函数形式 `a.operator+(b)`
2. 运算符重载通常有两种实现方式：外部函数 / 成员函数
3. 外部函数写法：不属于类，没有 `this` 指针，所有操作数都作为参数传入
4. 成员函数写法：左操作数就是 `this`，右操作数作为参数传入

典型声明示例：

```cpp
const T operator+(const T& a, const T& b);
```

### Functor / function object 与 `operator()`

重载函数调用运算符 `operator()` 的类对象称为函数对象（Functor / function object）。它可以像普通函数一样调用，同时在成员变量中保存上下文或状态：

```cpp
class Adder {
    int base;

public:
    explicit Adder(int value) : base(value) {}

    int operator()(int value) const {
        return base + value;
    }
};

Adder add10(10);         // base 是函数对象保存的状态
int result = add10(5);   // 普通调用语法，等价于 add10.operator()(5)
```

普通函数本身不能保存每个实例独有的状态，而不同的函数对象可以各自保存不同的 `base`。带捕获的 Lambda 本质上也会生成一个保存捕获状态、并提供 `operator()` 的闭包对象。

## `auto`

`auto` 根据初始化表达式推断类型，因此声明变量时必须能得到初始值：

```cpp
const int value = 10;
const int& ref = value;

auto a = ref;        // int：按值推断会去掉顶层 const 和引用
auto& b = ref;       // const int&：auto& 保留引用和 const
auto* p = &value;    // const int*：保留指向对象的 const
const auto c = value; // const int：显式加回顶层 const
```

简单理解：`auto` 默认得到一个新的值，`auto&` 则绑定原对象；修改 `b` 是否允许，取决于原对象是否为 `const`。

这里的“去掉 `const`”只针对**顶层 `const`**，即变量自身不可修改；不会去掉指向对象的 `const`：

```cpp
int number = 10;
int* const fixed = &number; // 指针自身是 const，所指 int 不是 const
auto pointer = fixed;      // int*：去掉指针自身的顶层 const

const int* source = &number;
auto readonly = source;    // const int*：保留所指对象的 const
```

更准确地说，普通 `auto` 使用类似函数模板参数的推导规则，声明中的 `&`、`&&`、`const` 也参与决定最终类型。`auto&&` 在常见的表达式初始化中是**转发引用**，并不保证得到右值引用：

```cpp
int number = 10;
const int constant = 20;

auto&& a = number;   // int&：绑定左值，引用折叠后仍是左值引用
auto&& b = constant; // const int&：保留被引用对象的 const
auto&& c = 30;       // int&&：绑定纯右值
```

因此，“`auto` 默认去掉引用和顶层 `const`”说的是 `auto name = expr` 这种按值声明，不能直接套到 `auto&` 或 `auto&&`。按值推导还会让数组、函数退化为相应指针；引用形式则可以保留它们的类型。

什么时候该用 `auto`（推荐）：

1. 迭代器，是最常见也最推荐的场景

   ```cpp
   for (auto it = s.begin(); it != s.end(); ++it)
   ```

   原因：

   - `string::iterator` 太长
   - 以后如果容器类型变了，代码不用一起改

2. STL 返回值类型很复杂时

   ```cpp
   auto it = mp.find(key);
   // 等价于
   unordered_map<int, string>::iterator it = mp.find(key);
   ```

### 尾置返回类型与 C++14 返回类型推断

尾置返回类型（trailing return type）把返回类型写在参数列表后面，适合返回类型依赖参数的模板或较复杂类型：

```cpp
auto add(int a, int b) -> int {
    return a + b;
}
```

C++14 起，普通函数可以直接用 `auto` 从 `return` 表达式推断返回类型；多个 `return` 必须能推断为同一类型：

```cpp
auto multiply(int a, int b) {
    return a * b; // 推断为 int
}
```

### `decltype` 与 `decltype(auto)`

**`auto` 和 `decltype` 使用不同的类型推导规则。** 可以这样记：普通 `auto` 更关心“我要用这个初始值声明一个什么类型的新变量”；`decltype` 更关心“这个实体的声明类型，或这个表达式的类型与值类别是什么”。`decltype` 在编译期查询类型，不会实际执行括号中的表达式。

对于这里讨论的普通变量和成员，判断 `decltype(expr)` 分两步：

1. **未额外加括号的变量名或成员访问**，例如 `x`、`object.member`：直接取所命名实体的声明类型，可以包括 `const`、`&`、`&&`。
2. **其他表达式**：设表达式的类型为 `T`，再按值类别决定结果：

   | 表达式值类别 | `decltype(expr)` 的结果 | 例子（`x` 是 `int` 变量） |
   | --- | --- | --- |
   | 左值（lvalue）：表示一个可定位的对象 | `T&` | `decltype((x))` 是 `int&` |
   | 将亡值（xvalue）：有身份、可作为移动来源的表达式 | `T&&` | `decltype(std::move(x))` 是 `int&&` |
   | 纯右值（prvalue）：用于计算值或初始化对象的表达式 | `T` | `decltype(x + 1)` 是 `int` |

将亡值与纯右值合称右值，但在 `decltype` 中，两者的结果并不相同。

为什么一般表达式的左值对应 `T&`，而不是仅仅得到 `T`？因为只给出 `T` 就无法区分“计算一个值”和“访问一个已有对象”。`decltype` 用引用类型把值类别的信息编码到结果类型中：`T&` 表达左值访问，`T&&` 表达可作为移动来源的访问，`T` 则对应纯右值。

以同一个 `int` 对象为例，可以观察这三种结果在声明变量时的区别：

```cpp
#include <utility>

int x = 3;
decltype((x)) alias = x;                // int&：绑定已有对象 x
decltype(3) copy = x;                   // int：用当前值初始化独立对象
decltype(std::move(x)) movable = std::move(x); // int&&：仍然绑定 x

alias = 10;              // 修改 x；copy 仍为 3
movable = 20;            // 修改的仍是 x，不是新建的 int
// &alias == &x，&movable == &x；copy 则是另一个对象
```

这里 `x`、`(x)` 和 `std::move(x)` 的表达式类型都是 `int`，区别在值类别；`decltype` 的结果才分别按规则表现为 `int`、`int&`、`int&&`。`std::move(x)` 本身只改变表达式的值类别，不复制对象，也不执行资源转移；是否发生移动取决于后续操作是否调用移动构造或移动赋值。

还要区分**类型信息**和**具体对象身份**：`int&` 说明可以绑定一个 `int` 左值，却不记录“它一定是 x”；是初始化中的 `= x` 让 `alias` 绑定到 x。`decltype((x))` 本身不会创建引用变量，也不会修改 x。它让后续声明能表达对已有对象的访问，而不必退化为按值复制。

```cpp
#include <utility> // std::move

int x = 10;
int& ref = x;
const int& cref = x;
int&& rref = 20;

decltype(x) a = 1;       // int
decltype(ref) b = x;     // int&
decltype((x)) c = x;     // int&：额外括号使 (x) 按左值表达式处理
decltype(x + 1) d = 20;  // int：x + 1 是纯右值
decltype(cref) e = x;    // const int&：保留声明类型
decltype(rref) f = 30;   // int&&：变量名走第一条规则
decltype((rref)) g = x;  // int&：有名字的右值引用变量，表达式仍是左值
decltype(std::move(x)) h = std::move(x); // int&&：表达式是将亡值
```

**括号不改变 `x` 的值类别，却会改变 `decltype` 采用哪条规则。** `decltype(x)` 查询声明类型，而 `decltype((x))` 按左值表达式计算；同样，若 `const` 对象的成员声明为 `int`，`decltype(object.member)` 是 `int`，`decltype((object.member))` 则是 `const int&`（普通非 `mutable` 成员）。所以不能把 `decltype` 简化成“总是照搬表达式的一切限定”。

用同一个初始值对比，差异最明显：

```cpp
const int value = 10;
const int& ref = value;

auto copy = ref;             // int：独立的新值，可以修改 copy
decltype(ref) alias = ref;   // const int&：绑定原对象，不能通过 alias 修改它
decltype(value) same = 20;   // const int：新对象保留声明类型的 const
```

`decltype(auto)` 使用 `decltype` 的规则推断完整类型，能保留引用；返回时括号会影响结果：

```cpp
int value = 10;

decltype(auto) getValue() { return value; }      // int
decltype(auto) getReference() { return (value); } // int&
```

变量初始化也适用相同规则：`decltype(auto) a = value;` 得到 `int`，`decltype(auto) b = (value);` 得到 `int&`；若初始值是前例的 `const int& ref`，则 `decltype(auto) c = ref;` 得到 `const int&`。它不是“更聪明的 `auto`”，而是选择了另一套规则；不能写 `decltype(auto)&`，返回引用时也必须确保被引用对象仍然存活，不能返回局部对象的悬空引用。

| 写法 | 主要用途 | 是否保留引用和顶层 `const` |
| --- | --- | --- |
| 普通按值 `auto` | 从初始值推导新变量或函数返回值 | 默认去掉；可显式使用 `const auto` 等形式 |
| `auto&` / `auto&&` | 绑定对象或转发 | 根据声明形式、初始化表达式与引用折叠决定 |
| `decltype(expr)` | 查询实体声明类型或表达式类型与值类别 | 按上述两步规则决定，不能笼统说“始终保留” |
| `decltype(auto)` | 在变量或返回类型处使用 `decltype` 规则推导 | 与 `decltype(expr)` 一致，额外括号会影响结果 |

规则依据：C++ 标准工作草案的 [占位类型推导](https://eel.is/c++draft/dcl.type.auto.deduct)、[`decltype` 说明符](https://eel.is/c++draft/dcl.type.decltype)、[值类别](https://eel.is/c++draft/basic.lval)与[转发及移动辅助函数](https://eel.is/c++draft/forward)。以上示例使用 C++17 已有规则，不涉及较新标准新增的语法。

## 模板（`template`） {#templates}

模板让一份代码描述一族函数、类或变量，由编译器根据**类型参数或编译期值**生成需要的具体版本。它是 C++ 泛型编程的基础：先表达算法需要哪些操作，再让满足要求的类型参与，而不是为每一种类型复制代码。

本节以 C++17 为主要编译标准；Concepts、`requires` 等 C++20 内容单独标注。前面的 `auto`、`decltype` 和值类别讲解在这里会继续用到。

本章依次按函数模板、类模板、模板约束、模板的其他功能组织。各分类内先讲基础规则，再说明与其直接相关的扩展；已有的现代 C++ 内容保留在相应分类中。

### Template Functions（函数模板） {#template-functions}

#### Template 基本概念

不使用模板时，同一个取较大值算法可能分别写成 `int larger(int, int)`、`double larger(double, double)`。两者只有类型不同，重复实现却要分别维护。模板把类型变化提取为参数，让算法主体只写一次。

模板不是运行时传入一个“类型对象”，也不是动态类型：`larger<int>` 与 `larger<double>` 是编译期确定的不同函数版本。它们生成后仍可像普通函数一样在运行时调用。

#### 函数模板语法

假设要对整数、浮点数做同样的“取较大值”操作。分别写多个函数会重复算法；模板把变化部分抽成类型参数 `T`：

```cpp
template<typename T>
T larger(T a, T b) {
    return a < b ? b : a;
}

int integer = larger(3, 7);          // 推导 T = int，结果为 7
double real = larger(2.5, 1.0);      // 推导 T = double，结果为 2.5
double mixed = larger<double>(3, 4.5); // 显式指定 T，再按 double 参数接收
// larger(3, 4.5);                   // 推导冲突：T 同时需要是 int 和 double
```

`template<typename T>` 是模板参数列表；`T a, T b` 是普通函数参数列表。类型参数在编译时确定，`a`、`b` 的值可以在运行时才得到。这里要求 `T` 支持 `<`，并能按值传递和返回；模板不意味着“任何类型都能用”。指针使用 `<` 也不会自动变成字符串内容比较，通用接口仍需明确语义。

声明类型参数时，`typename T` 与 `class T` 等价；`class T` 不要求实参一定是类，`int` 也可以。模板通常声明在命名空间或类作用域，不能直接在普通函数体中声明一个类模板或函数模板。

#### 模板实例化（Instantiation） {#template-function-instantiation}

定义模板是在描述生成规则；当使用需要某个具体版本时，编译器按模板实参进行实例化。例如调用 `larger(3, 7)` 推导出 `T = int`，再使用对应的 `larger<int>`；调用 `larger(2.5, 1.0)` 使用 `larger<double>`。

```cpp
template<class T>
T twiceValue(T value) { return value + value; }

int integer = twiceValue(3);       // 隐式实例化需要的 int 版本
double real = twiceValue(1.5);     // 隐式实例化需要的 double 版本
template long twiceValue<long>(long); // 显式实例化定义
```

实例化不是“在运行时生成函数”，也不保证每个版本必然保留一份独立机器代码，优化器可能内联或合并代码。显式实例化使用既有规则生成版本；显式特化则另写一套实现。头文件可见性与集中实例化见后面的[编译与链接](#template-instantiation)。

#### 模板类型推导（Type Deduction） {#template-deduction}

函数模板从函数实参与形参结构推导类型。前面普通 `auto`、`auto&`、`auto&&` 的推导与这个机制紧密相关。

| 形参形式 | 主要效果 |
| --- | --- |
| `T value` | 按值推导，忽略实参的引用和顶层 `const`；数组/函数通常退化为指针 |
| `T& value` | 绑定左值，保留被引用对象的限定；能保留数组长度 |
| `const T& value` | 以只读引用接收；可绑定左值或临时对象，`const` 由形参提供 |
| 可推导的 `T&& value` | 转发引用；根据实参值类别推导并发生引用折叠 |

例如 `const int n = 3` 传给 `T` 时推导 `T = int`，传给 `T&` 时推导 `T = const int`，传给 `const T&` 时推导 `T = int`，最终形参仍为 `const int&`。

推导并不先尝试所有普通隐式转换来让模板匹配；前面 `larger(3, 4.5)` 不能自动“统一成 double”。推导完成后，对最终选定函数的调用仍可能进行允许的参数转换。返回类型也不会反向指导普通调用：

```cpp
template<class T>
T makeDefault() { return T{}; }

// int n = makeDefault();   // 无函数实参可用来推导 T；左侧 int 不会补上 T
int n = makeDefault<int>();
```

有些位置属于**非推导上下文**，例如 `typename T::value_type` 中的 `T`：仅凭某个成员类型，无法唯一反推出它属于哪个容器。C++20 的 `std::type_identity_t<T>` 可有意让某个参数不参与推导，让 `T` 由其他参数先确定；在 C++17 中可以用一个带 `using type = T` 的类模板表达相同思路。

#### 显式指定模板实参

推导不能统一两种实参类型时，可以明确指定类型，让对应形参按照这个类型接收实参：

```cpp
template<class T>
T minimum(T a, T b) { return b < a ? b : a; }

double result = minimum<double>(10.4, 23); // 23 转成 double，再比较
// minimum(10.4, 23);                     // 推导要求同一个 T，发生冲突
```

`minimum<double>` 中的参数是编译期模板实参，圆括号内才是函数实参。显式指定也不能使不存在或不允许的转换变得合法，转换还可能改变精度。显式实参按声明顺序从左到右填写；剩余参数若允许，仍可推导或采用默认值。

#### 多个模板类型参数

两个输入不必同类型时，应把它们建模为两个独立参数，而不是强迫调用者指定共同类型：

```cpp
#include <type_traits>

template<class T1, class T2>
std::common_type_t<T1, T2> addMixed(T1 a, T2 b) {
    using Result = std::common_type_t<T1, T2>;
    return static_cast<Result>(a) + static_cast<Result>(b);
}

auto result = addMixed(2, 3.5);
static_assert(std::is_same_v<decltype(result), double>);
```

这里分别推导 `T1 = int`、`T2 = double`，`common_type_t` 确定共同结果类型。并非任意类型组合都有共同类型，也不保证结果不会溢出。可以显式写 `addMixed<int>(2, 3.5)` 只指定第一个参数，第二个继续推导；是否需要共同类型取决于算法，表达式的结果类型也可用后面的尾置返回类型描述。

#### 函数模板重载（Overloading）

普通函数与函数模板可以共存。先比较候选的可行性与转换质量；在其他条件相当时，非模板函数通常优先，多个函数模板则通过偏序等规则挑选更特定的版本。

```cpp
int choose(int) { return 1; }

template<class T>
int choose(T) { return 2; }

template<class T>
int choose(T*) { return 3; }

int value = 0;
int a = choose(1);       // 1：相同匹配质量下选普通函数
int b = choose(1.5);     // 2：模板精确匹配 double，优于转换成 int
int c = choose(&value);  // 3：T* 模板比通用 T 模板更特定
int d = choose<>(1);     // 2：显式模板调用，不选普通 choose(int)
```

#### 显式特化（Explicit Specialization）

显式特化为一组确定的模板参数提供不同实现。下面通用版本比较值，`const char*` 版本比较字符串内容，避免把字符指针的比较误当作文本比较：

```cpp
#include <cstring>

template<class T>
bool lessValue(T a, T b) { return a < b; }

template<>
bool lessValue<const char*>(const char* a, const char* b) {
    return std::strcmp(a, b) < 0;
}

bool ordered = lessValue("alpha", "beta"); // 使用 const char* 特化
```

本例要求两个指针指向有效、以空字符结尾的字符串。`template<>` 表示模板参数已完全确定；相同签名但没有 `template<>` 的函数通常是普通重载，不是特化。特化声明应在触发相关隐式实例化的首次使用前可见。

函数模板**不能偏特化**。想处理“所有指针”应写 `template<class T> ... (T*)` 重载；类模板的偏特化在后文单独说明。函数全特化定义放在头文件时还须处理 ODR，通常显式写 `inline` 或集中在一个 `.cpp` 中。

#### 重载与特化的优先关系

不要记成“普通函数永远优先”。函数模板全特化也不作为独立重载候选参加同样的选择流程；通常先确定主模板，再确定它的特化。想给函数增加某一类参数的实现，重载往往比函数全特化更直观。

不能简单给“普通函数、特化、主模板”排出一条固定优先级。先进行重载决议，比较参数转换、模板偏序等；选定主模板后，才使用属于该主模板的显式特化。

下面三个命名空间是三个独立对照，不必猜测声明究竟特化了哪个主模板：

```cpp
namespace GenericSpecialized {
    template<class T> int pick(T) { return 1; }
    template<> int pick<int*>(int*) { return 2; } // 特化通用 T 主模板
    template<class T> int pick(T*) { return 3; }
}
namespace PointerSpecialized {
    template<class T> int pick(T) { return 1; }
    template<class T> int pick(T*) { return 3; }
    template<> int pick<int>(int*) { return 4; } // 特化 T* 主模板
}
namespace OrdinaryOverload {
    template<class T> int pick(T) { return 1; }
    template<class T> int pick(T*) { return 3; }
    int pick(int*) { return 5; }
}
int object = 0;
int first = GenericSpecialized::pick(&object); // 3：选中 T* 主模板
int second = PointerSpecialized::pick(&object); // 4：选中 T* 后使用它的特化
int third = OrdinaryOverload::pick(&object); // 5：同等匹配质量下选普通函数
```

第一组的 `pick<int*>` 属于通用 `T` 主模板；虽然参数也是 `int*`，它不会自动成为 `T*` 主模板的特化。第二组特化的是指针主模板本身，所以能被使用。第三组是普通重载参与决议。

#### 非类型模板参数 {#template-parameters}

在 C++17/20 的常用语法中，模板参数分为类型参数、非类型参数和模板模板参数。较新的标准草案也将非类型参数称为 constant template parameter。

| 参数类别 | 传入的是什么 | 例子 |
| --- | --- | --- |
| 类型参数 | 一个类型 | `template<class T>`，传入 `int` |
| 非类型参数（NTTP） | 编译期值 | `template<std::size_t N>`，传入 `32` |
| 模板模板参数 | 一个符合参数形状的类模板或别名模板 | `template<template<class...> class C>`，传入 `std::vector` |

非类型模板参数也能用于函数模板。固定维数在编译时传入，而数组里的数据仍可在运行时变化：

```cpp
#include <array>
#include <cstddef>

template<class T, std::size_t N = 2>
T firstCoordinate(const std::array<T, N>& values) {
    static_assert(N > 0, "coordinates must not be empty");
    return values[0];
}

std::array<double, 3> position{1.0, 2.0, 3.0};
double first = firstCoordinate(position); // T = double，N = 3
```

`N` 可以推导，也可以显式指定；默认值只在没有其他实参或推导结果时使用。普通运行时变量不能直接作为这种模板实参。C++17 常见非类型参数包括整数、枚举、指针和引用等；C++20 扩展到浮点数及满足结构化类型要求的类。`std::string` 不能直接作为类类型的非类型模板参数，字符串字面量也不能直接充当指针模板实参。

```cpp
template<auto Value> // C++17：从编译期值推导参数类型
struct Constant { static constexpr auto value = Value; };
static_assert(Constant<42>::value == 42);
```

#### 成员函数模板

普通类也可以定义成员函数模板；类本身不必是模板：

```cpp
#include <iostream>

class Printer {
public:
    template<class T>
    void print(const T& value) const { std::cout << value << '\n'; }
};
Printer printer;
void printExamples() {
    printer.print(42);
    printer.print("ready");
}
```

类模板的成员模板还可有自己的类型参数：

```cpp
template<class T>
class Converter {
public:
    template<class U>
    T convert(const U& value) const;
};

template<class T>       // 外层类模板的参数
template<class U>       // 内层成员函数模板的参数
T Converter<T>::convert(const U& value) const {
    return static_cast<T>(value);
}

Converter<double> converter;
double result = converter.convert(3); // 外层 T = double，成员 U = int
```

类外定义这里需要**两层 `template` 声明**，顺序先外层 `T`、再内层 `U`；`Converter<T>::` 指明成员属于哪个类模板实例。外层参数确定之后，每次调用仍可独立推导 `U`。只有一层 `template<class T>` 时，无法表达这个成员函数自身也是模板。

成员函数模板不能是 `virtual`，但类模板可以包含签名固定的普通虚成员函数。成员模板也必须在调用所需的实例化位置具备可用定义。

### Template Classes（类模板） {#template-classes}

#### 类模板基本语法

类模板描述一族类型，`Box<int>`、`Box<std::string>` 才是具体类型；`Box` 本身不能在普通类型位置直接代替它们。

```cpp
#include <string>
#include <utility>

template<class T>
class Box {
    T value_;
public:
    explicit Box(T value) : value_(std::move(value)) {}
    const T& get() const { return value_; }
};

Box<int> numberBox(42);
Box<std::string> textBox(std::string("ready"));
```

这两个实例化产生不同类型，不能默认互相赋值。成员中的 `T` 也会分别成为 `int`、`std::string`。这里返回 `const T&` 避免读取时复制，但引用不能超过 `Box` 对象的生命周期；模板不会自动解决所有权问题。

类外定义成员函数时，要同时写模板参数和具体所属类型：

```cpp
template<class T>
class Holder {
    T value_{};
public:
    const T& get() const;
};

template<class T>
const T& Holder<T>::get() const {
    return value_;
}
```

这里的 `get()` 是**类模板的普通成员函数**：它使用外层参数 `T`，自己没有新增模板参数，因此类外定义只写一层 `template<class T>`。相比之下，前面 `Converter<T>::convert<U>()` 还有内层参数 `U`，类外定义要写两层。成员位于类模板中，并不意味着该成员本身也是成员函数模板。

#### 类型与非类型参数组合

固定维数坐标既需要元素类型，也需要维数；两者共同决定对象类型：

```cpp
#include <array>
#include <cstddef>

template<class T, std::size_t N>
class Point {
    std::array<T, N> coordinates_{};
public:
    T& at(std::size_t index) { return coordinates_.at(index); }
    const T& at(std::size_t index) const { return coordinates_.at(index); }
};
Point<double, 3> spatialPoint;
Point<int, 2> pixelPoint;
```

`Point<double, 3>` 与 `Point<double, 2>` 是不同类型，不能默认互相赋值。类型参数决定数据表示，非类型参数决定编译期维数；运行时可变长度应考虑 `std::vector`。每种不同组合都可能产生新的实例化并增加编译成本。

#### 默认模板参数

默认模板参数为常见配置提供省略写法；即使全部采用默认值，通常仍要写 `<>`：

```cpp
#include <array>
#include <cstddef>

template<class T = int, std::size_t N = 8>
struct Buffer {
    std::array<T, N> data{};
};

Buffer<> defaults;             // Buffer<int, 8>
Buffer<double, 16> samples;    // 容量进入类型
// int size = readSize();
// Buffer<int, size> invalid;  // 普通运行时变量不能作为 N
```

类模板普通参数的默认值通常安排在尾部；之后仍可有参数包。函数模板后续参数若能从函数实参推导，默认参数的排列限制有所不同。调用时显式模板实参从左到右指定，不能像命名参数那样随意跳过中间位置。

#### 类模板构造函数

构造函数负责创建某个已经确定的类模板对象；它与模板实例化不是同一过程。同类型复制可用普通拷贝构造函数，跨类型转换则常用构造函数模板：

```cpp
#include <type_traits>

template<class T>
class Coordinate {
    T value_{};
public:
    Coordinate() = default;
    explicit Coordinate(T value) : value_(value) {}
    Coordinate(const Coordinate&) = default;
    Coordinate(Coordinate&&) = default;
    Coordinate& operator=(const Coordinate&) = default;
    Coordinate& operator=(Coordinate&&) = default;

    template<class U,
             std::enable_if_t<std::is_convertible_v<const U&, T> &&
                              !std::is_same_v<U, T>, int> = 0>
    explicit Coordinate(const Coordinate<U>& other) : value_(other.get()) {}

    const T& get() const { return value_; }
};
Coordinate<int> origin;
Coordinate<int> integer(3);
Coordinate<int> copied(integer);       // 普通拷贝构造
Coordinate<double> real(integer);     // 构造函数模板，U = int
```

`Coordinate(const Coordinate&)` 中的类名在类内代表当前 `Coordinate<T>`。构造函数模板不等于拷贝构造函数，也不会替代所有特殊成员函数规则；本例显式保留同类型复制与移动。跨类型构造是 `explicit`，避免无意的隐式转换。`is_convertible` 只检查转换是否合法，不能保证所有数值转换无精度损失。

不同的 `Coordinate<U>` 与 `Coordinate<T>` 是不同类的特化，不能当然访问彼此的私有数据；本例通过公开 `get()` 获取值。若设计确实需要，可显式声明模板友元。过于宽泛的 `U&&` 转发构造函数可能抢走原本预期的复制调用，应对适用类型进行约束。

#### 类模板的文件组织

模板定义是生成具体实体的规则；实例化则把指定参数代入这些规则。隐式实例化由需要具体实体的使用触发，并不是定义模板时就为所有可能类型生成代码。类模板实例化也不等于立即实例化每个成员函数体：未使用的成员可以尚未被要求实例化。

**为什么模板实现通常放在头文件？** 使用方一般需要看到定义才能隐式实例化。只有声明可见、实现藏在某个 `.cpp` 中时，使用方可能成功编译却在链接时找不到需要的具体版本。常见做法是把定义写在 `.hpp`，或放入 `.tpp` 再由头文件包含；`.tpp` 只是文件组织惯例，没有特殊语言含义。

类模板的成员函数若在类外定义，同样应把定义保存在调用方可见的头文件或被其包含的 `.tpp` 中。不要把 `.tpp` 当作普通 `.cpp` 单独编译后就期待任意类型都能链接。

#### 编译与链接 {#template-instantiation}

常见构建过程是源文件 `.cpp` 经预处理和编译生成目标文件，再由链接器组合成可执行文件。目标文件扩展名依工具链不同可为 `.o` 或 `.obj`。每个包含头文件的源文件形成独立翻译单元。

只有模板声明时，某些调用能通过编译；如果链接阶段找不到所需实例化的定义，仍会失败。模板并没有绕过普通的编译、符号定义与链接流程。

显式实例化可以集中生成有限几种版本，减少重复编译。下面是**两个文件**的布局示意：

```text
// numeric.hpp
template<class T>
T square(T value);                 // 对使用方只公开声明
extern template int square<int>(int); // 显式实例化声明

// numeric.cpp
#include "numeric.hpp"
template<class T>
T square(T value) { return value * value; }
template int square<int>(int);     // 显式实例化定义：此处生成 int 版本
```

其他文件包含头文件即可调用 `square(3)`；若要使用 double，而程序未提供其定义或实例化，可能出现链接失败。`extern template` 用来抑制相应的隐式实例化，不是把普通声明变成可支持任意类型的外部函数。显式实例化也不是特化：它使用模板既有规则生成代码；特化则给特定参数另写规则。

ODR（One Definition Rule，单一定义规则）允许满足条件的模板定义出现在多个翻译单元，但要求定义等保持一致，不应靠宏让同一个模板在不同文件中变成不同实现。**模板定义放头文件不等于所有相关定义都自动免疫重复定义错误**：例如函数模板的全特化定义放头文件时，通常要显式加 `inline`，或把定义集中放到一个 `.cpp` 中；`inline` 在这里处理多重定义，并不保证机器代码一定内联。

模板中的函数局部静态变量通常按具体实例化分别存在，例如 `counter<int>()` 与 `counter<double>()` 的静态计数器不同；同一实例化不会仅因为被多个文件包含就理应变成每个文件一份。若声明为内部链接实体，则还要考虑链接范围。C++17 的 `inline static` 数据成员可以简化类模板静态成员的头文件定义。

#### 全特化与偏特化 {#template-specialization}

**主模板**给出通用规则；**全特化**为一组完全指定的参数提供实现；**偏特化**为一类参数形状提供实现，例如所有指针。

```cpp
template<class T>
struct TypeKind {
    static constexpr int value = 0; // 主模板
};

template<>
struct TypeKind<bool> {
    static constexpr int value = 1; // 全特化
};

template<class T>
struct TypeKind<T*> {
    static constexpr int value = 2; // 偏特化：所有指针类型
};

static_assert(TypeKind<int>::value == 0);
static_assert(TypeKind<bool>::value == 1);
static_assert(TypeKind<double*>::value == 2);
```

特化不是继承：特化类不会自动获得主模板的全部成员，需要自己提供接口。多个偏特化同时匹配时要能选出更特定的版本，否则会产生歧义，例如“第一参数是 int”和“第二参数是 double”对 `<int, double>` 可能都匹配。

| 模板类别 | 全特化 | 偏特化 | 常见替代方式 |
| --- | --- | --- | --- |
| 类模板 | 支持 | 支持 | 策略参数、约束 |
| 函数模板 | 支持 | **不支持** | 函数重载、`if constexpr`、Concepts |
| 变量模板 | 支持 | 支持 | 类型萃取 |
| 别名模板 | **不能直接特化** | **不能直接特化** | 特化底层类模板，再取其成员类型 |
| Concept（C++20） | 不支持 | 不支持 | 定义或组合新的 Concept |

显式特化必须在会触发相关隐式实例化的首次使用之前可见，并遵守相应作用域规则；不能先使用通用实现，再在同一程序里悄悄换成特化。也不能随意特化标准库模板：只有标准明确允许的情况才能在 `std` 中提供特化。

#### CTAD：类模板实参推导（C++17） {#template-ctad}

函数模板能从调用参数推导；C++17 起，某些创建类模板对象的声明也能从初始化参数推导模板实参，称为 CTAD（Class Template Argument Deduction）。编译器从构造函数等生成推导候选，也可使用显式推导指南。

```cpp
#include <type_traits>
#include <utility>
#include <vector>

std::pair pair(1, 2.5); // std::pair<int, double>
std::vector values{1, 2, 3}; // std::vector<int>

template<class T>
struct Wrap {
    T value;
    explicit Wrap(T input) : value(std::move(input)) {}
};

template<class T>
Wrap(T) -> Wrap<T>; // 用户定义推导指南；本例构造函数也能提供隐式指南

Wrap wrapped(42);
static_assert(std::is_same_v<decltype(wrapped), Wrap<int>>);
```

推导指南没有函数体，不负责构造对象；它只决定得到哪个模板类型，接着仍需用真正的构造函数初始化。一般也不能只显式写一部分类模板实参，再期望 CTAD 自动补上剩余部分。C++20 扩展了聚合类型与别名模板等相关推导能力。

注意初始化形式会影响结果：`std::vector values{3, 7}` 是两个元素 3、7，而 `std::vector<int> values(3, 7)` 是三个值为 7 的元素。`std::vector values;` 没有足够信息推导元素类型，仍然不合法；类模板名称也不能因此在所有类型位置省略实参。

#### 依赖名称：`typename`、`template` 与 `this->` {#template-dependent-names}

模板中的某些名称依赖参数，定义模板时还无法知道它代表类型、变量还是成员模板。C++ 因而采用两阶段名称查找的规则：非依赖名称一般在定义处解析，依赖名称在实例化时结合具体参数解析；实例化时的查找也受可见性与 ADL 等规则限制，不是任意“再扫一遍所有代码”。

```cpp
#include <vector>

template<class Container>
typename Container::value_type firstValue(const Container& c) {
    // typename 告诉解析器：这个依赖的限定名是类型
    return c.at(0);
}

template<class Converter>
int asInteger(const Converter& converter) {
    return converter.template convert<int>(3.5);
    // template 告诉解析器：convert<int> 是成员模板调用
}

int first = firstValue(std::vector<int>{7, 8});
```

`typename` 在这里与声明模板参数时的用途不同；不是每个依赖名称前都能加 `typename`，已经确定为类型或不是类型的位置有不同规则。C++20 在部分上下文放宽了省略要求，学习 C++17 时保留上述明确写法更易理解。

当类模板继承一个依赖的基类时，裸写成员名通常不会在定义阶段搜索该基类：

```cpp
template<class T>
struct Base {
    void reset() {}
};

template<class T>
struct Derived : Base<T> {
    void clear() {
        this->reset(); // 也可用 using Base<T>::reset 再调用
    }
};
```

也能写 `Base<T>::reset()`，但显式限定调用对于虚函数会绕过虚分派；`this->reset()` 保留通常的虚调用语义。模板与用户类型的非成员操作常利用 ADL，例如先 `using std::swap;` 再调用 `swap(a, b)`，让用户类型提供的同命名空间重载有机会被找到。

### Introducing Constraints（模板约束） {#template-constraints}

#### SFINAE 与检测惯用法 {#template-sfinae}

SFINAE 是 **Substitution Failure Is Not An Error**：函数模板参与候选选择等规定场景时，如果将实参代入后的直接上下文无效，可以排除这个候选，而不是立即让整个编译失败。这是一条模板选择规则，不是运行时异常处理。

```cpp
#include <type_traits>

template<class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
T integerTwice(T value) {
    return value + value;
}

int doubled = integerTwice(3); // 候选有效，结果 6
// integerTwice(2.5);         // 候选被排除；没有其他重载则调用仍然报错
```

`enable_if_t<true, int>` 是 `int`，条件为 false 时没有可用类型。这里把它放在模板参数列表，使条件影响候选选择；若只是函数体里写一个不合法类型，通常就是实例化错误，并不会得到同样的 SFINAE 效果。由实例化其他类等副作用引起的错误也未必属于可安全忽略的直接上下文。

检测一个类型是否有 `.size()`，可用 C++17 的 `void_t`：

```cpp
#include <string>
#include <type_traits>
#include <utility>

template<class T, class = void>
struct HasSize : std::false_type {};

template<class T>
struct HasSize<T, std::void_t<decltype(std::declval<const T&>().size())>>
    : std::true_type {};

static_assert(HasSize<std::string>::value);
static_assert(!HasSize<int>::value);
```

`declval<T>()` 让未求值上下文能假想一个指定类型的表达式，不需要真正构造对象，也不能在运行时调用它。`.size()` 合法时，`void_t<...>` 变成 `void`，偏特化匹配；不合法时这条偏特化被排除，回到主模板。这里只检查表达式可用性，不验证 `.size()` 的复杂度或语义；C++20 可用 `requires` 更直接表达这种检测。

除数字分类外，也可用 `is_base_of_v` 检查继承关系，并让 `enable_if_t` 约束模板候选：

```cpp
#include <type_traits>

struct Serializable { virtual ~Serializable() = default; };
struct Record : Serializable {};

template<class T,
         std::enable_if_t<std::is_base_of_v<Serializable, T> &&
                          std::is_convertible_v<const T*, const Serializable*>, int> = 0>
const Serializable& asSerializable(const T& object) { return object; }

Record record;
const Serializable& view = asSerializable(record);
```

`is_base_of_v` 对私有继承、歧义继承也可能为 true，不能单独证明外部可做基类转换；本例再检查指针可转换性。返回的是原对象的引用，不能超过其生命周期。SFINAE 约束描述的是候选能否成立，不会自动解决继承设计或对象所有权。

#### Concepts、约束与 `requires`（C++20） {#template-concepts}

Concept 是命名的编译期约束，描述一个类型应满足哪些条件。它比在函数体深处报出模板错误更早地表达接口要求，也可以参与重载选择；它不创建接口对象，不要求继承。

```cpp
#include <concepts>

template<class T>
requires std::integral<T>
constexpr T twice(T value) {
    return value + value;
}

// 等价的简写：template<std::integral T>
static_assert(twice(3) == 6);
// twice(1.5); // double 不满足 integral，候选不可用

template<class T>
concept Addable = requires(const T& a, const T& b) {
    { a + b } -> std::convertible_to<T>;
};

template<Addable T>
T add(const T& a, const T& b) {
    return a + b;
}

static_assert(Addable<int>);
```

`requires` 有两种要区分的用途：**requires 子句**给声明附加约束，例如 `requires Addable<T>`；**requires 表达式**用来检查一组类型/表达式要求，例如上面 Concept 右侧的 `requires(...) { ... }`，结果是编译期布尔值。

| requires 表达式中的要求 | 示例 | 检查内容 |
| --- | --- | --- |
| 简单要求 | `a.size();` | 表达式是否有效，不执行它 |
| 类型要求 | `typename T::value_type;` | 所命名的类型是否存在 |
| 复合要求 | `{ a.size() } noexcept -> std::same_as<std::size_t>;` | 表达式、异常说明、结果类型约束 |
| 嵌套要求 | `requires std::integral<typename T::value_type>;` | 另一个约束是否满足 |

需要多个条件时可以用 `&&`、`||` 组合。约束依赖模板参数时，requires 表达式里的某些无效要求会使结果为 false；它不是在任意非模板上下文中屏蔽编译错误的万能方法。

`std::integral` 包括 `bool`；若业务只允许非 bool 整数，应显式排除它。`Addable` 只检查表达式及转换条件，仍不能自动保证加法的数学性质；数值溢出、分配失败等运行时问题也不会因此消失。

约束更强的重载有时可优先选择，但编译器使用**约束归一化、原子约束同一性与包含关系**等规则，不是一般数学定理证明器。把公共条件定义成同一个 Concept 再组合，通常比在多个重载里重复写相似表达式更易得到预期的约束偏序。别把重载条件、`static_assert` 与 `if constexpr` 混成同一个机制。

Hashable 可以检查类型是否支持需要的哈希操作；下面是补充的 C++20 示例：

```cpp
#include <concepts>
#include <cstddef>
#include <functional>
#include <string>

template<class T>
concept Hashable = requires(const T& value) {
    { std::hash<T>{}(value) } -> std::convertible_to<std::size_t>;
};

template<Hashable T>
std::size_t hashValue(const T& value) { return std::hash<T>{}(value); }

static_assert(Hashable<int>);
static_assert(Hashable<std::string>);
struct Unhashed {};
static_assert(!Hashable<Unhashed>);
```

此 Concept 检查 `std::hash<T>` 是否能用于该表达式；它不检验“相等对象必须得到相同哈希”等语义要求，也不足以单独保证一个类型可作为无序容器的键。键还需要合适的相等比较；无序容器也可采用自定义哈希器，未满足这里的 `Hashable` 不代表所有配置都不可用。

#### `if constexpr` 与标签分派 {#template-if-constexpr}

SFINAE/约束控制“哪些函数可被选中”；`if constexpr`（C++17）控制“已选中的模板实现使用哪条分支”。普通 `if` 两边都必须满足编译要求，条件即使是常量也不能替代这种模板机制。

```cpp
#include <type_traits>

template<class T>
auto readValue(T value) {
    if constexpr (std::is_pointer_v<T>) {
        return *value;
    } else {
        return value;
    }
}

int number = 7;
int a = readValue(number);  // int 版本不实例化解引用分支
int b = readValue(&number); // 指针版本取出所指值，要求指针有效
```

在模板实例化中，条件已确定时，被丢弃的分支不会按该实例继续实例化，但仍需能被解析；非依赖名称等错误不应指望被隐藏。在非模板代码里写 `if constexpr(false)` 也不能把任意不合法代码变成合法代码。

这段代码的普通 `auto` 返回值按值推导；若目标是保留引用，需重新设计返回表达式和 `decltype(auto)`，同时确保对象仍存活。选择一个分支，不等于该分支的运行时前提已经满足，例如空指针仍需处理。

在 C++17 之前，可用**标签分派**：让一个统一入口根据 `std::true_type` / `std::false_type` 等标签调用两个不同重载。它把不同实现拆成函数；`if constexpr` 则常把它们集中在一个模板里。类型标签还可以表达优先级和类别，不限于布尔判断。

#### 类型萃取与编译期计算 {#template-traits}

类型萃取（type traits）把类型性质表示成编译期值或另一个类型；标准库 `<type_traits>` 提供大量现成工具。`_v` 通常是布尔/数值变量模板简写（C++17），`_t` 通常是成员类型别名简写（多见于 C++14）。

```cpp
#include <type_traits>

static_assert(std::is_integral_v<int>);
static_assert(!std::is_integral_v<double>);
static_assert(std::is_same_v<std::remove_const_t<const int>, int>);

using Raw = std::remove_cv_t<std::remove_reference_t<const int&>>;
static_assert(std::is_same_v<Raw, int>);

template<class T>
struct IsPointer : std::false_type {};

template<class T>
struct IsPointer<T*> : std::true_type {};

static_assert(IsPointer<int*>::value);
```

上例主模板与偏特化构成一个简单类型判断器；生产代码可直接使用 `std::is_pointer`。常用工具还包括 `is_same`、`is_constructible`、`is_invocable`（C++17）、`conditional_t`、`common_type_t`、`remove_reference_t` 与 `decay_t`。

`decay_t` 不只是去掉引用/限定，还把数组与函数变成指针；C++20 的 `remove_cvref_t` 则只去掉引用与顶层 cv 限定。选择错误的变换可能丢掉数组长度或接口需要的引用性质。`static_assert` 用于编译时验证，而 `constexpr` 函数是否在编译期执行还取决于调用上下文；“模板参数编译时确定”不代表模板函数体一定在编译期运行。

### Interesting Features（模板的其他功能） {#template-features}

#### 参数包与折叠表达式 {#template-packs}

参数包（parameter pack）表示零个或多个模板参数；函数参数包则表示零个或多个函数参数。`...` 的展开把某个模式应用到包内每一个元素，`sizeof...(Ts)` 得到参数个数。

```cpp
#include <iostream>

template<class... Ts>
void printAll(const Ts&... args) {
    ((std::cout << args << ' '), ...); // C++17：对逗号运算符进行折叠
    std::cout << '\n';
}

template<class... Ts>
auto sum(Ts... args) {
    return (0 + ... + args); // 有初始值的左折叠：((0 + a) + b) + c
}
```

`printAll(1, "ready", 2.5)` 可接收不同类型，只要求各参数支持输出。这里使用逗号折叠，输出按从左到右的顺序进行；不能据此推断所有包展开都具有同样的求值顺序。

| 折叠形式 | 三个参数时的括号结构 |
| --- | --- |
| `(... op pack)` | `(a op b) op c` |
| `(pack op ...)` | `a op (b op c)` |
| `(init op ... op pack)` | `((init op a) op b) op c` |
| `(pack op ... op init)` | `a op (b op (c op init))` |

折叠方向对减法等非结合运算很重要。没有初始值的一元折叠，仅 `&&`、`||`、逗号允许空包，结果分别为 `true`、`false`、`void()`；加法等空包需自行提供初始值。`sum()` 的结果为 `0`，但初始值 `0` 也会影响类型和重载，不能直接用于所有可相加的对象。

在 C++11/14 中，参数包也能配合递归重载展开；C++17 折叠表达式简化了很多这种写法。使用 `std::index_sequence` / `std::make_index_sequence`（C++14）还能生成编译期索引包，用于按下标展开 tuple 等固定结构；C++17 的 `std::apply` 则直接把 tuple 中的元素作为函数参数展开。

#### 其他模板特性

##### tuple、get 与 make_tuple

`std::tuple` 用可变参数类模板保存不同类型的元素；`std::get<I>` 通过编译期索引访问，`std::make_tuple` 便于推导元素类型：

```cpp
#include <functional>
#include <string>
#include <tuple>
#include <type_traits>

int count = 3;
auto record = std::make_tuple(count, std::string("ready"));
static_assert(std::is_same_v<decltype(record), std::tuple<int, std::string>>);
std::get<0>(record) = 10; // 修改 tuple 内的副本，原 count 仍为 3

auto references = std::make_tuple(std::ref(count));
std::get<0>(references) = 12; // std::ref 被展开为引用，修改原 count
```

普通 `make_tuple` 对实参做类似 `decay` 的类型处理并保存值；`std::ref` / `std::cref` 是明确保存引用的例外。`std::tie` 也可形成左值引用 tuple；这些引用仍不能超过原对象的生命周期。按类型写 `std::get<T>`（C++14）要求该类型在 tuple 中恰好出现一次；运行时变化的整数不能直接作为 `get<I>` 的模板实参。

C++17 可用结构化绑定拆开 tuple；`std::apply` 将 tuple 的元素展开给可调用对象，与前面的参数包机制对应。

##### 别名模板与变量模板

别名模板给复杂类型起名字，不创建一个具有新身份的类型：

```cpp
#include <cstddef>
#include <vector>

template<class T>
using Vec = std::vector<T>; // C++11

template<class T>
inline constexpr std::size_t objectBytes = sizeof(T); // 变量模板 C++14；inline C++17

Vec<int> values;
static_assert(objectBytes<int> == sizeof(int));
```

`Vec<int>` 就是 `std::vector<int>`。变量模板则定义一族变量，例如 `objectBytes<int>`、`objectBytes<double>`；可用 `constexpr` 保存编译期计算结果。C++17 的 `inline` 变量便于把同一个定义放在被多个翻译单元包含的头文件中。

##### 模板模板参数

模板模板参数表示“传入一种容器模板”，再由内部补上元素类型：

```cpp
#include <deque>
#include <vector>

template<class T, template<class...> class Container = std::vector>
struct Collection {
    Container<T> values;
};

Collection<int> vectorCollection;
Collection<int, std::deque> dequeCollection;
```

这里要求传入模板能用 `Container<T>` 构造有效类型；`std::array` 还需要容量参数，不能按这个接口直接替换。参数形状兼容与操作语义兼容是两件事：如果算法使用下标操作，所选容器还必须提供相应操作。

##### extern template

`extern template` 是显式实例化声明，用来抑制相应的隐式实例化；程序仍应在其他翻译单元提供需要的显式实例化定义。它通常用于集中生成有限类型的版本，不是让任意模板实现都能隐藏在 `.cpp` 中的通用开关。完整的多文件布局见[编译与链接](#template-instantiation)。

##### 转发引用、引用折叠与完美转发 {#template-forwarding}

包装函数需要把参数交给另一个函数。如果把所有参数按值接收，会丢掉引用并可能复制；如果一律 `std::move`，又会把调用者的左值当成可移动来源。完美转发的目标是保持调用者传入时的值类别与相关限定。

```cpp
#include <utility>

template<class F, class... Args>
decltype(auto) relay(F&& function, Args&&... args) {
    return std::forward<F>(function)(std::forward<Args>(args)...);
}
```

这里 `F`、`Args` 从实参推导，所以 `F&&`、`Args&&` 是转发引用。传入 `int` 左值时，`Args` 可推导为 `int&`，`Args&&` 经折叠得到 `int&`；传入右值时，`Args` 推导为 `int`，形参为 `int&&`。

| 组合 | 折叠结果 |
| --- | --- |
| `T& &`、`T& &&`、`T&& &` | `T&` |
| `T&& &&` | `T&&` |

引用折叠通过模板替换或类型别名产生，不能直接在普通声明中把两个 `&` 类型拼写出来。形参有名字后，它在函数体里的表达式是左值；`std::forward<T>` 根据推导出的 `T` 恢复应有的转发形式。`std::move` 则无条件把通常的对象表达式转换为将亡值，两者不能互换。

`const T&&` 不是上述转发引用。`class Box<T>` 的普通成员参数 `T&&` 若 `T` 已由类的实参固定，也不是转发引用；需要成员自身的 `template<class U> ... (U&&)` 才能进行这一推导。

此处 `decltype(auto)` 保留被调用函数的引用返回值；若包装器改成普通 `auto`，可能复制结果。这个最小包装器支持普通可调用对象；成员函数指针等形式可改用 `std::invoke`（C++17）。还要考虑返回引用的生命周期、`noexcept` 传播，且不要多次转发同一个可移动对象后继续假定它保留原资源。花括号列表没有普通表达式类型，裸 `{1, 2}` 也不能直接供这种 `Args&&` 推导。

##### 泛型 Lambda 与缩写函数模板

C++14 泛型 Lambda 可用 `auto` 形参，它的调用运算符本质上是成员函数模板：

```cpp
auto multiply = [](auto a, auto b) { return a * b; };
int integer = multiply(2, 3);
double real = multiply(2.5, 4.0);
```

两个独立的 `auto` 对应两个独立类型参数，不要求 `a`、`b` 同类型。想让参数共享一个 `T`，C++20 可以显式声明 Lambda 模板参数：

```cpp
auto sameTypeAdd = []<class T>(T a, T b) { return a + b; }; // C++20
int total = sameTypeAdd(2, 3);
// sameTypeAdd(2, 3.5); // 同一个 T 推导冲突

auto addAny(auto a, auto b) { return a + b; } // C++20：缩写函数模板
```

普通 `auto` 返回类型推导从 C++14 就存在，而在普通函数参数列表里用 `auto` 是 C++20 的缩写函数模板语法，两者不是同一版本的功能。Lambda 的 `auto&&` 形参也可用于转发，仍要在函数体里配合正确的 `std::forward`；不要仅因出现 `&&` 就认定已经完成完美转发。

##### 模板元编程、策略类与 CRTP

模板元编程用模板选择与编译期计算产生类型或值。类型萃取、参数包与条件类型都属于常见工具；能用清晰的 `constexpr` 函数计算一个数值时，通常不用为了数值运算建立很深的递归类模板。

**策略类（policy）**把某种可替换行为作为模板参数：例如日志器的输出后端、容器的分配器、排序算法的比较器。选择在编译时完成，常可内联，但每种策略组合会产生新的类型；若必须在运行时切换，应考虑虚接口、函数对象或其他组合方案。

CRTP（Curiously Recurring Template Pattern）让派生类把自己作为基类模板的参数，从而获得静态分派：

```cpp
template<class Derived>
class Runnable {
public:
    void run() {
        static_cast<Derived&>(*this).runImpl();
    }
};

class Job : public Runnable<Job> {
public:
    void runImpl() { /* 执行具体任务 */ }
};
```

`Runnable<Job>` 在编译时知道 `Job`，不需要借助虚函数来决定 `runImpl`。代价是不同派生类对应不同基类实例化，不天然形成同一种可在运行时异构存储的基类接口。这个转换要求实际派生对象与参数吻合；不要独立构造一个 `Runnable<Job>` 后调用 `run()`。基类构造/析构阶段也不能借此安全访问尚未构造或已销毁的派生状态。

在 C++20 中，Concepts 可约束策略接口；在 C++23 中，显式对象参数（常称 deducing this）还能简化部分依赖 CRTP 的成员实现，但需要相应语言与编译器支持。这些技术是实现选择，不是每个类都需要采用的结构。

#### 返回类型推导 {#template-return-deduction}

函数模板的结果类型不一定与某个输入类型相同。可以使用 `auto`、`decltype`、`decltype(auto)` 或尾置返回类型描述，选择取决于是否需要保留引用。

```cpp
#include <type_traits>

template<class X, class Y>
auto addTrailing(X x, Y y) -> decltype(x + y) { // 尾置返回类型，C++11
    return x + y;
}

template<class X, class Y>
auto addInferred(X x, Y y) { return x + y; } // 从函数体推导，C++14

template<class T>
auto readCopy(T& value) { return value; } // 普通 auto，按值返回

template<class T>
decltype(auto) readReference(T& value) { return (value); } // C++14，保留引用

int value = 3;
static_assert(std::is_same_v<decltype(addTrailing(1, 2.5)), double>);
static_assert(std::is_same_v<decltype(readCopy(value)), int>);
static_assert(std::is_same_v<decltype(readReference(value)), int&>);
```

尾置返回类型在参数列表之后，因而能引用参数名 `x`、`y`；`decltype(x + y)` 根据表达式确定类型，表达式本身不在此执行。普通 `auto` 返回推导通常去掉顶层限定与引用；`decltype(auto)` 按 `decltype` 规则确定结果，括号以及表达式值类别因此会影响结果。

本例 `value` 是声明为 `T&` 的参数，所以 `decltype(value)` 本身也是 `T&`；括号的差异在按值形参上更明显：

```cpp
template<class T>
decltype(auto) copyValue(T value) { return value; } // decltype(value) = T，按值返回
// 若写 return (value)，会得到 T&，引用却指向即将销毁的局部形参。
```

不要为保留引用而引用局部变量、按值形参或已销毁的临时对象。`auto` 推导返回类型的定义一般需要在使用前可见；尾置返回类型中的替换可用于候选判断，而仅在函数体里失败的返回推导通常不能当作安全的 SFINAE 手段。前面的类型推导章节给出了完整的 `decltype` 与 `auto` 规则。

#### 性能、错误定位与常见陷阱

模板能把类型选择提前，减少某些运行时间接调用，并给优化器更多信息；但不是“用了模板就一定更快”。代价包括编译时间增加、错误信息变长，以及许多实例化带来的代码体积增长。链接器可能合并部分机器代码，但不能依赖它消除所有膨胀。

遇到错误时，从报错中最接近自己调用位置的实例化链入手：确认具体模板实参、哪个候选被排除、哪种操作不合法；必要时在局部使用 `static_assert` 验证推导结果，再检查根因，而不是先改标准库里的报错行。

| 容易误解的说法 | 更准确的规则 |
| --- | --- |
| 模板可以接收任何类型 | 实例化所需操作、约束和运行时前提都必须满足 |
| 模板函数在编译期运行 | 模板参数选择发生在编译时，函数仍可在运行时执行 |
| `T&&` 总是右值引用 | 可推导、无 cv 限定的函数模板类型参数可能构成转发引用 |
| 任何替换错误都是 SFINAE | 适用场景与直接上下文有范围，函数体等错误通常仍会失败 |
| `if constexpr` 隐藏任何错误 | 丢弃实例化分支不等于不解析、不检查所有非依赖问题 |
| 函数模板能偏特化 | 应使用重载或其他选择机制 |
| 加上 Concept 就证明语义正确 | 编译器可检查语法/类型条件，许多语义要求仍靠设计保证 |
| 全特化自动继承通用实现 | 特化是另一个实现，要自己保持所需接口 |
| 模板自动避免悬空引用 | 所有权、对象生命周期和返回值类别仍需明确 |
| 模板必须全部写在头文件 | 通用隐式实例化通常如此，有限版本可集中显式实例化 |

#### 如何选择模板形式 {#template-selection}

先判断变化发生在整个类型，还是某个函数的输入；两者也可以同时存在。

| 需要变化的部分 | 常见选择 | 判断依据 |
| --- | --- | --- |
| 一个独立算法的输入类型 | 函数模板 | 算法可复用，但不需要为它创建一族对象类型 |
| 类的存储类型或接口类型随参数变化 | 类模板 | 例如 `Box<int>` 与 `Box<std::string>` 的成员类型不同；不要求所有成员都随参数变化 |
| 类的存储类型固定，只有某个函数需要接收不同类型 | 普通类中的成员函数模板 | 例如同一个 `Printer` 对象可以输出整数与字符串，无须创建 `Printer<int>` |
| 类的存储类型需要变化，构造时还要接收另一种类型 | 类模板与构造函数模板组合 | 外层 `T` 决定保存的类型，内层 `U` 决定这次构造接收的类型；如 `Coordinate<double>` 从 `Coordinate<int>` 构造 |

这是一种选型思路，不是硬性规定：已知只有少量输入类型时，普通重载可能更直接；需要运行时切换不同实现时，还应考虑虚接口或类型擦除。构造函数模板只负责允许的构造方式，不会自动提供跨类型赋值，也不会替代同类型拷贝构造函数。

#### 综合示例：固定容量缓冲区（C++17） {#template-example}

这个可独立编译的程序把类型参数、非类型参数、成员模板、完美转发、折叠表达式和编译时约束放在同一个小场景中。容量是编译期配置；添加几条记录和容量检查仍在运行时发生。

```cpp
#include <array>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

template<class T, std::size_t Capacity>
class FixedBuffer {
    static_assert(Capacity > 0, "capacity must be positive");
    static_assert(std::is_default_constructible_v<T>, "slots require default construction");
    std::array<T, Capacity> slots_{};
    std::size_t size_ = 0;
public:
    template<class U,
             std::enable_if_t<std::is_assignable_v<T&, U&&>, int> = 0>
    void push(U&& value) {
        if (size_ == Capacity) throw std::length_error("buffer full");
        slots_[size_] = std::forward<U>(value);
        ++size_; // 只有赋值成功后才增加逻辑大小
    }

    template<class... Us>
    void pushAll(Us&&... values) {
        (push(std::forward<Us>(values)), ...);
    }

    std::size_t size() const { return size_; }
    static constexpr std::size_t capacity() { return Capacity; }

    const T& at(std::size_t index) const {
        if (index >= size_) throw std::out_of_range("invalid index");
        return slots_[index];
    }
};

int main() {
    FixedBuffer<std::string, 3> buffer;
    std::string first = "alpha";
    buffer.pushAll(first, std::string("beta"), "gamma");
    static_assert(decltype(buffer)::capacity() == 3);
    for (std::size_t i = 0; i < buffer.size(); ++i)
        std::cout << buffer.at(i) << '\n';
}
```

输出为三行 `alpha`、`beta`、`gamma`。`T` 是元素类型，`Capacity` 是进入类型的容量值；`U` 在每次 push 调用中分别推导，左值字符串复制、右值字符串可移动，字面量则按字符串赋值接口处理。引用折叠与转发保持参数类别，但字符串对象真正做什么仍由它的赋值运算符决定。

这不是 `std::vector` 的替代实现：所有槽位预先默认构造，所以限制了可用类型；`pushAll` 逐项执行，若中途失败，前面成功添加的项目仍保留，不能宣称整个批次有强异常保证。若需要支持不能默认构造的类型、严格事务式批次或可变容量，应进一步设计存储与异常策略。

#### 版本速查与原始参考

| 标准 | 本节相关能力 |
| --- | --- |
| C++98/03 | 函数/类模板、特化、模板模板参数、显式实例化 |
| C++11 | 参数包、别名模板、转发引用、`std::forward`、基础 type traits |
| C++14 | 变量模板、泛型 Lambda、许多 `_t` 简写、`index_sequence` |
| C++17 | 折叠表达式、`if constexpr`、CTAD、`template<auto>`、inline 变量、`void_t`、许多 `_v` 简写 |
| C++20 | Concepts、`requires`、缩写函数模板、显式 Lambda 模板参数、扩展非类型参数与部分 CTAD 能力 |
| C++23 | 显式对象参数，可简化部分静态多态与转发成员设计 |

以下链接指向 WG21 官方发布的工作草案，可在 PDF 中搜索方括号内的稳定条款标识；草案不是付费正式标准，阅读时也要注意版本及后续缺陷修正。

- [C++17 工作草案 N4659](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf)：模板参数 `[temp.param]`、调用推导 `[temp.deduct.call]`、推导指南 `[temp.deduct.guide]`、偏特化 `[temp.class.spec]`、显式特化 `[temp.expl.spec]` 与显式实例化 `[temp.explicit]`。
- 同一份 C++17 草案还包括依赖名称 `[temp.dep]`、参数包 `[temp.variadic]`、折叠表达式 `[expr.prim.fold]`、转发辅助函数 `[forward]`、模板推导与替换 `[temp.deduct]` 和 `if constexpr` `[stmt.if]`。
- [C++20 工作草案 N4861](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/n4861.pdf)：Concept 定义 `[temp.concept]`、约束 `[temp.constr]`、requires 表达式 `[expr.prim.req]`，以及模板参数和推导规则的扩展。
- [C++23 工作草案 N4950](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf)：函数声明 `[dcl.fct]` 中的显式对象参数；本节仅介绍其用途，不将其混入 C++17 示例。


## STL：容器、迭代器与算法 {#stl}

### STL、泛型编程与 `namespace std` {#stl-overview}

STL（Standard Template Library，标准模板库）是一套以**泛型编程**为核心的数据结构与算法体系，通常用来指 C++ 标准库中的容器、迭代器、算法及相关函数对象、适配器。C++ 标准库还包含输入输出、线程、智能指针等内容，不能把整个标准库都等同于 STL。

容器（Containers）负责保存元素；迭代器（Iterators）提供统一的位置与访问方式；算法（Algorithms）通过迭代器处理一个范围。算法因而可以复用，但仍要求迭代器能力和元素操作满足条件，例如 `std::sort` 需要随机访问迭代器。

```cpp
#include <algorithm>
#include <vector>

void sortNumbers() {
    std::vector<int> values{5, 1, 3};
    std::sort(values.begin(), values.end()); // {1, 3, 5}
}
```

`std` 是标准库使用的命名空间；应包含对应头文件，再通过 `std::vector`、`std::sort` 等名称使用。局部使用 `using std::vector;` 可以减少重复限定，头文件中不宜写 `using namespace std;`，以免影响所有包含者。不能随意向 `std` 添加名称；标准允许的特定模板特化是例外。

模板、`auto`、引用、构造函数、Lambda 等是语言能力，STL 利用它们实现泛型接口。本章侧重这些能力在容器和算法中的作用；通用规则仍保留在本笔记对应章节。

### 容器分类与头文件 {#stl-containers}

**顺序容器（Sequence Containers）**按位置保存元素；“顺序”不表示元素已按值排序。

| 容器 | 头文件 | 内存布局与访问 | 主要特点 |
| --- | --- | --- | --- |
| `std::array<T, N>` | `<array>` | 固定大小、连续存储，随机访问 | 大小进入类型，无动态扩容 |
| `std::vector<T>` | `<vector>` | 动态连续存储，随机访问 | 默认优先考虑，尾部增长方便 |
| `std::deque<T>` | `<deque>` | 通常分块存储，随机访问 | 两端插入删除方便，整体不保证连续 |
| `std::list<T>` | `<list>` | 通常为双向链表，双向遍历 | 已知位置时节点插入删除快 |
| `std::forward_list<T>` | `<forward_list>` | 通常为单向链表，只向前遍历 | 节点接口以“前驱之后”为中心，无 `size()` |

**关联容器（Associative Containers）**按键组织元素。这里把有序与无序两组分开；标准中的 associative containers 通常专指有序组，哈希组称 unordered associative containers。

| 容器 | 头文件 | 保存内容 | 是否允许等价键重复 |
| --- | --- | --- | --- |
| `std::set<K>` | `<set>` | 键本身 | 不允许 |
| `std::multiset<K>` | `<set>` | 键本身 | 允许 |
| `std::map<K, T>` | `<map>` | 键值对 | 不允许 |
| `std::multimap<K, T>` | `<map>` | 键值对 | 允许 |
| `std::unordered_set<K>` | `<unordered_set>` | 键本身 | 不允许 |
| `std::unordered_multiset<K>` | `<unordered_set>` | 键本身 | 允许 |
| `std::unordered_map<K, T>` | `<unordered_map>` | 键值对 | 不允许 |
| `std::unordered_multimap<K, T>` | `<unordered_map>` | 键值对 | 允许 |

有序组按照比较器定义的顺序遍历；无序组没有按键排序的保证，但**仍然可以遍历**。`multi` 表示允许等价键，不表示一个元素自动存储多个值。

**容器适配器（Container Adapters）**基于其他容器提供受限接口，不是任意位置都能访问的普通序列。

| 适配器 | 头文件 | 规则 | 默认底层容器 |
| --- | --- | --- | --- |
| `std::stack<T>` | `<stack>` | LIFO，后进先出 | `std::deque<T>` |
| `std::queue<T>` | `<queue>` | FIFO，先进先出 | `std::deque<T>` |
| `std::priority_queue<T>` | `<queue>` | 每次访问优先级最高的元素 | `std::vector<T>` |

### 容器初始化与构造函数 {#stl-initialization}

`std::vector<T> v(n, value)` 表示创建 `n` 个元素，每个元素用 `value` 初始化；它与花括号的元素列表不同。

```cpp
#include <array>
#include <initializer_list>
#include <vector>

void initializeContainers() {
    std::vector<int> empty;             // 0 个元素
    std::vector<int> zeros(3);          // {0, 0, 0}
    std::vector<int> repeated(3, 7);     // {7, 7, 7}
    std::vector<int> listed{3, 7};       // {3, 7}
    std::vector<int> copied(listed);    // 拷贝构造，保存独立的元素
    std::vector<int> selected(listed.begin(), listed.end()); // 区间构造
    std::initializer_list<int> initial{1, 2, 3};
    std::vector<int> fromList(initial);

    std::array<int, 4> fixed{1, 2};      // {1, 2, 0, 0}
    std::array<int, 4> allZero{};        // 全部为 0
    // std::array<int, 4> uninitialized; // 局部 int 元素没有被初始化
    (void)fixed;
    (void)allZero;
}
```

统一初始化、列表初始化或花括号初始化常指 `{...}` 形式，但不能理解为“它与圆括号总是一样”。存在可用的 `std::initializer_list` 构造函数时，列表初始化通常优先考虑该构造函数；因此 `vector<int>(3, 7)` 与 `vector<int>{3, 7}` 的含义不同。

列表初始化禁止规定的**窄化转换（Narrowing Conversion）**，例如 `int n{3.5};`、`std::vector<int> v{3.5};` 不合法。某些整数转换是否窄化还取决于常量值能否表示，不能简单归结为“类型不同就不行”。

`std::initializer_list<T>` 的元素是只读的 `const T`；从列表构造容器通常需要复制元素，所以 `std::vector<std::unique_ptr<Base>>{...}` 通常无法用于逐项移动这些指针，应改用 `push_back` / `emplace_back`。

`std::vector<int> nums;` 明确指定元素类型；`std::vector nums;` 无法推导元素类型。C++17 起 `std::vector nums{1, 2, 3};` 可通过 CTAD 得到 `std::vector<int>`，但 CTAD 不会使所有省略模板实参的写法都合法。

### 常用接口：不要把某个容器的方法套到所有容器 {#stl-operations}

| 操作 | 含义与前提 | 常见支持范围 |
| --- | --- | --- |
| `size()` / `empty()` | 元素数量 / 是否为空 | 通常都有；`forward_list` 没有 `size()` |
| `clear()` | 销毁全部元素，使可变长容器为空 | 普通动态容器；`array` 和三种适配器不提供 |
| `front()` / `back()` | 首元素 / 尾元素的引用，需要非空 | 顺序容器；`forward_list` 只有 `front()` |
| `push_back()` / `pop_back()` | 尾部添加 / 删除，删除需要非空 | `vector`、`deque`、`list` |
| `push_front()` / `pop_front()` | 首部添加 / 删除，删除需要非空 | `deque`、`list`、`forward_list` |
| `insert()` / `erase()` | 插入 / 真正删除元素，重载因容器而异 | 多数动态容器；`forward_list` 使用 after 接口 |
| `at()` | 带边界检查访问，失败抛 `std::out_of_range` | `array`、`vector`、`deque`；`map` 等按键访问 |
| `operator[]` | 序列按下标访问，C++17 不检查边界 | `array`、`vector`、`deque`；映射容器含义不同 |
| `remove()` / `remove_if()` | 链表成员函数直接删除匹配节点 | `list`、`forward_list`；不是 `vector` 的成员 |
| `fill(value)` | 将所有现有元素赋为同一值，不改变大小 | `array` 成员；通用范围使用 `std::fill` |

`pop_back()`、`pop_front()` 和适配器的 `pop()` 只删除，不返回被删元素；需要值时先读取，再删除。`front()` / `back()` 也不会自动处理空容器。固定大小 `array<T, 0>` 可以存在，但不可访问其首尾元素。

序列的 `insert(position, first, last)` 在指定位置前复制源区间元素；`erase(first, last)` 删除半开区间 `[first, last)`。关联容器也可按区间插入、删除，但会依据键规则决定位置；重复键是否插入取决于容器种类。输入范围、目标位置必须有效，不能随意将同一容器的重叠区间当作插入来源。

```cpp
#include <algorithm>
#include <vector>

void insertAndEraseRange() {
    std::vector<int> values{1, 4};
    std::vector<int> extra{2, 3};
    values.insert(values.begin() + 1, extra.begin(), extra.end()); // {1, 2, 3, 4}
    values.erase(values.begin() + 1, values.begin() + 3);          // {1, 4}
    std::fill(values.begin(), values.end(), 9);                   // {9, 9}
}
```

### 顺序容器的结构与使用 {#stl-sequence}

#### `std::array`：固定大小与连续存储

`std::array<T, N>` 保存恰好 `N` 个元素，`size()` 总为 `N`。它没有 `push_back`、`erase`、`clear` 或动态扩容接口，但能遍历、排序、填充。固定容量不表示一定存放在栈上：对象可以是局部变量、类成员，也可以由动态分配获得，存储位置取决于对象本身。

```cpp
#include <array>
#include <algorithm>

void useArray() {
    std::array<int, 3> values{3, 1, 2};
    std::sort(values.begin(), values.end()); // {1, 2, 3}
    values.fill(7);                         // {7, 7, 7}
    values.at(0) = 5;                       // 带边界检查
}
```

#### `std::vector`：size、capacity 与动态扩容 {#stl-vector-memory}

普通 `vector<T>` 的现有元素连续存储，可用 `data()` 获取首地址；`std::vector<bool>` 是特殊的位压缩特化，不能照搬普通元素引用与连续 `bool` 数组的假设。

`size()` 是已构造的元素数量，`capacity()` 是当前分配空间可容纳、无需重新分配的元素数量。只有 `[0, size())` 内的元素能被访问；容量中的空闲空间不是已经存在的元素。

```cpp
#include <vector>

void manageVectorStorage() {
    std::vector<int> values;
    values.reserve(8);       // capacity >= 8，size 仍为 0
    // values[0] = 1;       // 错误：reserve 没有创建元素
    values.push_back(1);     // size = 1
    values.resize(3);        // {1, 0, 0}，创建两个新元素
    values.resize(1);        // 销毁后两个元素，不缩小 capacity
    values.clear();          // size = 0，capacity 不变
    values.shrink_to_fit();  // 非强制请求；实现可以不缩小容量
}
```

当添加元素所需空间超过容量时，会发生 **Reallocation（重新分配）**：获取新存储、构造或转移元素、销毁旧存储中的元素并释放原内存。所有指向旧存储的迭代器、指针和引用因此失效。扩容通常采用几何增长，但标准没有指定固定的 1.5 倍或 2 倍比例；尾部插入具有摊还 `O(1)` 的保证，单次扩容仍可能是 `O(n)`。

预先知道数量时，`reserve(n)` 能减少扩容；不宜每插入一个元素都调用 `reserve(size() + 1)`，这种做法可能破坏高效增长策略。`reserve()` 只在请求超过现有容量时重新分配，不是缩容操作；`resize()` 才改变元素数量。

#### 容器复制、移动与 `noexcept`

| 操作 | 例子 | 主要含义 |
| --- | --- | --- |
| 拷贝构造（Copy Constructor） | `auto b = a;` | 创建新容器，复制元素 |
| 移动构造（Move Constructor） | `auto b = std::move(a);` | 创建新容器，利用源对象资源或移动元素 |
| 拷贝赋值 | `b = a;` | 用源内容替换既有目标内容，可能复用目标存储 |
| 移动赋值 | `b = std::move(a);` | 替换既有目标内容，资源接管受分配器条件影响 |

普通 `vector` 的不带额外分配器的移动构造可以接管存储；带分配器的构造或某些移动赋值可能仍需逐元素移动，不能断言所有移动都为 `O(1)`。标准库对象移动后通常处于**有效但未指定的状态**：能析构，能执行满足前提的操作；不应假定它保留原内容或一定为空。

`std::move` 只是转换表达式值类别，不直接转移资源。从 `const` 对象 `std::move` 通常不能匹配普通的非 const 移动构造，可能仍复制。

扩容时如何转移元素还取决于 `T`：有不抛异常的移动构造时适合移动；若移动可能抛异常且元素可复制，实现常采用复制来维护异常保证。对于不可复制且移动可能抛异常的类型，某些操作的异常保证会减弱。应在确实不抛异常时把移动构造标记为 `noexcept`，不能为了“让 vector 更快”而作虚假承诺。

容器按值保存 `T`，并负责销毁它保存的对象；`vector<T*>` 保存的是裸地址，删除或销毁容器不会自动 `delete` 指针所指对象。通用拷贝、移动及资源管理规则见本笔记的构造函数与移动语义章节。

#### `std::deque`：分块存储与两端操作

`deque` 通常使用多个存储块与块索引，提供 `O(1)` 随机访问，却不保证元素形成一个连续数组。不能像 `vector` 那样把首元素地址加下标当作通用访问方式。两端单元素插入、删除为 `O(1)`；中间插入、删除仍需要移动元素，通常为线性复杂度。

```cpp
#include <deque>

void useDeque() {
    std::deque<int> values{2, 3};
    values.push_front(1);
    values.push_back(4);   // {1, 2, 3, 4}
    values.pop_front();
    values.pop_back();     // {2, 3}
    values.at(0) = 9;      // {9, 3}
}
```

#### `std::list`：双向链表

`list` 通常每个节点保存值、前驱与后继，节点不连续。已取得有效位置迭代器时，单元素插入与删除为 `O(1)`；**寻找位置**仍可能需要 `O(n)`，因此不应笼统说“链表中间插入总比 vector 快”。额外指针、逐节点分配和缓存局部性也会影响实际性能。

`list` 不提供 `operator[]` 或 `at()`；它的迭代器不能写 `it + 3`。可以用 `std::next(it, 3)` 顺序移动。排序应使用 `list::sort()`，而非要求随机访问的 `std::sort`。

```cpp
#include <list>
#include <iterator>

void useList() {
    std::list<int> values{3, 1, 2, 2};
    auto position = std::next(values.begin());
    values.insert(position, 4); // 插在原 1 前面
    values.remove(2);           // 真正删除所有值为 2 的节点
    values.remove_if([](int x) { return x < 3; });
    values.sort();              // {3, 4}
    values.reverse();           // {4, 3}
}
```

#### `std::forward_list`：单向链表与前驱位置

单链表删除节点需要知道前驱，接口因此提供 `before_begin()`、`insert_after()` 与 `erase_after()`。`before_begin()` 是首节点之前的位置，不能解引用；`erase_after(prev)` 删除 `prev` 的后继，要求这个后继存在。

```cpp
#include <forward_list>

void eraseNegativeNodes() {
    std::forward_list<int> values{-1, 2, -3, 4};
    auto previous = values.before_begin();
    auto current = values.begin();
    while (current != values.end()) {
        if (*current < 0) {
            current = values.erase_after(previous);
        } else {
            previous = current;
            ++current;
        }
    } // {2, 4}
    values.push_front(1);
    values.pop_front();
}
```

`erase_after(before, last)` 删除的是**开区间 `(before, last)`**，这是普通 `erase(first, last)` 半开区间规则的一个重要区别。`forward_list` 没有 `back()`、`push_back()`、`pop_back()` 或 `size()`，可用 `std::distance(begin(), end())` 统计长度，但需 `O(n)`；支持成员 `remove`、`remove_if` 和 `sort`。

### 容器适配器：FIFO、LIFO 与优先级 {#stl-adapters}

```cpp
#include <functional>
#include <queue>
#include <stack>
#include <vector>

void useAdapters() {
    std::stack<int> history;
    history.push(1);
    history.push(2);
    int newest = history.top(); // 2，LIFO
    history.pop();

    std::queue<int> tasks;
    tasks.push(1);
    tasks.push(2);
    int oldest = tasks.front(); // 1，FIFO；back() 是 2
    tasks.pop();

    std::priority_queue<int> largestFirst;
    largestFirst.push(2);
    largestFirst.push(7);
    int largest = largestFirst.top(); // 7，默认最大堆
    std::priority_queue<int, std::vector<int>, std::greater<int>> smallestFirst;
    smallestFirst.push(2);
    smallestFirst.push(7);
    int smallest = smallestFirst.top(); // 2，最小堆
    (void)newest;
    (void)oldest;
    (void)largest;
    (void)smallest;
}
```

这些适配器提供 `size()`、`empty()`、`push()`、`pop()`，但没有公开的 `begin()` / `end()`，不能直接用范围 for 遍历，也不能直接交给 `std::sort`。访问 `top()` / `front()` 或 `pop()` 前须确保非空。适配器可配置底层容器，但该容器必须支持所需接口，例如不能把 `list` 作为 `priority_queue` 的底层随机访问序列。

`priority_queue` 不是 FIFO，它只维护堆结构而不是完整排序：`top()` 为 `O(1)`，堆调整为 `O(log n)`，底层 vector 的某次扩容还可能带来 `O(n)` 开销。`std::stack` 的“栈”是数据结构，与本笔记“堆和栈的区别”里的内存区域概念应分开理解。

### 关联容器：键、比较器与哈希 {#stl-associative}

#### 有序关联容器与平衡搜索树

`set` / `map` 家族通常用平衡搜索树实现，常见实现采用红黑树。红黑树是一种自平衡二叉搜索树，使按键查找、插入及删除的主要搜索过程为 `O(log n)`；**标准要求的是接口、顺序与复杂度，不强制规定必须用红黑树**。

比较器（Compare Functor）默认通常为 `std::less<K>`，要求形成**严格弱序**。键等价定义为 `!comp(a, b) && !comp(b, a)`，不一定等于 `a == b`；`set` 的元素唯一性、`map` 的键唯一性依据这个等价关系。使用 `<=` 作为比较器会违反严格性。

自定义类型可通过比较函数对象排序，`operator()` 返回“左边应排在右边之前吗”：

```cpp
#include <set>
#include <string>

struct Employee {
    int id;
    std::string name;
};
struct CompareEmployee {
    bool operator()(const Employee& a, const Employee& b) const {
        return a.id < b.id;
    }
};

void insertEmployees() {
    std::set<Employee, CompareEmployee> employees;
    auto [position, inserted] = employees.insert(Employee{7, "Alice"});
    auto [existing, added] = employees.insert(Employee{7, "Bob"});
    // inserted 为 true，added 为 false；同一 id 被视为等价键。
    // existing 指向原来的 Alice，重复插入不会自动替换名字。
    (void)position;
    (void)inserted;
    (void)existing;
    (void)added;
}
```

唯一键 `set::insert(value)` 返回 `std::pair<iterator, bool>`：`first` 指向插入成功的元素或已有等价元素，`second` 表示是否插入。提示位置插入、区间插入等重载的返回值不同；`multiset::insert(value)` 返回迭代器。

`set` 元素不能通过迭代器随意修改，因为修改键可能破坏顺序；`map` 的键同样只读，映射值可以修改。需要更改键时可删除再插入，或使用 C++17 节点句柄并遵守相关接口。

#### `std::pair`、map 键值对与查询

`std::pair<A, B>` 在 `<utility>` 中声明，成员 `first`、`second` 分别保存两个值。`map<K, T>` 和 `unordered_map<K, T>` 的元素类型是 `std::pair<const K, T>`：`first` 为只读键，`second` 为映射值。

```cpp
#include <map>
#include <string>
#include <utility>

void queryMap() {
    std::pair<int, std::string> item{1, "one"};
    std::map<std::string, int> counts{{"apple", 2}, {"banana", 3}};
    auto found = counts.find("apple");
    if (found != counts.end()) {
        found->second += 1;
    }
    int created = counts["pear"]; // 缺键：插入 pear，int 值初始化为 0
    counts["pear"] = 5;
    int checked = counts.at("pear"); // 缺键时抛 out_of_range，不插入
    (void)item;
    (void)created;
    (void)checked;
}
```

| 操作 | 键存在 | 键不存在 | 能否对 const map 调用 |
| --- | --- | --- | --- |
| `operator[](key)` | 返回值的引用 | 插入键与初始化的值 | 不能 |
| `find(key)` | 返回元素迭代器 | 返回 `end()` | 能 |
| `at(key)` | 返回值的引用 | 抛 `std::out_of_range` | 能，得到只读引用 |
| `count(key)` | 唯一键为 1；multi 可大于 1 | 返回 0 | 能 |

仅想查询时优先 `find()`，不要用 `operator[]` 意外添加数据。`map` / `unordered_map` 的 `find` 按键查找，不能直接按映射值查找；按值搜索一般要遍历。`multimap` / `unordered_multimap` 不提供 `operator[]` 或 `at()`，可用 `equal_range(key)` 遍历同键的范围。

#### 无序关联容器、Hash Table 与 Bucket

哈希表（Hash Table）根据哈希函数（Hash Function）计算键的哈希值，再映射到哈希桶（Bucket）。不同键可能落入同一桶或产生相同哈希值，称为碰撞，因此还需要相等判断；哈希相等不代表键相等。

默认哈希器为 `std::hash<K>`，默认相等比较器为 `std::equal_to<K>`，通常使用 `operator==`。如果相等比较器认为两个键等价，哈希器**必须**给出相同哈希值；反方向没有要求。自定义类型可以向容器传入哈希器与相等比较器，不必修改 `std`。

```cpp
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>

struct Key {
    int id;
    std::string region;
};
struct KeyHash {
    std::size_t operator()(const Key& key) const {
        return std::hash<int>{}(key.id) ^ (std::hash<std::string>{}(key.region) << 1);
    }
};
struct KeyEqual {
    bool operator()(const Key& a, const Key& b) const {
        return a.id == b.id && a.region == b.region;
    }
};

void useCustomHash() {
    std::unordered_map<Key, int, KeyHash, KeyEqual> counts;
    counts.reserve(32); // 根据预期元素数安排桶容量
    counts[Key{7, "east"}] = 3;
    auto found = counts.find(Key{7, "east"});
    if (found != counts.end()) found->second += 1;
}
```

这个组合哈希只用于展示接口，碰撞仍可能发生；实际分布要结合键数据评估。平均查找、插入、删除常为 `O(1)`，极端碰撞时可退化为 `O(n)`，不能说“哈希表永远常数时间”。

`bucket_count()` 是桶数，`load_factor()` 大致表示每桶平均元素数，`max_load_factor()` 控制增长阈值。`reserve(n)` 根据预期元素数量安排桶；`rehash(n)` 请求至少相应桶数，并满足负载要求。重哈希可能改变遍历顺序并使迭代器失效，但不会使已有元素的指针和引用因重哈希本身失效。

`std::unordered_map<int, std::vector<int>>` 合法：被哈希的是键 `int`，映射值 `vector<int>` 无须可哈希。若把 `vector<int>` 用作键，默认哈希器通常不满足要求，需设计合适的哈希与相等语义。`unordered_map` 前两个模板参数是键与值，后续还可配置哈希器、相等比较器及分配器；`unordered_set` 除键类型外同样有这些策略参数。

### 迭代器、半开区间与遍历 {#stl-iterators}

迭代器可以先理解成“位置”，`*it` 解引用取得该位置元素，`++it` 前进到下一位置，`it->member` 访问元素的成员。具体实现可能是指针，也可能是包装节点、存储块或调试状态的对象；不能认定 `vector::iterator` 在所有实现中就是裸指针。

| 容器 | 常见迭代器实现思路 |
| --- | --- |
| `vector` / `string` | 指向连续存储的位置，可能包裹指针 |
| `deque` | 存储块与块内位置的组合 |
| `list` / `forward_list` | 包装链表节点的位置 |
| `map` / `unordered_map` | 包装树节点或哈希节点的位置 |

`begin()` 指向首元素，`end()` 指向末元素之后；`[first, last)` 包含 `first`、不包含 `last`。空容器中 `begin() == end()`；`end()` 不能解引用，`++end()` 也不是合法的通用操作。

```cpp
#include <string>
#include <vector>

void visitElements() {
    std::string text = "abc";
    for (auto it = text.begin(); it != text.end(); ++it) {
        *it = static_cast<char>(*it + 1);
    } // 对这个字符串也可用下标循环访问相同元素

    std::vector<int> values{1, 2, 3};
    std::vector<int>::const_iterator reader = values.cbegin();
    int first = *reader;
    ++reader; // 迭代器可移动，但不能写 *reader = 9
    (void)first;
}
```

`cbegin()` / `cend()` 明确获得只读元素迭代器；对 const 容器调用 `begin()` 也会获得相应 const_iterator。**const_iterator** 限制通过迭代器修改元素；`const auto it = values.begin()` 则限制迭代器对象本身，元素可能仍可修改，两者不同。

#### 五类经典迭代器与能力要求

下面采用 C++17 常用的经典分类；“读写能力”还取决于元素是否 const，输出迭代器也不是必须可读的输入迭代器。

| 英文类别 | 中文 | 核心能力 | 例子 |
| --- | --- | --- | --- |
| Input Iterator | 输入迭代器 | 单遍读取、解引用与递增 | `std::istream_iterator` |
| Output Iterator | 输出迭代器 | 单遍写入、解引用赋值与递增 | `std::back_insert_iterator` |
| Forward Iterator | 前向迭代器 | 输入能力加多遍遍历 | `forward_list`、无序关联容器 |
| Bidirectional Iterator | 双向迭代器 | 前向能力加 `--` | `list`、有序关联容器 |
| Random Access Iterator | 随机访问迭代器 | 双向能力加常数时间跳转、差值与位置比较 | `array`、普通 `vector`、`deque` |

随机访问并不等于连续存储，`deque` 就是反例。C++20 进一步提供连续迭代器（Contiguous Iterator）的概念；不要把新旧概念体系完全等同。输入迭代器不保证多遍能力，例如从流中读取数据后不能假定一个旧副本仍指向可重新读取的同一项。

#### `std::advance` 与 `std::next`

两者在 `<iterator>` 中声明。`std::advance(it, n)` 修改原迭代器；`std::next(it, n)` 返回移动后的副本，原迭代器不变。随机访问迭代器可用常数时间跳转，链表迭代器需逐项移动，通常为 `O(abs(n))`。

```cpp
#include <iterator>
#include <list>

void moveIterator() {
    std::list<int> values{10, 20, 30, 40};
    auto it = values.begin();
    auto third = std::next(it, 2); // 指向 30，it 仍指向 10
    std::advance(it, 1);           // it 现在指向 20
    int sum = *it + *third;
    (void)sum;
}
```

负数步长要求双向或更强迭代器；前向迭代器不能倒退。任何移动都必须在有效范围内，`next` / `advance` 不会自动检查越界。`std::distance(first, last)` 同样在随机访问范围中为 `O(1)`，对一般链表范围为 `O(n)`。

#### 范围 for：`auto`、`auto&` 与 `const auto&`

| 声明 | 含义 | 典型用途 |
| --- | --- | --- |
| `for (auto x : values)` | 按值接收元素，通常复制 | 小标量值，或明确需要副本 |
| `for (auto& x : values)` | 引用原元素，可在允许时修改 | 原地更新、避免复制 |
| `for (const auto& x : values)` | 只读引用 | 读取大对象或不可复制对象 |

```cpp
#include <map>
#include <string>
#include <vector>

void rangeForExamples() {
    std::vector<int> values{1, 2, 3};
    for (auto& value : values) value *= 2;
    for (auto value : values) {
        value = 0; // 只改副本
        (void)value;
    }
    std::map<int, std::string> names{{1, "one"}, {2, "two"}};
    for (const auto& [key, name] : names) {
        (void)key;
        (void)name;
    }
}
```

set 的键通过迭代器是只读的，因此写 `auto&` 也不代表一定能改元素；`vector<bool>` 的代理引用等特殊类型宜按接口选择 `auto&&`。范围 for 隐藏了迭代器与终点，遍历过程中使它们失效同样危险；不能一边遍历 vector 一边随意 `push_back` 或 `erase`。

#### Iterator Invalidation：迭代器失效 {#stl-iterator-invalidation}

“失效”意味着旧迭代器不再能按原方式使用，继续解引用或比较可能产生未定义行为。引用、指针、迭代器和 `end()` 的稳定性要分别看待。

| 容器与操作 | 主要规则 |
| --- | --- |
| 普通 vector 发生重新分配 | 所有迭代器、指针、引用以及旧 `end()` 失效 |
| vector 不扩容的插入 | 插入位置及其后迭代器、引用失效；位置之前保留，旧 `end()` 失效 |
| vector 删除元素 | 删除位置及其后迭代器、引用失效，旧 `end()` 失效 |
| deque 首尾插入 | 迭代器失效，已有元素的引用保留；中间插入还会使引用失效 |
| deque 删除 | 首尾删除主要影响被删元素，删尾还影响旧 `end()`；中间删除会使全部迭代器、引用失效 |
| list / forward_list 插入 | 不使已有元素迭代器、引用失效 |
| list / forward_list 删除 | 只使被删元素的迭代器、引用失效 |
| set / map 家族插入、删除 | 插入不使已有迭代器、引用失效；删除只影响被删元素 |
| unordered 家族重哈希 | 迭代器失效，已有元素的指针和引用仍有效 |
| unordered 家族插入、删除 | 插入若触发重哈希则使迭代器失效；删除只影响被删元素 |
| array | 没有改变大小的操作；对象本身结束生命周期后不能继续引用 |

表格针对普通插入删除等操作，赋值、交换、移动还要查对应契约。动态容器 `clear()` 销毁所有元素，不能继续使用原元素引用。对 vector 缓存首元素引用后再扩容，即使程序“看起来正常”，也不保证合法。

遍历删除时，使用 `erase` 返回的后继位置：

```cpp
#include <vector>

void eraseWhileIterating() {
    std::vector<int> values{1, 2, 3, 4};
    for (auto it = values.begin(); it != values.end();) {
        if (*it % 2 == 0) it = values.erase(it);
        else ++it;
    } // {1, 3}，删除后不再递增已经失效的旧 it
}
```

这一用法不能不加区别地套到 `forward_list`，它需要保留前驱并用 `erase_after`。

### STL 算法与可调用对象 {#stl-algorithms}

通用算法主要在 `<algorithm>` 中声明，处理迭代器给出的半开区间，不直接决定容器的存储布局，也通常不会自动改变容器 `size()`。

| 算法 | 作用 | 要求与复杂度概览 |
| --- | --- | --- |
| `std::swap(a, b)` | 交换两个对象，在 `<utility>` 中声明 | 普通泛型版本常通过移动；成本取决于类型，容器可有专门重载 |
| `std::reverse(first, last)` | 反转范围内的值 | 双向迭代器；`O(n)` |
| `std::remove(first, last, value)` | 将不等于 value 的元素压到前面 | 可写前向迭代器与赋值；`O(n)`，不真正删容器元素 |
| `std::remove_if(first, last, pred)` | 按谓词压缩保留元素 | 同上；`O(n)` |
| `std::replace(first, last, old, replacement)` | 将匹配的值赋为新值 | 可写前向迭代器；`O(n)` |
| `std::fill(first, last, value)` | 向已有元素赋同一值 | 可写前向迭代器；`O(n)` |
| `std::sort(first, last, comp)` | 按严格弱序排序 | 随机访问迭代器；`O(n log n)` 次比较，通常不稳定 |
| `std::for_each(first, last, fn)` | 对每个元素调用 fn | 输入迭代器；调用次数为 `n` |
| `std::count_if(first, last, pred)` | 统计满足谓词的元素数 | 输入迭代器；`O(n)` |

`std::sort` 不适用于 list / forward_list，应调用其成员 `sort()`。set / map 的键不能赋值，不能通过 `std::remove` 压缩或 `std::sort` 改变键排列；需要它们自己的查询与删除接口。算法能力取决于迭代器和元素操作，不能只看“容器有 begin/end”。

#### Function Pointer、Functor、Lambda 与 Predicate

算法可接收函数指针（Function Pointer）、函数对象（Functor，定义 `operator()` 的类实例）或 Lambda。返回可用于条件判断结果的可调用对象称为谓词（Predicate）；比较器通常是接收两个参数的谓词，并有额外的严格弱序要求。

```cpp
#include <algorithm>
#include <vector>

bool isPositive(int value) { return value > 0; }
struct Above {
    int limit;
    bool operator()(int value) const { return value > limit; }
};

void usePredicates() {
    std::vector<int> values{-2, 1, 4, 7};
    bool (*predicate)(int) = &isPositive;
    auto positive = std::count_if(values.begin(), values.end(), predicate); // 3
    auto aboveThree = std::count_if(values.begin(), values.end(), Above{3}); // 2
    int threshold = 5;
    auto aboveFive = std::count_if(values.begin(), values.end(),
                                 [threshold](int x) { return x > threshold; }); // 1
    std::for_each(values.begin(), values.end(), [](int& x) { x *= 2; });
    std::replace(values.begin(), values.end(), -4, 0);
    std::reverse(values.begin(), values.end());
    std::sort(values.begin(), values.end());
    std::swap(values.front(), values.back());
    (void)positive;
    (void)aboveThree;
    (void)aboveFive;
}
```

Lambda 的捕获列表 `[threshold]` 保存一个值副本；`[&threshold]` 引用原变量；`[]` 不捕获外部局部变量。无捕获 Lambda 可在匹配签名时转换为函数指针，带捕获 Lambda 通常不行。泛型算法模板可以直接接收闭包或函数对象，不必为了传入 Lambda 一律包装成 `std::function`。

谓词应符合算法契约，不应修改其检查的元素或破坏容器结构。算法可能复制可调用对象，不能依赖所有调用都发生在同一个外部函数对象实例上。引用捕获仍要考虑生命周期；比较器在处理同一批数据时应保持一致的排序语义。Functor 与 Lambda 的通用语法保留在本笔记相关章节。

#### Erase-Remove Idiom：逻辑移除再真正删除 {#stl-erase-remove}

`std::remove` / `std::remove_if` 将应保留元素移动到范围前部，并返回**新的逻辑终点**；容器大小保持不变，尾部仍有有效但内容不应依赖的元素。随后调用容器的 `erase` 才真正销毁尾部元素、改变大小。

```cpp
#include <algorithm>
#include <vector>

void eraseRemoveExamples() {
    std::vector<int> values{1, 2, 3, 2, 4};
    auto newEnd = std::remove(values.begin(), values.end(), 2);
    // size 仍为 5；[begin(), newEnd) 是 {1, 3, 4}。
    values.erase(newEnd, values.end()); // size 变为 3
    values.erase(std::remove_if(values.begin(), values.end(),
                               [](int x) { return x % 2 == 0; }), values.end());
    // 最终 {1, 3}
}
```

链表可直接用成员 `remove` / `remove_if` 删除节点，避免通用算法逐项赋值。C++20 对部分容器提供 `std::erase` / `std::erase_if` 简化接口；本例使用 C++17 写法。删除相同数值时也要避免把容器内一个随后可能被移动赋值的元素引用作为待删值，可先复制所需值。

### 时间复杂度、内存布局与容器选择 {#stl-container-selection}

`O(1)` 表示操作次数不随元素数量线性增长，并不保证实际耗时恒定或更快；`O(log n)`、`O(n)` 描述规模变化的趋势。比较、哈希、元素复制等自身开销还要单独考虑。

| 容器 | 随机访问 / 按键查找 | 单元素插入删除的主要成本 | 内存与稳定性考量 |
| --- | --- | --- | --- |
| array | 下标 `O(1)`；按值搜索 `O(n)` | 大小不能改变 | 连续、无扩容 |
| vector | 下标 `O(1)`；按值搜索 `O(n)` | 尾插摊还 `O(1)`、尾删 `O(1)`；中间 `O(n)` | 连续、缓存友好；扩容使地址失效 |
| deque | 下标 `O(1)`；按值搜索 `O(n)` | 首尾 `O(1)`；中间通常 `O(n)` | 通常分块，迭代器与引用稳定性不同 |
| list / forward_list | 顺序访问、搜索 `O(n)` | 已知位置 / 前驱后 `O(1)` | 节点额外开销，未删除节点较稳定 |
| set / map 家族 | 按键查找 `O(log n)` | 普通插入 `O(log n)`；按键删除还受匹配数量影响 | 通常树节点，保留键顺序与稳定迭代器 |
| unordered 家族 | 按键平均 `O(1)`、最坏 `O(n)` | 平均常数量级；重哈希或多匹配另计 | 桶与节点开销，重哈希影响迭代器 |

按键删除多个匹配项需计入删除数量；有序容器按迭代器删除单项通常摊还 `O(1)`，不能把一切树操作都写成 `O(log n)`。区间操作也要计入处理的元素数量。

一般先考虑 `vector`，再根据真正需求选择：固定大小用 array，两端操作多用 deque，只需栈 / 队列语义用适配器，必须保持节点位置并已有位置时才考虑链表；按键查找用关联容器，需有序遍历或范围查询选树型，主要等值查询且不需顺序时考虑哈希型。

```cpp
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

void chooseKeyContainers() {
    std::unordered_set<char> vowels{'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
    std::unordered_map<std::string, int> counts{{"apple", 2}, {"banana", 3}};
    std::unordered_map<int, int> numbers{{1, 10}, {2, 20}, {5, 50}};
    std::unordered_map<int, std::vector<int>> groups; // 值无须可哈希
    std::set<int> ordered{5, 1, 3, 2}; // 遍历顺序为 1、2、3、5
    std::map<int, std::string> names{{1, "one"}, {3, "three"}, {2, "two"}};
    numbers[5] = 10;
    for (const auto& pair : numbers) {
        (void)pair.first; // key
        (void)pair.second; // value
    }
    auto found = numbers.find(5);
    if (found != numbers.end()) found->second += 1;
    if (vowels.count('a')) vowels.insert('b');
    vowels.erase('a');
}
```

容量、随机访问与顺序访问、插入删除的真实位置、元素大小、内存布局和迭代器稳定性应一起考虑。不能只看复杂度表：短序列中连续存储的 vector 可能比频繁分配节点的 list 更快，具体工作负载仍需测量。

### STL 与运行时多态：避免对象切片 {#stl-polymorphism}

模板按静态类型生成代码，运行时多态则通过虚函数根据实际对象选择实现。两者可以组合：`vector<std::unique_ptr<Base>>` 是一个静态确定的容器类型，元素指针可以指向不同派生类，并通过虚函数运行时分派。

`vector<Base>` 按值保存 Base。若 Base 可具体实例化，把 Derived 放进去会发生 **Object Slicing（对象切片）**：只复制或移动 Base 子对象，派生数据与派生动态类型不会被保留；若 Base 是抽象类，则相应按值存储对象的方式不可行。容器不会自动提供多态克隆。

```cpp
#include <iostream>
#include <memory>
#include <vector>

class Task {
public:
    virtual ~Task() = default;
    virtual void run() const = 0;
};
class SaveTask final : public Task {
public:
    void run() const override { std::cout << "save\n"; }
};
class SendTask final : public Task {
public:
    void run() const override { std::cout << "send\n"; }
};

int main() {
    std::vector<std::unique_ptr<Task>> tasks;
    tasks.push_back(std::make_unique<SaveTask>());
    tasks.push_back(std::make_unique<SendTask>());
    for (const auto& task : tasks) task->run(); // save、send
    auto moved = std::move(tasks); // 转移所有权，未复制派生对象
    moved.clear(); // 销毁 unique_ptr，释放其拥有的 Task 对象
}
```

`unique_ptr` 表示独占所有权，不能复制，`vector<unique_ptr<T>>` 因而不能按普通元素复制方式进行容器复制；若确实要复制多态对象，应提供显式 `clone()` 等协议。需要共享生命周期时可考虑 `shared_ptr`，但复制 shared_ptr 只共享对象，不是深拷贝派生对象。非拥有引用也需保证目标对象活得足够久。

删除派生对象时，先执行派生析构函数体，再逆序销毁其成员、销毁基类部分；基类同样按自己的规则清理。通过拥有的基类指针删除派生对象时，基类应具备相应的虚析构接口。容器元素之间的销毁顺序不应假定必然是插入顺序或逆序；vector 扩容也可能销毁旧位置的元素，不能简单按 push 次数预测全部析构日志。

智能指针管理多态对象还有一个稳定性区别：vector 扩容会移动 unique_ptr 元素，使指向这些指针槽位的引用失效，但所拥有的堆上对象通常没有因此搬迁；当其所有者被删除或重置时，指向实际对象的裸指针才也会悬空。析构函数、虚函数及智能指针的通用机制继续参见本笔记对应章节。

### 原始参考与版本范围

本章代码以 C++17 为主；标注的连续迭代器概念与 `std::erase_if` 等属于 C++20。完整规则应以对应标准版本与后续缺陷修正为准。

- [WG21 C++17 工作草案 N4659](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf)：容器 `[containers]`、迭代器 `[iterators]`、算法 `[algorithms]`；vector 容量 `[vector.capacity]`、修改 `[vector.modifiers]`，以及关联容器 `[associative.reqmts]`、无序关联容器 `[unord.req]`。
- [Microsoft 迭代器文档](https://learn.microsoft.com/en-us/cpp/standard-library/iterators?view=msvc-170)：遍历与经典迭代器能力分类；阅读时仍须区分实现说明和标准要求。
- [Microsoft vector 文档](https://learn.microsoft.com/en-us/cpp/standard-library/vector-class?view=msvc-170)：构造、容量和修改接口。
- [Microsoft algorithm 文档](https://learn.microsoft.com/en-us/cpp/standard-library/algorithm-functions?view=msvc-170)：通用算法及接口要求。

## 二叉树

### 一、二叉树基础概念

二叉树是一种树形数据结构，每个节点最多有两个孩子，通常叫做左孩子和右孩子。

- 根节点：整棵树最上面的节点。
- 叶子节点：没有孩子的节点。
- 子树：某个节点和它下面的所有节点组成的小树。
- 深度 / 高度：从根节点到某个节点，或从某个节点到叶子节点的层数。
- 满二叉树：每一层节点都放满。
- 完全二叉树：除了最后一层，前面每层都满；最后一层从左到右连续排列。
- 二叉搜索树：左子树节点值通常小于根，右子树节点值通常大于根。

```text
        1
      /   \
     2     3
    / \   / \
   4   5 6   7
```

### 二、二叉树在 C 语言中的结构体定义

LeetCode 的二叉树题一般已经给好这个结构体：

```c
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};
```

含义：

- `val`：当前节点存的值。
- `left`：指向左孩子，没有左孩子就是 `NULL`。
- `right`：指向右孩子，没有右孩子就是 `NULL`。

### 三、二叉树遍历

常见遍历顺序：

- 前序遍历：根 左 右
- 中序遍历：左 根 右
- 后序遍历：左 右 根
- 层序遍历 / BFS：一层一层访问，通常使用队列

用 1~7 节点的满二叉树举例：

```text
        1
      /   \
     2     3
    / \   / \
   4   5 6   7
```

| 遍历方式 | 访问顺序 |
| ---- | ---- |
| 前序 | `1 2 4 5 3 6 7` |
| 中序 | `4 2 5 1 6 3 7` |
| 后序 | `4 5 2 6 7 3 1` |
| 层序 | `1 2 3 4 5 6 7` |

### 四、C 语言代码模板

递归遍历的核心写法是：先判断 `root == NULL`，再按遍历顺序处理根、左子树、右子树。

**前序遍历递归模板**

```c
// LeetCode 144: Binary Tree Preorder Traversal
#define MAX_NODE 2000

void preorderDfs(struct TreeNode* root, int* ans, int* size) {
    if (root == NULL) return;

    ans[(*size)++] = root->val;          // 根
    preorderDfs(root->left, ans, size);  // 左
    preorderDfs(root->right, ans, size); // 右
}

int* preorderTraversal(struct TreeNode* root, int* returnSize) {
    int* ans = (int*)malloc(sizeof(int) * MAX_NODE);
    *returnSize = 0;
    preorderDfs(root, ans, returnSize);
    return ans;
}
```

**中序遍历递归模板**

```c
// LeetCode 94: Binary Tree Inorder Traversal
#define MAX_NODE 2000

void inorderDfs(struct TreeNode* root, int* ans, int* size) {
    if (root == NULL) return;

    inorderDfs(root->left, ans, size);   // 左
    ans[(*size)++] = root->val;          // 根
    inorderDfs(root->right, ans, size);  // 右
}

int* inorderTraversal(struct TreeNode* root, int* returnSize) {
    int* ans = (int*)malloc(sizeof(int) * MAX_NODE);
    *returnSize = 0;
    inorderDfs(root, ans, returnSize);
    return ans;
}
```

**后序遍历递归模板**

```c
// LeetCode 145: Binary Tree Postorder Traversal
#define MAX_NODE 2000

void postorderDfs(struct TreeNode* root, int* ans, int* size) {
    if (root == NULL) return;

    postorderDfs(root->left, ans, size);  // 左
    postorderDfs(root->right, ans, size); // 右
    ans[(*size)++] = root->val;           // 根
}

int* postorderTraversal(struct TreeNode* root, int* returnSize) {
    int* ans = (int*)malloc(sizeof(int) * MAX_NODE);
    *returnSize = 0;
    postorderDfs(root, ans, returnSize);
    return ans;
}
```

**层序遍历 BFS 模板**

```c
// LeetCode 102: Binary Tree Level Order Traversal
#define MAX_NODE 2000

int** levelOrder(struct TreeNode* root, int* returnSize, int** returnColumnSizes) {
    *returnSize = 0;
    *returnColumnSizes = NULL;
    if (root == NULL) return NULL;

    int** ans = (int**)malloc(sizeof(int*) * MAX_NODE);
    *returnColumnSizes = (int*)malloc(sizeof(int) * MAX_NODE);

    struct TreeNode** queue = (struct TreeNode**)malloc(sizeof(struct TreeNode*) * MAX_NODE);
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        int levelSize = rear - front;
        int* level = (int*)malloc(sizeof(int) * levelSize);

        for (int i = 0; i < levelSize; i++) {
            struct TreeNode* node = queue[front++];
            level[i] = node->val;

            if (node->left != NULL) queue[rear++] = node->left;
            if (node->right != NULL) queue[rear++] = node->right;
        }

        ans[*returnSize] = level;
        (*returnColumnSizes)[*returnSize] = levelSize;
        (*returnSize)++;
    }

    free(queue);
    return ans;
}
```

### 五、递归思想解释

递归可以理解成：把一棵大树的问题，拆成根节点、左子树、右子树三个部分。

```text
处理 root
递归处理 root->left
递归处理 root->right
```

以遍历为例，函数只负责“当前这棵树”：

- 如果当前节点是 `NULL`，说明这棵树为空，直接返回。
- 如果当前节点不是 `NULL`，就按指定顺序访问根节点、左子树、右子树。
- 左右子树本身也是二叉树，所以继续调用同一个函数。

三种 DFS 遍历只差“访问根节点”的位置：

```text
前序：先访问根，再递归左右
中序：先递归左，再访问根，再递归右
后序：先递归左右，再访问根
```

## 双指针

```cpp
// 对撞指针（左右夹逼）
l = 0, r = n - 1;
while (l < r) { ... }

// 快慢指针（同向）
slow = 0;
for (fast = 0; fast < n; fast++) {
    if (ok(nums[fast])) nums[slow++] = nums[fast];
}

// 滑动窗口（变长区间）
l = 0;
for (r = 0; r < n; r++) {
    while (invalid) l++;
}

// 匹配指针
i = 0, j = 0;
while (i < s && j < t) {
    if (s[i] == t[j]) i++;
    j++;
}
```

判断题型时可以这样想：

1. 有没有“连续子数组”？
   有的话，大概率考虑滑动窗口
2. 数组是否有序？
   有序时，大概率可以考虑双指针
3. 是否在找“两个数的关系”？
   有序用双指针，无序常用哈希表
4. 是否涉及“左边和 == 右边和”？
   可以先想到前缀和

## 时间复杂度（Time Complexity）

- `O(n)`：线性时间，例如遍历一个长度为 `n` 的数组
- `O(n^2)`：二次时间，例如双重循环遍历 `n x n` 矩阵
- `O(log n)`：对数时间，例如红黑树、二分查找、堆操作

额外速记：

- 哈希表：平均 `O(1)`
- 红黑树：`O(log n)`
- 暴力循环：常见是 `O(n)`

## 空间复杂度

空间复杂度描述的是算法运行时额外占用的内存大小。

```cpp
vector<int> v(n);  // O(n)，需要一个长度为 n 的数组

int sum(int n) {
    int s = 0;
    for (int i = 1; i <= n; i++)
        s += i;
    return s;
}  // O(1)，只用了常数个额外变量
```

## 堆和栈的区别

- 栈：自动分配、自动回收的临时内存
- 堆：手动申请、手动（或系统）释放的长期内存
- 栈中常见存放：函数参数、局部变量、基本数据类型、指针/引用变量本身、返回地址等函数调用信息
- 堆中常见存放：通过 `new` / `malloc` 动态申请的对象、数组、结构体、类实例等生命周期不固定的数据

| 中文 | 英语 | 法语 |
| ---- | ---- | ---- |
| 栈 | **Stack** | **Pile** |
| 堆 | **Heap** | **Tas** |

```text
ListNode* p = new ListNode(3);

栈（Stack / Pile）        堆（Heap / Tas）
┌────────────┐           ┌──────────────┐
│ p (地址)   │ ────────▶ │ ListNode{3}  │
└────────────┘           └──────────────┘
```

指针变量 `p` 在栈上，而它指向的对象在堆上。

```cpp
int a = 10;              // 普通局部变量，通常在栈上
int* p = new int(20);    // 指针变量 p 在栈上，*p 在堆上
```

## 最大公约数思想

可以用辗转相除法：

```cpp
int r = a % b;
a = b;
b = r;
```

## 面向对象

面向对象可以理解成：用“对象”来组织数据，并把操作这些数据的方法也放到对象身上。

```cpp
string s = "HELLO";
s.erase(2);
s.push_back('!');
```

这里 `s` 是对象，`erase` 和 `push_back` 是操作这个对象的方法。


## 抽象类

在 C++ 中，只要一个类中包含至少一个纯虚函数（pure virtual function），这个类就是抽象类。

纯虚函数写法：

```cpp
virtual 返回类型 函数名() = 0;
```

这里的 `= 0` 不是“等于 0”，而是 C++ 规定的纯虚函数写法。

## 抽象类的核心特点

1. 不能创建对象

```cpp
class Animal {
public:
    virtual void speak() = 0;  // 纯虚函数
};
Animal a; // ❌ 错误，不能创建抽象类对象
```

2. 可以作为基类
3. 子类必须实现所有纯虚函数，否则子类本身还是抽象类

## 工程风格示例

```cpp
class Device {
public:
    virtual void init() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual ~Device() {}   // 必须有虚析构函数
};
// 实现：
class UART : public Device {
public:
    void init() override { cout << "UART init\n"; }
    void start() override { cout << "UART start\n"; }
    void stop() override { cout << "UART stop\n"; }
};
// 然后在主程序里：
Device* dev = new UART();
dev->init();
// 这就是：面向接口编程
```

## 继承

继承的核心思想是：如果一个类是另一个类的“特殊情况”，那就可以用继承。

比如：

- 狗是动物
- 车是交通工具
- 学生是人

```csharp
Student is a Person
Dog is an Animal
```

在 C++ 里就是：

```cpp
class Student : public Person
```

```cpp
class C : public A
```

意思是：`C` 继承 `A`，`C` 会自动拥有 `A` 的所有 `public` 成员。

这里的 `public` 表示继承方式：

| 写法        | 意义                      |
| --------- | ----------------------- |
| public    | 保持父类的 public 仍然是 public |
| protected | 父类 public 变成 protected  |
| private   | 父类 public 变成 private    |

如果不写，默认是 `private` 继承。

好，这个是 C++ 面向对象三大特性之一：

> 封装、继承、多态

## C++ 继承中的虚析构函数
在 C++ 继承中，有一个很重要的规则：
> 如果一个类可能被继承，并且将来可能通过“基类指针”删除子类对象，那么基类的析构函数应该声明为 `virtual`。
例如：
```cpp
class Base {
public:
    ~Base() {
        std::cout << "Base destructor\n";
    }
};
class Derived : public Base {
public:
    ~Derived() {
        std::cout << "Derived destructor\n";
    }
};
```
如果这样使用：
```cpp
Base* p = new Derived();
delete p;
```
`p` 的类型是 `Base*`，但是它实际指向的是一个 `Derived` 对象。
如果 `Base` 的析构函数不是虚函数，那么通过 `delete p` 删除对象时，可能只会调用基类的析构函数：
```cpp
Base destructor
```
而不会调用子类 `Derived` 的析构函数。
这会带来严重问题：如果子类中申请了内存、打开了文件、创建了线程，或者占用了其他资源，那么这些资源可能无法被正确释放。
正确写法应该是：
```cpp
class Base {
public:
    virtual ~Base() {
        std::cout << "Base destructor\n";
    }
};
class Derived : public Base {
public:
    ~Derived() {
        std::cout << "Derived destructor\n";
    }
};
```
这时再执行：
```cpp
Base* p = new Derived();
delete p;
```
析构函数的调用顺序是：
```cpp
Derived destructor
Base destructor
```
也就是说，先析构子类部分，再析构基类部分。
注意：虚析构函数不等于抽象类
一个类被继承，并不代表它一定要写成抽象类。
抽象类指的是包含纯虚函数的类，例如：
```cpp
class Base {
public:
    virtual void run() = 0;
};
```
这里的 `= 0` 表示纯虚函数，所以 `Base` 是抽象类，不能直接创建对象。
但是下面这个类不是抽象类：
```cpp
class Base {
public:
    virtual ~Base() {}
};
```

虽然它有虚析构函数，但析构函数不是纯虚函数，所以这个类仍然可以创建对象。

## 总结

继承本身不要求基类必须是抽象类。

但是，只要一个类会作为基类使用，并且可能出现下面这种写法：

```cpp
Base* p = new Derived();
delete p;
```

那么基类的析构函数就应该写成虚析构函数：

```cpp
virtual ~Base() {}
```

这样可以保证删除对象时，子类和基类的析构函数都会被正确调用，避免资源泄漏。


## 多态

多态（Polymorphism）字面意思是：

> 多种形态

在 C++ 里通常指：

> 同一个接口，不同对象，表现不同的行为

举个例子：

```cpp
Animal* a = new Dog();
Animal* b = new Cat();

a->speak();   // 汪
b->speak();   // 喵
```

同样是 `speak()`，但行为不同，这就是多态。

## 实现多态的三个条件

1. 继承
2. 虚函数（`virtual`）
3. 基类指针或引用调用

缺一个都不是真正的运行时多态。

## 完整示例代码

```cpp
#include <iostream>
using namespace std;
class Animal {
public:
    virtual void speak() {     // 虚函数
        cout << "Animal sound" << endl;
    }
};
class Dog : public Animal {
public:
    void speak() override {
        cout << "Dog: Woof" << endl;
    }
};
class Cat : public Animal {
public:
    void speak() override {
        cout << "Cat: Meow" << endl;
    }
};
int main() {
    Animal* a1 = new Dog();
    Animal* a2 = new Cat();

    a1->speak();   // 调用 Dog 的版本
    a2->speak();   // 调用 Cat 的版本

    delete a1;
    delete a2;
}
```

## `override` 与 `final`

`override` 表示子类函数要重写基类虚函数。若函数名、参数或 `const` 等签名不匹配，编译器会直接报错，因此重写虚函数时建议始终添加：

```cpp
class Device {
public:
    virtual void start() const = 0;
};

class Sensor : public Device {
public:
    void start() const override final {}  // 正确重写，并禁止继续重写
};
```

`final` 用在虚函数后表示禁止子类再次重写，用在类名后表示禁止该类被继承：

```cpp
class FixedDevice final : public Device {
public:
    void start() const override {}
};
```

简单记忆：`override` 是“确认我在重写”，`final` 是“到这里为止”。

## 如果没有 virtual 会怎样？

```cpp
class Animal {
public:
    void speak() {   // 没有 virtual
        cout << "Animal sound" << endl;
    }
};
```
那就会变成：
```
Animal sound
Animal sound
```

## 多态分两种

1. 编译时多态（静态多态）

   - 函数重载
   - 运算符重载
   - 模板

   例子：

```cpp
int add(int a, int b);
double add(double a, double b);
```

   编译时就确定调用哪个版本。

2. 运行时多态（动态多态）

   - 通过 `virtual` 实现
   - 通过基类指针调用

## 底层原理

多态靠的是：

> 虚函数表（vtable）

每个带 `virtual` 的类通常都会有虚函数表，对象内部也会存一个 `vptr`（指向虚函数表的指针）。

当你调用 `a->speak();` 时，实际流程大致是：

1. 通过 `vptr` 找到 `vtable`
2. 再找到真正的函数地址
3. 最终调用 `Dog::speak()`

这就是动态绑定（Dynamic Binding）。

## 多态的好处（工程角度）

在以后做嵌入式架构时，假设你有：

- UART 驱动
- SPI 驱动
- CAN 驱动

你可以这样写：

```cpp
Device* dev = new UART();
dev->init();
```

主程序根本不关心具体是什么设备，这就是：

> 面向接口编程

## 什么是多态？

多态是指同一接口在不同对象上表现出不同的行为。在 C++ 中通常通过继承和虚函数实现运行时多态，依赖虚函数表进行动态绑定。

## 和抽象类的关系

抽象类通常用来实现多态。

流程是：

```
抽象类定义接口
↓
子类实现接口
↓
基类指针调用
↓
产生多态
```

## 为什么不直接创建 Dog 和 Cat，而要用多态

如果程序里只处理几个确定对象，可以直接创建具体类。

```cpp
Dog d;
Cat c;
d.speak();
c.speak();
```

这种写法简单直接，完全没问题。
但是如果希望用一套统一代码处理多种不同对象，就适合使用多态。

```cpp
void makeSpeak(Animal& a) {
    a.speak();
}
```

这里 `makeSpeak()` 不关心传进来的是 `Dog` 还是 `Cat`，只要求它是一个 `Animal`。

```cpp
Dog d;
Cat c;
makeSpeak(d);
makeSpeak(c);
```

如果 `speak()` 是虚函数，那么虽然参数类型是 `Animal&`，实际执行时会根据真实对象类型调用对应版本。

```cpp
Animal& a = d;
a.speak(); // 实际调用 Dog::speak()
```

多态的核心作用是：用统一接口处理不同类型对象，减少重复代码，方便以后扩展新类型。

## 继承、抽象类、多态分别是什么

继承表示“是一种”的关系。

```cpp
class Dog : public Animal {};
```

意思是 `Dog` 是一种 `Animal`。
继承适合把多个类的共同特征抽出来，形成统一的父类类型。
抽象类一般用来定义规则，而不是直接创建对象。

```cpp
class Animal {
public:
    virtual void speak() = 0;
};
```

这里 `= 0` 表示纯虚函数，`Animal` 就变成抽象类。
抽象类适合规定所有子类必须实现某些函数。

```cpp
class Dog : public Animal {
public:
    void speak() override {
        cout << "Dog: Woof" << endl;
    }
};
```

如果 `Dog` 不实现 `speak()`，那么 `Dog` 也会继续是抽象类，不能创建对象。
多态指的是：同一个接口，不同对象有不同表现。

```cpp
Animal* a = new Dog();
a->speak(); // 调用 Dog 的版本
```

多态通常需要：有继承关系，父类函数是 `virtual`，子类重写该函数，通过父类指针或父类引用调用。

## 多态的典型使用场景

多态适合处理“类型不同，但行为接口相同”的对象。
例如动物系统中，不同动物都可以 `speak()`。

```cpp
vector<Animal*> animals;
animals.push_back(new Dog());
animals.push_back(new Cat());
for (Animal* a : animals) {
    a->speak();
}
```

循环中代码只有一份：

```cpp
a->speak();
```

但实际执行时，可能调用 `Dog::speak()`，也可能调用 `Cat::speak()`。
实际开发中，多态常用于通信接口、设备接口、驱动接口、任务接口等。

```cpp
class Communication {
public:
    virtual void send() = 0;
};
class UART : public Communication {
public:
    void send() override {
        cout << "Send by UART" << endl;
    }
};
class CAN : public Communication {
public:
    void send() override {
        cout << "Send by CAN" << endl;
    }
};
```

主程序只依赖统一接口：

```cpp
void sendData(Communication& com) {
    com.send();
}
```

以后新增 `TCP`、`RS485` 等通信方式时，原来的 `sendData()` 不需要修改。

## 基类引用和基类指针都可以实现多态

这个参数不是指针，而是引用。

```cpp
void sendData(Communication& com) {
    com.send();
}
```

`Communication&` 表示基类引用，`com` 是传入对象的别名。

```cpp
UART uart;
sendData(uart);
```

只要 `send()` 是虚函数，真实对象是 `UART`，就会调用 `UART::send()`。
指针版本也可以。

```cpp
void sendData(Communication* com) {
    com->send();
}
UART uart;
sendData(&uart);
```

也可以动态创建对象。

```cpp
Communication* p = new UART();
sendData(p);
delete p;
```

但是简单场景下更推荐引用和局部对象，因为不用手动 `new` 和 `delete`。
不要用值传递实现多态。

```cpp
void sendData(Communication com) {
    com.send();
}
```

这种写法会发生对象切片，只保留基类部分，子类部分会丢失，多态会失效。

## std::function 是什么

`std::function` 是 C++ 里的通用函数包装器，头文件是：

```cpp
#include <functional>
```

它可以保存“能像函数一样被调用的东西”，比如普通函数、lambda、函数对象、绑定后的成员函数。
基本语法：

```cpp
std::function<返回值类型(参数类型列表)> 变量名;
```

常见形式：

```cpp
std::function<void()> f1;          // 无参数，无返回值
std::function<int(int, int)> f2;   // 两个 int 参数，返回 int
std::function<void(int)> f3;       // 一个 int 参数，无返回值
```

保存普通函数：

```cpp
int add(int a, int b) {
    return a + b;
}
std::function<int(int, int)> f = add;
cout << f(3, 5) << endl;
```

`std::function` 常用于回调函数，也就是把一个函数行为作为参数传给另一个函数。

## lambda 是什么

lambda 可以理解成“没有名字的临时函数”。
普通函数写法：

```cpp
void hello() {
    cout << "Hello" << endl;
}
```

lambda 写法：

```cpp
[]() {
    cout << "Hello" << endl;
}
```

lambda 常用于只需要临时使用一次的小函数，不想单独起名字。
它也可以保存到 `std::function` 里。

```cpp
std::function<void()> f = []() {
    cout << "Hello lambda" << endl;
};
f();
```

这里 `f` 是一个函数变量，里面保存了这个 lambda，`f()` 就是调用它。

## lambda 中 [](){} 分别是什么意思

Lambda 的完整常用语法是：

```cpp
[capture-list](params) mutable -> return_type {
    body
}
```

其中参数列表、`mutable` 和尾置返回类型都可以按需省略。没有显式写 `-> return_type` 时，编译器会根据 `return` 表达式推断返回类型；多个返回分支必须能推断成兼容的类型。

`[]` 是捕获列表，用来捕获外部变量。

```cpp
[] // 不捕获外部变量
```

`()` 是参数列表，和普通函数参数一样。

```cpp
[](int a, int b) {
    return a + b;
}
```

`{}` 是函数体，里面写真正执行的代码。

```cpp
[]() {
    cout << "Hello lambda" << endl;
}
```

这段 lambda 表示：不捕获外部变量，没有参数，执行时输出一行文字。
如果要使用外部变量，可以写捕获列表：

```cpp
int x = 10;
auto f = [x]() {
    cout << x << endl;
};
f();
```

这里 `[x]` 表示把外部变量 `x` 捕获进 lambda。

常见捕获方式：

| 写法 | 含义 |
| --- | --- |
| `[]` | 不捕获外部变量 |
| `[&]` | 默认按引用捕获使用到的外部变量 |
| `[=]` | 默认按值捕获使用到的外部变量 |
| `[a]` | 只按值捕获 `a` |
| `[&a]` | 只按引用捕获 `a` |
| `[=, &a]` | 其他变量按值捕获，`a` 按引用捕获 |
| `[this]` | 捕获当前对象的 `this` 指针，以访问成员 |

按值捕获会把创建 Lambda 时的值保存到闭包对象中，默认不能修改这份副本；按引用捕获直接访问原变量，可以修改原变量，但 Lambda 的使用时间不能超过被引用变量的生命周期。

Lambda 生成的闭包对象中，`operator()` 默认是 `const`，所以按值捕获的成员默认只读。`mutable` 解决的问题是：**让 Lambda 能修改并保存自己的捕获副本状态，同时不修改外部原变量**。

```cpp
int count = 0;

// auto wrong = [count]() { return ++count; }; // 错误：副本默认只读
auto next = [count]() mutable {
    return ++count;
};

next(); // 返回 1
next(); // 返回 2，Lambda 自己保存了状态
// 外部 count 仍然是 0
```

因此，`mutable` 适合计数器、生成器等需要在多次调用之间保存内部状态的 Lambda。它只改变按值捕获副本的可修改性；按引用捕获本来就能修改外部对象，`mutable` 也不表示线程安全。

Lambda 定义后紧跟 `()` 就会立即调用；返回类型既可以推断，也可以显式写成尾置返回类型：

```cpp
int square = [](int x) { return x * x; }(5); // 立即调用，结果为 25

auto divide = [](double a, double b) -> double {
    return a / b;
};
```

嵌套 Lambda 只能使用它自己捕获的变量；需要使用外层作用域的变量时，外层 Lambda 必须先捕获，内层再从外层环境捕获：

```cpp
int value = 10;
auto outer = [value]() mutable {
    auto inner = [&value]() { ++value; };
    inner();
    return value;
};

outer(); // 返回 11；外部原始 value 仍为 10
```

## callback 为什么没有单独定义也能调用

在下面这段代码中，`callback` 不是普通函数名，而是函数参数名。

```cpp
void process(std::function<void()> callback) {
    callback();
}
```

它的类型是：

```cpp
std::function<void()>
```

意思是 `callback` 可以保存一个“无参数、无返回值”的可调用对象。
调用 `process()` 时，可以直接传入 lambda。

```cpp
process([]() {
    cout << "This is callback" << endl;
});
```

这相当于把这个匿名函数传给 `callback` 参数。
进入 `process()` 后，`callback` 里面已经保存了这个 lambda，所以可以写：

```cpp
callback();
```

这不是调用一个提前写好的普通函数，而是调用传进来的函数对象。
可以类比普通参数：

```cpp
void func(int x) {
    cout << x << endl;
}
func(10);
```

`x` 没有提前单独定义，但它是函数参数，所以可以在函数内部使用。
同理，`callback` 也是函数参数，只不过它保存的是一个函数行为。

## std::function 和函数指针的区别

函数指针可以保存普通函数。

```cpp
void hello() {
    cout << "Hello" << endl;
}
void (*fp)() = hello;
fp();
```

但是函数指针不能方便地保存带捕获的 lambda。

```cpp
int x = 10;
// void (*fp)() = [x]() { cout << x << endl; }; // 错误
```

`std::function` 可以保存带捕获的 lambda。

```cpp
int x = 10;
std::function<void()> f = [x]() {
    cout << x << endl;
};
f();
```

简单理解：函数指针更轻量，但功能简单；`std::function` 更灵活，但有一定额外开销。
在嵌入式裸机或资源很小的 MCU 上，经常使用函数指针；在 Linux、上位机、ROS、普通 C++ 应用里，`std::function` 很常见。

## unique_ptr 是什么

`std::unique_ptr` 是 C++ 里的智能指针，头文件是：

```cpp
#include <memory>
```

`std::make_unique<T>(参数)` 会创建一个 `T` 类型的对象，并立即交给 `unique_ptr` 管理。例如：

```cpp
auto number = std::make_unique<int>(42);
std::cout << *number; // 输出 42；*number 取得它管理的 int
```

这里的 `int` 是要创建的类型，圆括号中的 `42` 是传给它的初始化值。类似地，`std::make_unique<Dog>()` 会调用 `Dog` 的无参构造函数；`std::make_unique<Dog>(name)` 会把 `name` 传给 `Dog` 的构造函数。优先使用 `make_unique`，可以避免直接书写 `new`，对象也会在智能指针离开作用域时自动释放。

它可以理解成“自动 delete 的指针”，并且独占它管理的对象。

```cpp
auto p = std::make_unique<Dog>();
p->speak();
```

当 `p` 生命周期结束时，它会自动释放对象，不需要手动 `delete`。
`unique_ptr` 不能复制，只能转移所有权。

```cpp
std::unique_ptr<Dog> p1 = std::make_unique<Dog>();
// std::unique_ptr<Dog> p2 = p1; // 错误
std::unique_ptr<Dog> p2 = std::move(p1); // 正确
```

转移后，`p1` 变空，`p2` 接管对象。
`unique_ptr` 常用于替代裸指针的 `new/delete`，也常和多态一起使用。

```cpp
std::unique_ptr<Animal> a = std::make_unique<Dog>();
a->speak();
```

如果函数只是临时使用对象，一般传引用。

```cpp
void makeSpeak(Animal& animal) {
    animal.speak();
}
auto dog = std::make_unique<Dog>();
makeSpeak(*dog);
```

如果函数要接管对象所有权，才传 `unique_ptr`，并用 `std::move`。

```cpp
void takeAnimal(std::unique_ptr<Animal> animal) {
    animal->speak();
}
auto dog = std::make_unique<Dog>();
takeAnimal(std::move(dog));
```

## shared_ptr 是什么

`std::shared_ptr` 也是智能指针，头文件是：

```cpp
#include <memory>
```

它可以理解成“多个指针共同管理同一个对象”。

```cpp
auto p1 = std::make_shared<Dog>();
auto p2 = p1;
```

这时 `p1` 和 `p2` 共同拥有同一个 `Dog` 对象。
`shared_ptr` 内部有引用计数，记录当前有多少个 `shared_ptr` 正在管理这个对象。

```cpp
cout << p1.use_count() << endl;
```

当最后一个 `shared_ptr` 消失时，对象才会自动释放。

```cpp
std::shared_ptr<Animal> a = std::make_shared<Dog>();
a->speak();
```

如果函数只是使用对象，不保存它，推荐传引用。

```cpp
void makeSpeak(Animal& animal) {
    animal.speak();
}
auto a = std::make_shared<Dog>();
makeSpeak(*a);
```

如果函数要保存对象，让对象在函数结束后继续存在，可以传 `shared_ptr`。

```cpp
void saveAnimal(std::shared_ptr<Animal> animal) {
    animal->speak();
}
auto a = std::make_shared<Dog>();
saveAnimal(a);
```

这会复制一份 `shared_ptr`，引用计数会增加。

## shared_ptr 的循环引用问题

`shared_ptr` 最大的坑是循环引用。

```cpp
class B;
class A {
public:
    std::shared_ptr<B> b;
};
class B {
public:
    std::shared_ptr<A> a;
};
```

如果 `A` 持有 `B`，`B` 又持有 `A`，它们的引用计数可能永远无法变成 0。
解决方法是让其中一边使用 `weak_ptr`。

```cpp
class B {
public:
    std::weak_ptr<A> a;
};
```

`weak_ptr` 只观察对象，不增加引用计数，所以可以打破循环引用。

## unique_ptr、shared_ptr、weak_ptr 的区别

```text
unique_ptr：独占，不能复制，只能 move
shared_ptr：共享，可以复制，有引用计数
weak_ptr：观察，不拥有对象，不增加引用计数
```

一般优先用 `unique_ptr`，确实需要多个地方共享对象时再用 `shared_ptr`。

## 综合例子：多态 + unique_ptr

```cpp
#include <iostream>
#include <memory>
#include <vector>
using namespace std;
class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() = 0;
};
class Dog : public Animal {
public:
    void speak() override {
        cout << "Dog: Woof" << endl;
    }
};
class Cat : public Animal {
public:
    void speak() override {
        cout << "Cat: Meow" << endl;
    }
};
void makeSpeak(Animal& animal) {
    animal.speak();
}
int main() {
    vector<unique_ptr<Animal>> animals;
    animals.push_back(make_unique<Dog>());
    animals.push_back(make_unique<Cat>());
    for (auto& animal : animals) {
        makeSpeak(*animal);
    }
    return 0;
}
```

这个例子里，`vector<unique_ptr<Animal>>` 保存的是基类智能指针，但实际对象可以是 `Dog` 或 `Cat`。
`makeSpeak(Animal& animal)` 使用基类引用接收不同子类对象。
`speak()` 是虚函数，所以运行时会根据真实对象类型调用对应版本。
`unique_ptr` 负责自动释放对象，不需要手动 `delete`。
