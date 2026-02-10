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
是类的一种特殊成员函数，用于在创建对象时初始化对象。构造函数的名称与类名相同，并且没有返回类型。

# 析构函数 
类的一种特殊成员函数，用于在对象生命周期结束时执行清理操作。析构函数的名称与类名相同，但前面加上波浪号（~），并且没有返回类型和参数。

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

# 容器选择
vector<int> nums; // 创建一个存储整数的动态数组 vector nums;   // ❌ 非法，不知道装什么类型
vector不是一个普通的类，而是一个模板类template <typename T>;class vector { ... };
unordered_map 是一个 “双模板参数”的模板类 template <typename K, typename V>它是一个 哈希表（hash table），本质是：键 → 值（key → value）
unordered_set 是一个 “单模板参数”的模板类 template <typename T>它是一个 哈希集合（hash set），本质是：元素 → 存在与否（element → existence）
```c
#include <unordered_set>
using namespace std;

vector<int> nums; // 创建一个存储整数的动态数组
unordered_set<char> us ={'a','e','i','o','u','A','E','I','O','U'};// 初始化列表 不会自动排序
unordered_map<string, int> cnt = {{"apple", 2},{"banana", 3}};// 初始化列表 不会自动排序
unordered_map<int, int> mp = {{1, 10},{2, 20},{5, 50}};// 初始化列表 不会自动排序 ，哈希表在创建时value的值不确定，访问时会自动创建一个默认值（0）并返回它
set<int> s = {5, 1, 3, 2};// 初始化set 元素会自动排序，自动排序后：{1,2,3,5}
map<int, string> mp2 = {{1, "one"},{3, "three"},{2, "two"}};// 初始化列表，元素会自动排序，自动排序后：{1: "one", 2: "two", 3: "three"}

mp[5] = 10;// 访问 mp 中键为 5 的值，并把值设为10，如果不存在则创建一个默认值（0）并返回它，

for (auto& p : mp) {/*p.first   // key    p.second  // value*/}// 遍历p.first  == 1, 2, 5；p.second == 10, 20, 50

for (int x : s) {/*从小到大*/}// 遍历 set

if (mp.count(5)) {int v = mp[5];}// 查找 mp 中是否存在键为 5 的元素，如果存在则返回 1，否则返回 0；如果存在则访问 mp[5] 的值并赋给 v
if (us.count('a')) {}// 查找
auto it = us.find('a');// 查找 us 中是否存在元素 'a'，如果存在则返回一个指向该元素的迭代器，否则返回 us.end()

us.insert('b');// 插入元素 'b' 到 us 中，如果 'b' 已经存在则不插入
us.erase('a');// 删除元素 'a' 从 us 中，如果 'a' 不存在则不执行任何操作

sort(nums.begin(), nums.end());// 排序，排序后 nums 中的元素将按照从小到大的顺序排列
sort(nums.begin(), nums.end(), greater<int>());// 排序，排序后 nums 中的元素将按照从大到小的顺序排列
```
红黑树是一种“自己会保持平衡的二叉搜索树”它保证：查找 / 插入 / 删除永远是 O(log n)

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

# 时间复杂度（Time Complexity）
O(n) —— 线性时间 例如遍历一个长度为 n 的数组
O(n^2) —— 二次时间 例如嵌套循环遍历一个 n x n 的矩阵
O(log n) —— 对数时间 例如红黑树（set / map）；二分查找；堆操作
哈希表：平均 O(1)
红黑树：O(log n)
暴力循环：O(n)

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

# 最大公约数思想：可以用辗转相除法int r = a%b;a = b;b = r;

# 面向对象 = 用“对象”来组织数据 + 操作这些数据的方法
```c
    string s = "HELLO";
    s.erase(2);
    s.push_back('!');
``` 
这里s是一个对象，erase 和 push_back 是操作这个对象的方法



##  运算符重载
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