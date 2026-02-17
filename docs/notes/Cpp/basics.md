# 引用
引用 = 别名
必须在定义时初始化，一旦绑定，不能再绑其他变量了，引用不能为空
```c
int a = 1, b = 2;
int& r = a;
r = b;   // ❌ 不是改引用对象
//等价于
a = b;
```
# makefile
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

# 数据类型
| 数据类型 | 说明           | 内存大小（字节） |
| -------- | -------------- | -------------- |
| `int`    | 整数           | 4              |
| `long`   | 长整数         | 4 或 8         |  
| `float`  | 单精度浮点数     | 4              |
| `double` | 双精度浮点数     | 8              |
| `char`   | 字符           | 1              |
| `bool`   | 布尔值（true/false） | 1              |

```c
int number = 1; // 整数
(char)number; // 强制类型转换为 char，结果是 '\x01'（ASCII 码为 1 的字符）
char c = '0'+number; // 结果是 '1'（ASCII 码为 48 的字符）
char c2 = to_string(number)[0]; // 结果也是 '1'，to_string(number) 将整数转换为字符串 "1"，然后取第一个字符
```

# const 的用法
    bool stackArray::isEmpty() const{
        return head == -1;
    }这里的 const 保证函数不修改 head 的数值

# 判断
可以用 if(条件) return;else return; 或者 return (条件)?正确结果 : 错误结果
或者直接 retunr(条件);可以获得返回值1或0。

# 默认赋值 
例如stackArray(int size = default_size);出现在.h 文件中表示：如果调用时没给 size 赋值就用 default_size

# 命名空间 
namespace 是用于封装同一领域的一组实体。命名空间就是给名字“分组”的工具，用来避免名字冲突。使用 :: 运算符可以访问命名空间中的成员。

# 全局变量 
是在所有函数外定义的变量，整个程序都能用。其内存程序一启动就分配,程序结束才释放。

# 静态变量 
只创建一次，生命周期贯穿整个程序,即 static int cpt=0 在参数在某函数中一旦被创建，即使再次使用该函数读到这里也不会再次赋值，其内存程序一启动就分配,程序结束才释放。

# this 
是一个指针，指向“当前正在使用这个函数的那个对象”

# 构造函数 
是类的一种特殊成员函数，用于在创建对象时初始化对象。构造函数的名称与类名相同，并且没有返回类型。在不定义构造函数时，编译器会自动生成一个默认构造函数（无参数）。如果定义了带参数的构造函数，编译器将不再生成默认构造函数，因此如果需要无参数构造函数，必须显式定义它。
## 当父类 没有默认构造函数 时
例如：
```cpp
class A {
public:
    A(int x) {}// A 只有一个带参数的构造函数，没有默认构造函数，所以编译器不会自动生成默认构造函数。如果是class A {int x;}，编译器会自动生成一个默认构造函数 A() {}，就不会报错;
};
```
如果你写：
```cpp
class C : public A {
};
```
然后：
```cpp
C c;
```
会报错 ❌

# explicit 单参数构造函数都加 explicit
```cpp
// 如果你写：
Traveler(const std::string& s);
// 你可以这样写：
Traveler t = "Alice";
// 因为：
"Alice" → std::string → Traveler
// 发生了 隐式转换（implicit conversion）。
// 如果你加上：
explicit Traveler(const std::string& s);
// 就禁止这种隐式转换。
// 现在必须写：
Traveler t("Alice");  // 正确
Traveler t = "Alice"; // ❌ 不允许
```
# 析构函数 
类的一种特殊成员函数，用于在对象生命周期结束时执行清理操作。析构函数的名称与类名相同，但前面加上波浪号（~），并且没有返回类型和参数。
没有写析构函数时，编译器会自动生成一个默认析构函数，负责销毁对象时释放资源。 
一定要写析构函数的情况：
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
# 迭代器
迭代器 = 位置
| 容器                      | 迭代器底层          |
| ----------------------- | -------------- |
| `vector` / `string`     | **真·指针** 或接近指针 |
| `deque`                 | 复杂结构           |
| `list`                  | 包装了链表节点的对象     |
| `map` / `unordered_map` | 类对象（红黑树 / 哈希桶） |
```c
string::iterator it;
for (auto it = s.begin(); it != s.end(); ++it) {
    cout << *it << endl;}
//等价于
for (int i = 0; i < s.size(); i++) {
    cout << s[i] << endl;}
```

