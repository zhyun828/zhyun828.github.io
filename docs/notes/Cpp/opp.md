# C++ 补充笔记

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

## 循环

```cpp
vector<int> candies(5, 10);  // 创建一个长度为 5 的数组，每个元素初始为 10

for (int x : candies) {
    // 依次把 candies 里的每个元素取出来赋给 x
}
```
