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

    Point operator+(const Point& other) {
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

friend ostream& operator<<(ostream& os, const Point& p) {
    os << p.x << ", " << p.y;
    return os;
}

使用：

cout << p;

可以理解为：

operator<<(cout, p);
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

sort(nums.begin(), nums.end());//sort在algorithm库
sort(nums.begin(), nums.end(), greater<int>());
```

补充：

- `mp.find()` 只能按 `key` 查找，不能直接按 `value` 查找；按 `value` 通常要自己遍历
- 红黑树是一种“自平衡二叉搜索树”，因此查找 / 插入 / 删除通常是 `O(log n)`

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

## STL 是什么？

STL（Standard Template Library）就是 C++ 标准库里的一套通用数据结构和算法。

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

lambda 基本格式是：

```cpp
[捕获列表](参数列表) {
    函数体
};
```

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
