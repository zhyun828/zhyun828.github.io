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



```
## 循环
```c
vector<int> candies(5, 10); // 创建一个长度为 5 的数组，每个元素都初始化为 10
for (int x : candies){}把 candies 里的每一个元素，依次取出来，赋给 x
```
