## <cctype.h>
对于一个s[i]='3'的字符，想要得到它对应的整数3，可以使用以下方法 
```c
int x = s[i] - '0';
```
```c
isdigit(c) // 判断字符 c 是否为数字字符（0-9），返回一个非零值（true）如果是数字，否则返回 0（false） 
isalpha(c) // 判断字符 c 是否为字母字符（a-z 或 A-Z），返回一个非零值（true）如果是字母，否则返回 0（false） 
isspace(c) // 判断字符 c 是否为空白字符（空格、制表符、换行符等），返回一个非零值（true）如果是空白字符，否则返回 0（false） 
tolower(c) // 将字符 c 转换为小写字母，如果 c 是大写字母则转换，否则返回 c 本身 
toupper(c) // 将字符 c 转换为大写字母，如果 c 是小写字母则转换，否则返回 c 本身 ```
```
## 字符串
```c
string s1 ;//创建一个空字符串
string  s2 = "hello"; //创建一个字符串并初始化
s1.reserve(s2.size()); //预分配(s2.size())个字符的空间
s1.push_back('h'); //在字符串末尾添加一个字符
s1+= "ello"; //在字符串末尾添加一个字符串
cout << sizeof(s) << endl;//输出字符串对象占用的内存大小
//取前 n 个字符取后 n 个字符
string s = "ABCDEFG";
string first3 = s.substr(0, 3);   // "ABC"
string last3 = s.substr(s.size() - 3, 3);  // "EFG"
s.erase(2, 3);   // 从下标2开始删3个// 结果： "ABFG"
s.erase(n);   // 保留前 n 个字符，删除剩余的
s.empty();   // 判断字符串是否为空，bool类型
s.clear();   // 清空字符串
for (char c : s) // 用char c遍历字符串s中的每个字符
reverse(s.begin(), s.end());      // 反转字符
reverse(words.begin(), words.end()); // 反转字符串顺序。其中words是一个字符串数组vector<string> words

auto it = s.begin();// 获取字符串的迭代器，指向字符串的第一个字符 
// 等价于
std::string::iterator it;

s.insert(it, 'X'); // 在字符串开头插入字符 'X'
s.insert(pos, count, 'c');// 在字符串的 pos 位置插入 count 个字符 c;只有string类有这个重载版本，其他容器没有
s.find('o', pos); // 查找字符 'o' 的位置，从 pos 开始查找，返回第一个匹配的索引，如果没有找到则返回 string::npos
s.append(count, 'c'); // 在字符串末尾添加 count 个字符 c;只有string类有这个重载版本，其他容器没有
s.back()//back() 是：vector，string，deque的成员函数，返回一个引用，指向容器中的最后一个元素（栈顶）
st.empty()//empty() 是：vector，string，deque，set，map，unordered_set，unordered_map的成员函数，返回一个布尔值，表示容器是否为空
```
## 循环
```c
vector<int> candies(5, 10); // 创建一个长度为 5 的数组，每个元素都初始化为 10
for (int x : candies){}把 candies 里的每一个元素，依次取出来，赋给 x

string::iterator it;
for (auto it = s.begin(); it != s.end(); ++it) {
    cout << *it << endl;}
//等价于
for (int i = 0; i < s.size(); i++) {
    cout << s[i] << endl;}

