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
| `<utility>`       | 二元组     | `pair` `make_pair`                           | 返回两个值 |
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

## 输入输出控制
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

## Makefile

## 普通编译

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

## 判断

- 可以写成 `if (条件) return; else return;`
- 也可以写成三目运算符：`return (条件) ? 正确结果 : 错误结果;`
- 有时还可以直接 `return (条件);`，这样返回值就是 `1` 或 `0`

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

## `this`

`this` 是一个指针，指向“当前正在使用这个成员函数的那个对象”。

## 构造函数

构造函数是类的一种特殊成员函数，用于在创建对象时初始化对象。构造函数名称与类名相同，没有返回类型。

## 析构函数

析构函数用于在对象生命周期结束时执行清理操作。析构函数名称与类名相同，但前面加波浪号 `~`，并且没有返回类型和参数。

如果没有手写析构函数，编译器会自动生成一个默认析构函数，负责做基础销毁工作。

一定要写析构函数的典型情况：

## 类里有 new 
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
## 类被继承，并且你会用“基类指针删除子类”
这是继承里最危险的坑。
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

// 然后：
Base* p = new Derived();
delete p;

// 输出是：
Base destructor

// 正确做法
// 如果类会被继承，应该写：
class Base {
public:
    virtual ~Base() {}
};
```
## 迭代器

迭代器可以先理解成“位置”。

| 容器 | 迭代器底层 |
| ---- | ---------- |
| `vector` / `string` | 真正的指针，或者非常接近指针 |
| `deque` | 更复杂的结构 |
| `list` | 包装了链表节点的对象 |
| `map` / `unordered_map` | 类对象（红黑树 / 哈希桶） |

```cpp
string::iterator it;
for (auto it = s.begin(); it != s.end(); ++it) {
    cout << *it << endl;
}

// 等价于
for (int i = 0; i < s.size(); i++) {
    cout << s[i] << endl;
}
```

## `auto`

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

## 容器选择

先记两件事：

- `vector<int> nums;` 是对的；`vector nums;` 是错的，因为没写元素类型
- `unordered_map` / `unordered_set` 虽然“无序”，但并不代表不能遍历

补充理解：

- `vector` 是模板类：`template <typename T> class vector { ... };`
- `unordered_map` 是双模板参数，本质是“键 -> 值”
- `unordered_set` 是单模板参数，本质是“元素是否存在”
- `unordered_map<int, vector<int>>` 这类写法默认不行，因为 `vector<int>` 没有现成哈希函数

```cpp
#include <unordered_set>
using namespace std;

vector<int> nums;

unordered_set<char> us = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
unordered_map<string, int> cnt = {{"apple", 2}, {"banana", 3}};
unordered_map<int, int> mp = {{1, 10}, {2, 20}, {5, 50}};
set<int> s = {5, 1, 3, 2};  // 自动排序后为 {1, 2, 3, 5}
map<int, string> mp2 = {{1, "one"}, {3, "three"}, {2, "two"}};

mp[5] = 10;

for (auto& p : mp) {
    // p.first 是 key，p.second 是 value
}

for (int x : s) {
    // set 默认从小到大遍历
}

if (mp.count(5)) {
    int v = mp[5];
}

if (us.count('a')) {}
auto it = us.find('a');

us.insert('b');
us.erase('a');

sort(nums.begin(), nums.end());
sort(nums.begin(), nums.end(), greater<int>());
```

补充：

- `mp.find()` 只能按 `key` 查找，不能直接按 `value` 查找；按 `value` 通常要自己遍历
- 红黑树是一种“自平衡二叉搜索树”，因此查找 / 插入 / 删除通常是 `O(log n)`

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

## STL 是什么？

STL（Standard Template Library）就是 C++ 标准库里的一套通用数据结构和算法。

## 堆和栈的区别

- 栈：自动分配、自动回收的临时内存
- 堆：手动申请、手动（或系统）释放的长期内存

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

## `vector` 的构造函数

```cpp
vector<T> v(n, value);
```

表示创建一个长度为 `n` 的数组，并把每个元素都初始化为 `value`。

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

## 运算符重载
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

## 十、和抽象类的关系

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


