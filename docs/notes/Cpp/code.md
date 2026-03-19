# C++ 代码速记

## `<cctype.h>`

如果 `s[i] = '3'`，想得到对应整数 `3`，常见写法是：

```cpp
int x = s[i] - '0';
```

常用字符判断 / 转换函数：

```cpp
isdigit(c);  // 判断是否为数字字符
isalpha(c);  // 判断是否为字母字符
isspace(c);  // 判断是否为空白字符
tolower(c);  // 转成小写
toupper(c);  // 转成大写
```

## 字符串

```cpp
string s1;                     // 创建空字符串
string s2 = "hello";           // 创建并初始化
string s3 = to_string(123);    // 整数转字符串

s1.reserve(s2.size());         // 预分配空间
s1.push_back('h');             // 末尾加一个字符
s1 += "ello";                  // 末尾加一个字符串

string s = "ABCDEFG";
string first3 = s.substr(0, 3);             // "ABC"
string last3 = s.substr(s.size() - 3, 3);   // "EFG"

s.erase(2, 3);                 // 从下标 2 开始删 3 个，结果 "ABFG"
s.erase(n);                    // 保留前 n 个字符，删除剩余部分
s.empty();                     // 判断是否为空
s.clear();                     // 清空字符串

reverse(s.begin(), s.end());               // 反转字符
reverse(words.begin(), words.end());       // 反转字符串数组顺序

auto it = s.begin();            // 迭代器，指向第一个字符
std::string::iterator it2;      // 等价写法

s.insert(it, 'X');              // 开头插入字符
s.insert(pos, count, 'c');      // 在 pos 位置插入 count 个字符 c
s.find('o', pos);               // 从 pos 开始查找字符 'o'
s.append(count, 'c');           // 末尾追加 count 个字符 c
s.back();                       // 取最后一个元素
st.empty();                     // 判断容器是否为空
```

补充：

- `for (char c : s)` 可以遍历字符串中的每个字符。
- `sizeof(s)` 输出的是字符串对象本身占用的大小，不是字符串内容长度；内容长度要用 `s.size()`。

## 循环

```cpp
vector<int> candies(5, 10);  // 创建长度为 5、元素都为 10 的数组

for (int x : candies) {
    // candies 里的每个元素依次赋给 x
}

for (auto it = s.begin(); it != s.end(); ++it) {
    cout << *it << endl;
}

// 等价于
for (int i = 0; i < s.size(); i++) {
    cout << s[i] << endl;
}
```

## 容器选择

通用经验：

- 只有一个接口的容器常用 `push/pop`
- 其他顺序容器更常用 `push_back/pop_back`
- 几乎所有容器都有 `empty()` 和 `size()`
- 只有 `vector`、`string`、`deque` 有 `capacity()`
- 连续线性结构更常见 `front/back`
- 抽象结构更常见 `top`
- 树 / 哈希结构更常见 `begin/end`

容器特点速记：

- `stack<>`：适合频繁在末尾添加 / 删除，只访问栈顶；头文件 `<stack>`；底层通常是 `deque`
- `queue<>`：先进先出；头文件 `<queue>`；底层通常是 `deque`
- `priority_queue<>`：按优先级访问；头文件 `<queue>`；底层通常是堆
- `deque<>`：两端插删快，也支持较频繁访问；头文件 `<deque>`
- `vector<>`：随机访问快，但中间插删不擅长；头文件 `<vector>`
- `list<>`：中间插删方便，但随机访问差；头文件 `<list>`
- `set<>`：唯一元素 + 自动排序；头文件 `<set>`；底层通常是红黑树
- `map<>`：键值对 + 自动排序；头文件 `<map>`；底层通常是红黑树
- `unordered_set<>`：唯一元素 + 哈希查找，不保证顺序；头文件 `<unordered_set>`
- `unordered_map<>`：键值对 + 哈希查找，不保证顺序；头文件 `<unordered_map>`
- `bitset<>`：固定大小位数组；头文件 `<bitset>`

模板类速记：

- `vector` 不是普通类，而是模板类：`template <typename T> class vector { ... };`
- `unordered_map` 是双模板参数：`template <typename K, typename V>`
- `unordered_set` 是单模板参数：`template <typename T>`
- `bitset` 不会动态扩容，大小在编译期就确定：`template <size_t N>`

```cpp
#include <unordered_set>
using namespace std;

vector<int> nums;  // 正确：必须写元素类型
// vector nums;    // 错误：不知道装什么类型

unordered_set<char> us = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
unordered_set<int> s(nums.begin(), nums.end());
unordered_map<string, int> cnt = {{"apple", 2}, {"banana", 3}};
unordered_map<int, int> mp = {{1, 10}, {2, 20}, {5, 50}};
set<int> s2 = {5, 1, 3, 2};               // 自动排序为 {1, 2, 3, 5}
map<int, string> mp2 = {{1, "one"}, {3, "three"}, {2, "two"}};

bitset<8> bs("10101010");
bs.set();
bs.reset();
bs.test(i);

mp[5] = 10;  // 不存在时会自动创建默认值再赋值

for (auto& p : mp) {
    // p.first 是 key，p.second 是 value
}

for (int x : s2) {
    // set 默认从小到大遍历
}

if (mp.count(5)) {
    int v = mp[5];
}

if (us.count('a')) {}
auto it3 = us.find('a');

us.insert('b');
us.erase('a');

auto it4 = find(nums.begin(), nums.end(), 3);
int c = count(nums.begin(), nums.end(), 3);
sort(nums.begin(), nums.end());
sort(nums.begin(), nums.end(), greater<int>());
auto it5 = max_element(nums.begin(), nums.end());
```

补充：

- `vector` 没有 `count()` 和成员版 `find()`。
- `unordered_map` / `unordered_set` 虽然“无序”，但照样支持迭代器遍历。
- `mp.find()` 只能按 `key` 找，不能直接按 `value` 找；找 `value` 往往要自己遍历。
- `nums.begin()` 指向第一个元素；`nums.end()` 指向“最后一个元素的下一个位置”，不能直接取值。

最后记一句：

> 红黑树是一种“自己会保持平衡的二叉搜索树”，因此查找 / 插入 / 删除通常都是 `O(log n)`。