```
## 容器选择
只有一个接口的容器用push/pop，其他的用push_back和pop_back
几乎所有的容器都有empty和size函数，只有vector，string，deque有capacity函数
连续线性结构 → front/back，抽象结构 → top，树 / 哈希 → begin/end

stack<>适合：需要频繁在末尾添加/删除元素的场景，且只需要访问栈顶元素。 头文件`<stack>`，底层通常使用 `deque` 实现，提供了 `push`、`pop` 和 `top` 等成员函数。
queue<>适合：只能先进先出（FIFO）头文件`<queue>`，底层通常使用 `deque` 实现，提供了 `push`、`pop` 和 `front` 等成员函数。 priority_queue<>适合：需要按照优先级访问元素的场景。头文件`<queue>`，底层通常使用堆（heap）实现，提供了 `push`、`pop` 和 `top` 等成员函数。
deque<>适合：需要频繁在两端添加/删除元素的场景，且需要频繁访问元素的场景。头文件`<deque>`，底层使用双端队列实现，提供了 `push_back`、`push_front`、`pop_back`、`pop_front` 等成员函数。
vector<>适合：需要频繁访问元素的场景，且不需要频繁在中间添加/删除元素。 deque<>适合：需要频繁在两端添加/删除元素的场景，且需要频繁访问元素的场景。 头文件`<vector>`，底层使用动态数组实现，提供了 `push_back`、`pop_back` 等成员函数，并且支持随机访问。
list<>适合：需要频繁在中间添加/删除元素的场景，且不需要频繁访问元素的场景。头文件`<list>`，底层使用双向链表实现，提供了 `push_back`、`push_front`、`pop_back`、`pop_front` 等成员函数，并且支持双向迭代器。
set<>适合：需要存储唯一元素并进行快速查找的场景。头文件`<set>`，底层使用红黑树实现，提供了 `insert`、`erase`、`find` 等成员函数，并且自动排序元素。 
map<>适合：需要存储键值对并进行快速查找的场景。头文件`<map>`，底层使用红黑树实现，提供了 `insert`、`erase`、`find` 等成员函数，并且自动排序键值对。
unordered_set<>适合：需要存储唯一元素并进行快速查找的场景，但不关心元素的顺序。 头文件`<unordered_set>`，底层使用哈希表实现，提供了 `insert`、`erase`、`find` 等成员函数，并且不保证元素的顺序。
unordered_map<>适合：需要存储键值对并进行快速查找的场景，但不关心键值对的顺序。头文件`<unordered_map>`，底层使用哈希表实现，提供了 `insert`、`erase`、`find` 等成员函数，并且不保证键值对的顺序。 
bitset<>适合：需要存储大量布尔值并进行位操作的场景。头文件`<bitset>`，底层使用固定大小的位数组实现，提供了 `set`、`reset`、`test` 等成员函数，并且支持位操作。


vector<int> nums; // 创建一个存储整数的动态数组 vector nums;   // ❌ 非法，不知道装什么类型
vector不是一个普通的类，而是一个模板类template <typename T>;class vector { ... };
unordered_map 是一个 “双模板参数”的模板类 template <typename K, typename V>它是一个 哈希表（hash table），本质是：键 → 值（key → value）
unordered_set 是一个 “单模板参数”的模板类 template <typename T>它是一个 哈希集合（hash set），本质是：元素 → 存在与否（element → existence）
bitset 不会动态扩容，只能在编译时指定大小 template <size_t N> 是一串固定长度的 0/1 位
```c
#include <unordered_set>
using namespace std;

vector<int> nums; // 创建一个存储整数的动态数组，vector没有count和find函数
unordered_set<char> us ={'a','e','i','o','u','A','E','I','O','U'};// 初始化列表 不会自动排序
unordered_set<int> s(nums.begin(), nums.end());// 通过迭代器范围构造一个 unordered_set，包含 nums 中的所有元素
unordered_map<string, int> cnt = {{"apple", 2},{"banana", 3}};// 初始化列表 不会自动排序
unordered_map<int, int> mp = {{1, 10},{2, 20},{5, 50}};// 初始化列表 不会自动排序 ，哈希表在创建时value的值不确定，访问时会自动创建一个默认值（0）并返回它
set<int> s = {5, 1, 3, 2};// 初始化set 元素会自动排序，自动排序后：{1,2,3,5}
map<int, string> mp2 = {{1, "one"},{3, "three"},{2, "two"}};// 初始化列表，元素会自动排序，自动排序后：{1: "one", 2: "two", 3: "three"}
bitset<8> bs("10101010"); // 创建一个长度为 8 的 bitset，并用字符串 "10101010" 初始化它
bs.set(); // 将 bitset 中的所有位设置为 1
bs.reset(); // 将 bitset 中的所有位重置为 0
bs.test(i);   // 查看第 i 位是否为 1，返回一个布尔值
mp[5] = 10;// 访问 mp 中键为 5 的值，并把值设为10，如果不存在则创建一个默认值（0）并返回它，

for (auto& p : mp) {s.insert(p.first);s.insert(p.second);}// 遍历 mp 中的每个键值对 p，将键 p.first 和值 p.second 插入到 set s 中

for (int x : s) {/*从小到大*/}// 遍历 set

if (mp.count(5)) {int v = mp[5];}// 查找 mp 中是否存在键为 5 的元素，如果存在则返回 1，否则返回 0；如果存在则访问 mp[5] 的值并赋给 v
if (us.count('a')) {}// 查找
auto it = us.find('a');// 查找 us 中是否存在元素 'a'，如果存在则返回一个指向该元素的迭代器，否则返回 us.end()

us.insert('b');// 插入元素 'b' 到 us 中，如果 'b' 已经存在则不插入
us.erase('a');// 删除元素 'a' 从 us 中，如果 'a' 不存在则不执行任何操作

auto it = find(nums.begin(), nums.end(), 3);// 在 v 中查找元素 3，返回一个指向第一个匹配元素的迭代器，如果没有找到则返回 v.end()
int c = count(nums.begin(), nums.end(), 3);// 统计 v 中元素 3 出现的次数，返回一个整数
sort(nums.begin(), nums.end());// 排序，排序后 nums 中的元素将按照从小到大的顺序排列
sort(nums.begin(), nums.end(), greater<int>());// 排序，排序后 nums 中的元素将按照从大到小的顺序排列
```
红黑树是一种“自己会保持平衡的二叉搜索树”它保证：查找 / 插入 / 删除永远是 O(log n)