# auto
什么时候该用 auto（推荐）
1️⃣ 迭代器（首选场景）
``` c
for (auto it = s.begin(); it != s.end(); ++it)
```
原因：
string::iterator 太长
改容器类型时代码不用动
2️⃣ STL 返回值复杂时
```c
auto it = mp.find(key);
//等价于
unordered_map<int, string>::iterator it = mp.find(key);
```

# 双指针

```c
//对撞指针（左右夹逼）   
l = 0, r = n-1;
while (l < r) {...}
//快慢指针（同向）
slow = 0;
for (fast = 0; fast < n; fast++) {
    if (ok(nums[fast])) nums[slow++] = nums[fast];}
//滑动窗口（变长区间）
l = 0;
for (r = 0; r < n; r++) {
    while (invalid) l++;}
//匹配指针
i = 0, j = 0;
while (i < s && j < t) {
    if (s[i] == t[j]) i++;j++;}

```
1️⃣ 有没有“连续子数组”？
有 → 大概率滑动窗口
2️⃣ 数组是否有序？
有 → 大概率双指针
3️⃣ 是否在找“两个数关系”？
有序 → 双指针
无序 → 哈希表
4️⃣ 是否涉及“左边和 == 右边和”？
→ 前缀和

# 时间复杂度（Time Complexity）
O(n) —— 线性时间 例如遍历一个长度为 n 的数组
O(n^2) —— 二次时间 例如嵌套循环遍历一个 n x n 的矩阵
O(log n) —— 对数时间 例如红黑树（set / map）；二分查找；堆操作
哈希表：平均 O(1)
红黑树：O(log n)
暴力循环：O(n)

# 空间复杂度
算法运行时额外占用的内存大小
```c
vector<int> v(n);// O(n) 需要一个长度为 n 的数组来存储元素
int sum(int n) {
    int s = 0;
    for(int i=1;i<=n;i++)
        s += i;
    return s;
}// O(1) 只需要常数空间来存储变量 s 和 i
```
# STL 是什么？
STL（Standard Template Library）= C++ 标准库里的一套通用数据结构 + 算法

# 堆栈的区别
栈 = 自动分配、自动回收的临时内存
堆 = 手动申请、手动（或系统）释放的长期内存
| 中文 | 英语        | 法语       |
| -- | --------- | -------- |
| 栈  | **Stack** | **Pile** |
| 堆  | **Heap**  | **Tas**  |
<ListNode* p = new ListNode(3);>
栈（Stack / Pile）        堆（Heap / Tas）
┌────────────┐           ┌──────────────┐
│ p (地址)   │ ────────▶ │ ListNode{3}  │
└────────────┘           └──────────────┘
指针在栈上，对象在堆上

# vector 的构造函数
vector<T> v(n, value);
创建一个长度为 n 的数组，
每个元素都初始化为 value

# 最大公约数思想
可以用辗转相除法int r = a%b;a = b;b = r;
最大公约数（GCD, Greatest Common Divisor）的函数通常写成：
gcd(a, b)

# 面向对象 = 用“对象”来组织数据 + 操作这些数据的方法
```c
    string s = "HELLO";
    s.erase(2);
    s.push_back('!');
``` 
这里s是一个对象，erase 和 push_back 是操作这个对象的方法

# 链表操作
| 类型    | 模型          |
| ----- | ----------- |
| 删除    | 找前驱         |
| 插入    | 先接后断        |
| 反转    | 三指针         |
| 中点    | 快慢指针        |
| 倒数第 n | 双指针间距       |
| 合并    | dummy + 尾指针 |

# 二叉树 Binary Tree DFS（Depth First Search）深度优先
        1
       / \
      2   3
     / \   \
    4   5   6
前序遍历（根左右） 1 2 4 5 3 6
中序遍历（左根右） 4 2 5 1 3 6
后序遍历（左右根） 4 5 2 6 3 1
```c 
//递归代码对比
void dfs(TreeNode* root) {
    if (!root) return;

    // 前序位置
    cout << root->val;

    dfs(root->left);

    // 中序位置
    cout << root->val;

    dfs(root->right);

    // 后序位置
    cout << root->val;
}
```

# 运算符重载 

C++ 里面没有^运算符
2^31   // ❌ 不是 2 的 31 次方， 而是按位异或 
 2  = 00010
 31  = 11111
---------------- XOR
      11101 = 29

pow(2, 31)   // <cmath>✅ 2 的 31 次方，1 << 31也是 2 的 31 次方


1.  运算符重载是允许程序员为自定义数据类型定义运算符的行为，使得这些运算符可以像内置数据类型一样使用。

2.  a + b ⇔ operator+(a,b) 或 a.operator+(b) 运算符重载只有两种：外部函数 / 内部方法

3.  外部运算符重载 = 把运算符写成一个“普通函数”，不属于类，但可以作用在类对象上是一个 普通函数.所有操作数 都作为参数, 没有 this 指针,通常写在类外面 <const T operator+(T const& a, T const& b);>

4.  内部重载z1.operator+(z2);特点：是类的成员函数,左操作数 = this,右操作数 = 参数

# 抽象类
在 C++ 中：只要一个类中包含至少一个纯虚函数（pure virtual function），这个类就是抽象类
纯虚函数写法：virtual 返回类型 函数名() = 0;
这里的 = 0 不是“等于 0”，而是：C++ 规定：=0 表示这个函数是纯虚函数（pure virtual function）
## 抽象类的核心特点
1️⃣ 不能创建对象
```cpp
class Animal {
public:
    virtual void speak() = 0;  // 纯虚函数
};
Animal a; // ❌ 错误，不能创建抽象类对象
```
2️⃣ 可以作为基类
3️⃣ 子类必须实现所有纯虚函数，否则子类也是抽象类
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
# 继承
继承的核心思想是：如果一个类是另一个类的“特殊情况”，那就可以用继承。
比如：
狗 是 动物
车 是 交通工具
学生 是 人
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
意思是：
C 继承 A，C 自动拥有 A 的所有 public 成员
这个 public 是：继承方式
| 写法        | 意义                      |
| --------- | ----------------------- |
| public    | 保持父类的 public 仍然是 public |
| protected | 父类 public 变成 protected  |
| private   | 父类 public 变成 private    |
如果不写 public默认是：private 继承

好，这个是 C++ 面向对象三大特性之一：

> 封装、继承、多态

# 多态
多态（Polymorphism）字面意思：
> 多种形态
在 C++ 里指：
> 同一个接口，不同对象，表现不同的行为

举个例子：
```cpp
Animal* a = new Dog();
Animal* b = new Cat();

a->speak();   // 汪
b->speak();   // 喵
```
同样是 `speak()`但行为不同。这就是多态。
## 实现多态的三个条件
1️⃣ 继承
2️⃣ 虚函数（virtual）
3️⃣ 基类指针或引用调用
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
* 函数重载
* 运算符重载
* 模板
例子：
```cpp
int add(int a, int b);
double add(double a, double b);
```
编译时就确定调用哪个。

2. 运行时多态（动态多态）
* 通过 virtual 实现
* 通过基类指针调用

## 底层原理
多态靠的是：
> 虚函数表（vtable）

每个有 virtual 的类都会有：vtable
对象内部存一个：vptr（指向虚函数表的指针）
当你调用：a->speak();
实际流程是：
通过 vptr 找到 vtable
→ 找到函数地址
→ 调用真正函数 Dog::speak()

这就是动态绑定（Dynamic Binding）。

## 多态的好处（工程角度）
在你以后做嵌入式架构时：
假设你做：
* UART 驱动
* SPI 驱动
* CAN 驱动
你可以：
```cpp
Device* dev = new UART();
dev->init();
```
主程序根本不关心具体是什么设备。
这叫：
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


