# C++ 设计模式详解（扩展版）

> 这是一份面向 **初学者 + 面试准备 + 实战理解** 的 C++ 设计模式笔记。  
> 与普通“背定义”的版本不同，这一版会尽量讲清楚：
>
> - 模式到底解决什么问题
> - 为什么这样设计
> - 什么时候用，什么时候不要用
> - 推荐写法怎么真正使用
> - 常见变体（例如单例模式的饿汉式 / 懒汉式 / 线程安全）
> - 代码都尽量附上注释
>
> ---
>
> 设计模式通常分为三大类：
>
> 1. **创建型模式**：关注“对象怎么创建更合理”  
> 2. **结构型模式**：关注“类和对象怎么组织更合理”  
> 3. **行为型模式**：关注“对象之间怎么协作更合理”
>
> GoF（Gang of Four）经典设计模式一共 **23 种**。

---

# 一、学习设计模式前先明白的事

## 1. 设计模式不是语法，而是“经验总结”

设计模式不是 C++ 关键字，也不是标准库里的固定 API。  
它本质上是：

**前人反复遇到某类问题后，总结出来的一种通用解决思路。**

所以学习设计模式时，不能只记“定义”，而要记：

- 它解决什么问题
- 它为什么成立
- 它的代价是什么
- 它和别的方案相比有什么差别

## 2. 不要为了用模式而用模式

设计模式的目标是：

- 提高可维护性
- 降低耦合
- 提高扩展性
- 让代码更容易理解

如果你的代码本来就很简单，却硬套很多模式，反而会：

- 结构变复杂
- 类变多
- 阅读成本增加
- 调试更困难

所以原则是：

**先写清楚，再考虑是否需要模式。**

## 3. 现代 C++ 中的设计模式和老教材不完全一样

很多老书中的设计模式代码更偏向 Java/C++98 风格。  
而现代 C++（C++11/14/17/20）有：

- `unique_ptr` / `shared_ptr`
- `std::function`
- lambda
- 局部静态变量线程安全
- 模板
- move 语义
- RAII

所以今天写设计模式时，通常要结合现代 C++ 来写，而不是机械照搬老代码。

---

# 二、创建型模式（Creational Patterns）

创建型模式关注的是：**对象如何创建**。

包括：

1. 单例模式（Singleton）
2. 工厂方法模式（Factory Method）
3. 抽象工厂模式（Abstract Factory）
4. 建造者模式（Builder）
5. 原型模式（Prototype）

---

# 1. 单例模式（Singleton）

## 1.1 核心思想

单例模式的目标是：

> **一个类在整个程序中只允许存在一个实例，并且提供一个全局访问点。**

例如：

- 配置管理器
- 日志管理器
- 线程池管理器
- 数据库连接池管理器
- 资源管理器

这些对象通常不希望被随意创建多个。

---

## 1.2 为什么要把构造函数私有化

如果构造函数是 `public`，那外部就可以这样写：

```cpp
Singleton a;
Singleton b;
```

这样就创建出多个对象了，违背了“全局只能有一个实例”的要求。

所以单例模式通常会：

- **把构造函数私有化**：外部不能直接创建对象
- **把拷贝构造和赋值也禁用**：避免复制出第二个对象
- **提供一个静态函数 `getInstance()`**：外部只能通过它拿到唯一对象

这就是“限制创建入口”的核心思想。

---

## 1.3 单例模式最基本写法

```cpp
#include <iostream>
using namespace std;

class Singleton {
private:
    // 1) 构造函数私有化：外部不能写 Singleton s;
    Singleton() {
        cout << "Singleton constructor called\n";
    }

    // 2) 禁止拷贝构造：防止 Singleton s2 = s1;
    Singleton(const Singleton&) = delete;

    // 3) 禁止赋值：防止 s2 = s1;
    Singleton& operator=(const Singleton&) = delete;

    // 4) 唯一实例的静态指针
    static Singleton* instance;

public:
    // 5) 提供全局访问点
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

    void doSomething() {
        cout << "Singleton is working\n";
    }
};

// 静态成员类外初始化
Singleton* Singleton::instance = nullptr;

int main() {
    // 外部不能这样写：Singleton s;  因为构造函数是 private

    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();

    s1->doSomething();
    s2->doSomething();

    // 地址相同，说明拿到的是同一个对象
    cout << s1 << endl;
    cout << s2 << endl;

    return 0;
}
```

---

## 1.4 “推荐写法”到底怎么使用

很多人看到“构造函数私有化，外部不能创建对象”，但不知道实际怎么用。

### 外部不能这样用

```cpp
Singleton s;          // 错误：构造函数 private
Singleton s2(s);      // 错误：拷贝构造 deleted
Singleton s3 = s;     // 错误：拷贝构造 deleted
```

### 只能这样用

```cpp
Singleton* s = Singleton::getInstance();
s->doSomething();
```

也就是说：

- 你不是“自己 new 一个对象”
- 你不是“自己定义一个对象”
- 你是“向类要那个唯一对象”

这就是单例模式的使用方式。

它像一个“全局唯一柜台”：

- 柜台不让你自己开分店
- 你只能去官方窗口办理业务

---

## 1.5 单例模式的几种常见写法

单例模式最常见的面试点就在这里。

### A. 饿汉式（Eager Singleton）

#### 思想

类加载时、程序启动早期，**先把唯一实例创建好**。  
也就是说：不等你第一次调用 `getInstance()`，对象就已经存在了。

#### 代码

```cpp
#include <iostream>
using namespace std;

class EagerSingleton {
private:
    EagerSingleton() {
        cout << "EagerSingleton constructor\n";
    }

    EagerSingleton(const EagerSingleton&) = delete;
    EagerSingleton& operator=(const EagerSingleton&) = delete;

    // 程序开始时就创建好对象
    static EagerSingleton instance;

public:
    static EagerSingleton& getInstance() {
        return instance;
    }

    void work() {
        cout << "Eager singleton working\n";
    }
};

// 静态对象在 main 之前就会构造
EagerSingleton EagerSingleton::instance;

int main() {
    EagerSingleton& s = EagerSingleton::getInstance();
    s.work();
    return 0;
}
```

#### 优点

- 实现简单
- 天然避免“第一次创建时的竞争问题”
- 如果编译器和运行时环境正常，通常线程安全

#### 缺点

- 即使程序根本不用这个对象，它也会提前创建
- 可能造成启动期开销
- 如果这个对象依赖其他静态对象，可能遇到 **静态初始化顺序问题**

#### 什么时候适合

- 单例对象很轻量
- 程序启动后一定会用到
- 不在意提前构造

---

### B. 懒汉式（Lazy Singleton）

#### 思想

**第一次真正需要时再创建对象。**  
这叫“延迟初始化”。

#### 非线程安全版本

```cpp
#include <iostream>
using namespace std;

class LazySingleton {
private:
    LazySingleton() {
        cout << "LazySingleton constructor\n";
    }

    LazySingleton(const LazySingleton&) = delete;
    LazySingleton& operator=(const LazySingleton&) = delete;

    static LazySingleton* instance;

public:
    static LazySingleton* getInstance() {
        // 第一次调用时才创建
        if (instance == nullptr) {
            instance = new LazySingleton();
        }
        return instance;
    }

    void work() {
        cout << "Lazy singleton working\n";
    }
};

LazySingleton* LazySingleton::instance = nullptr;
```

#### 优点

- 用到时才创建
- 节省不必要的初始化开销

#### 缺点

- **多线程下不安全**

---

## 1.6 为什么懒汉式会有线程安全问题

假设有两个线程同时第一次调用 `getInstance()`：

```cpp
if (instance == nullptr) {
    instance = new LazySingleton();
}
```

可能发生：

- 线程 A 判断 `instance == nullptr`，成立
- 线程 B 也判断 `instance == nullptr`，也成立
- A 创建一个对象
- B 又创建一个对象

结果：创建了两个实例。  
这就违反了单例模式的要求。

所以说：

> 懒汉式如果不加同步控制，在多线程环境下通常是不安全的。

---

## 1.7 线程安全的懒汉式：加锁版本

```cpp
#include <iostream>
#include <mutex>
using namespace std;

class ThreadSafeSingleton {
private:
    ThreadSafeSingleton() {
        cout << "ThreadSafeSingleton constructor\n";
    }

    ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
    ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;

    static ThreadSafeSingleton* instance;
    static mutex mtx;  // 用于保护第一次创建过程

public:
    static ThreadSafeSingleton* getInstance() {
        lock_guard<mutex> lock(mtx);  // 自动加锁/解锁

        if (instance == nullptr) {
            instance = new ThreadSafeSingleton();
        }
        return instance;
    }

    void work() {
        cout << "Thread-safe singleton working\n";
    }
};

ThreadSafeSingleton* ThreadSafeSingleton::instance = nullptr;
mutex ThreadSafeSingleton::mtx;
```

#### 为什么这样是线程安全的

因为同一时刻只有一个线程能进入加锁区域：

- 第一个线程进来，发现为空，于是创建对象
- 第二个线程只能等第一个线程结束后再进来
- 第二个线程进来时，发现对象已经存在，不会再创建

#### 缺点

- 每次调用 `getInstance()` 都要加锁
- 即使对象已经创建好了，后续访问还会有锁开销

---

## 1.8 双重检查锁（Double-Checked Locking）

它想优化“每次都加锁”的问题。

```cpp
#include <iostream>
#include <mutex>
using namespace std;

class DCLSingleton {
private:
    DCLSingleton() {
        cout << "DCLSingleton constructor\n";
    }

    DCLSingleton(const DCLSingleton&) = delete;
    DCLSingleton& operator=(const DCLSingleton&) = delete;

    static DCLSingleton* instance;
    static mutex mtx;

public:
    static DCLSingleton* getInstance() {
        // 第一次检查：大部分时候对象已存在，可直接返回
        if (instance == nullptr) {
            lock_guard<mutex> lock(mtx);

            // 第二次检查：防止多个线程排队进入后重复创建
            if (instance == nullptr) {
                instance = new DCLSingleton();
            }
        }
        return instance;
    }
};

DCLSingleton* DCLSingleton::instance = nullptr;
mutex DCLSingleton::mtx;
```

#### 说明

这个写法在很多教材里很常见，但在不同语言/旧内存模型下容易出坑。  
在现代 C++ 中，如果不结合更严格的内存序和原子语义，初学阶段不建议把它当作首选写法。

面试时你可以这样说：

> 双重检查锁是为了减少每次访问都加锁的开销，但实现细节容易出错。现代 C++ 更推荐使用函数内局部静态对象实现单例。

---

## 1.9 现代 C++ 推荐写法：Meyers Singleton

这是今天最推荐掌握的写法。

```cpp
#include <iostream>
using namespace std;

class Singleton {
private:
    Singleton() {
        cout << "Singleton constructor\n";
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton& getInstance() {
        // C++11 起，函数内局部静态变量的初始化是线程安全的
        static Singleton instance;
        return instance;
    }

    void doSomething() {
        cout << "Doing something...\n";
    }
};

int main() {
    // 注意这里拿到的是引用
    Singleton& s1 = Singleton::getInstance();
    Singleton& s2 = Singleton::getInstance();

    s1.doSomething();

    // 取地址后可看到是同一个对象
    cout << &s1 << endl;
    cout << &s2 << endl;

    return 0;
}
```

### 为什么推荐它

#### 1）懒加载

只有第一次调用 `getInstance()` 才会创建对象。

#### 2）线程安全

从 **C++11** 开始，标准保证：

> 函数内部局部静态变量的初始化过程是线程安全的。

这意味着多个线程第一次同时调用时，不会构造出多个对象。

#### 3）不需要自己 `new`

避免了手动内存管理问题。

#### 4）生命周期自动管理

程序结束时会自动析构。

---

## 1.10 单例模式的完整推荐版

```cpp
#include <iostream>
using namespace std;

class Logger {
private:
    // 构造函数私有化，防止外部随意创建对象
    Logger() {
        cout << "Logger created\n";
    }

    // 禁止拷贝，避免复制出第二个对象
    Logger(const Logger&) = delete;

    // 禁止赋值
    Logger& operator=(const Logger&) = delete;

public:
    // 提供唯一访问入口
    static Logger& getInstance() {
        // 局部静态对象：懒加载 + C++11 线程安全
        static Logger instance;
        return instance;
    }

    void log(const string& msg) {
        cout << "[LOG] " << msg << endl;
    }
};

int main() {
    // 正确使用方式：通过 getInstance() 获取唯一对象
    Logger& logger = Logger::getInstance();

    logger.log("Program started");
    logger.log("Loading config...");

    // 再拿一次，仍然是同一个对象
    Logger& logger2 = Logger::getInstance();
    logger2.log("Still the same logger");

    // 比较地址
    cout << &logger << endl;
    cout << &logger2 << endl;

    return 0;
}
```

---

## 1.11 单例模式适用场景

适合：

- 日志系统
- 配置系统
- 线程池管理器
- 缓存管理器
- 资源注册中心

不适合：

- 业务对象到处都做成单例
- 需要多个独立实例的场景
- 测试中需要灵活替换对象的场景

---

## 1.12 单例模式优缺点

### 优点

- 保证全局唯一实例
- 避免重复创建高代价对象
- 便于提供统一访问入口

### 缺点

- 全局状态会增加耦合
- 单元测试不友好
- 生命周期难控制时可能产生隐患
- 滥用后会让代码像“到处可访问的全局变量”

---

## 1.13 面试回答模板

### 什么是单例模式

单例模式是一种创建型模式，它保证某个类在整个程序运行期间只有一个实例，并提供一个全局访问点。典型做法是把构造函数私有化，禁止拷贝和赋值，然后通过静态成员函数返回唯一实例。

### 为什么构造函数要私有化

因为如果构造函数是公有的，外部就可以随意创建多个对象，这就违背了单例“唯一实例”的要求。私有化后，类外只能通过 `getInstance()` 获取对象。

### 单例有哪些写法

常见有饿汉式、懒汉式、加锁懒汉式、双重检查锁、Meyers Singleton。现代 C++ 最推荐的是函数内局部静态对象，因为它既支持懒加载，又能利用 C++11 保证初始化线程安全。

---

# 2. 工厂方法模式（Factory Method）

## 2.1 核心思想

工厂方法模式的目标是：

> **把“对象创建”这件事交给子类决定，而不是把 new 写死在业务代码里。**

也就是说，客户端关心“我要一个产品”，但不关心“产品是怎么 new 出来的”。

---

## 2.2 为什么需要它

假设你有一个图形系统：

- `Circle`
- `Rectangle`

如果你在业务代码里直接写：

```cpp
Shape* s = new Circle();
```

那么业务代码就和具体类 `Circle` 耦合死了。  
以后想替换成 `Rectangle` 或新增 `Triangle`，就要改业务代码。

工厂方法模式把“创建哪种对象”封装起来，从而降低耦合。

---

## 2.3 结构

- 抽象产品（Product）
- 具体产品（ConcreteProduct）
- 抽象工厂（Creator）
- 具体工厂（ConcreteCreator）

---

## 2.4 代码示例

```cpp
#include <iostream>
#include <memory>
using namespace std;

// 抽象产品
class Shape {
public:
    virtual void draw() = 0;
    virtual ~Shape() = default;
};

// 具体产品：圆形
class Circle : public Shape {
public:
    void draw() override {
        cout << "Draw Circle\n";
    }
};

// 具体产品：矩形
class Rectangle : public Shape {
public:
    void draw() override {
        cout << "Draw Rectangle\n";
    }
};

// 抽象工厂
class ShapeFactory {
public:
    virtual unique_ptr<Shape> createShape() = 0;
    virtual ~ShapeFactory() = default;
};

// 具体工厂：专门创建 Circle
class CircleFactory : public ShapeFactory {
public:
    unique_ptr<Shape> createShape() override {
        return make_unique<Circle>();
    }
};

// 具体工厂：专门创建 Rectangle
class RectangleFactory : public ShapeFactory {
public:
    unique_ptr<Shape> createShape() override {
        return make_unique<Rectangle>();
    }
};

int main() {
    unique_ptr<ShapeFactory> factory = make_unique<CircleFactory>();

    // 客户端并不知道具体 new 了什么，只知道拿到了一个 Shape
    unique_ptr<Shape> shape = factory->createShape();
    shape->draw();

    return 0;
}
```

---

## 2.5 适用场景

- 创建逻辑复杂
- 希望把创建和使用分离
- 未来可能扩展更多具体类型

## 2.6 优点

- 降低客户端和具体类耦合
- 更利于扩展
- 符合开闭原则

## 2.7 缺点

- 类数量增加
- 结构更复杂

---

# 3. 抽象工厂模式（Abstract Factory）

## 3.1 核心思想

抽象工厂模式解决的是：

> **创建一整组相互关联的对象，而不是单个对象。**

例如你在做跨平台 UI：

- Windows 风格按钮 + Windows 风格文本框
- Mac 风格按钮 + Mac 风格文本框

它们是一整套“同风格产品族”。

---

## 3.2 和工厂方法的区别

- **工厂方法**：通常创建一种产品
- **抽象工厂**：通常创建一组相关产品

---

## 3.3 代码示例

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Button {
public:
    virtual void paint() = 0;
    virtual ~Button() = default;
};

class TextBox {
public:
    virtual void show() = 0;
    virtual ~TextBox() = default;
};

class WinButton : public Button {
public:
    void paint() override {
        cout << "Windows Button\n";
    }
};

class WinTextBox : public TextBox {
public:
    void show() override {
        cout << "Windows TextBox\n";
    }
};

class MacButton : public Button {
public:
    void paint() override {
        cout << "Mac Button\n";
    }
};

class MacTextBox : public TextBox {
public:
    void show() override {
        cout << "Mac TextBox\n";
    }
};

class GUIFactory {
public:
    virtual unique_ptr<Button> createButton() = 0;
    virtual unique_ptr<TextBox> createTextBox() = 0;
    virtual ~GUIFactory() = default;
};

class WinFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<WinButton>();
    }

    unique_ptr<TextBox> createTextBox() override {
        return make_unique<WinTextBox>();
    }
};

class MacFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<MacButton>();
    }

    unique_ptr<TextBox> createTextBox() override {
        return make_unique<MacTextBox>();
    }
};
```

---

## 3.4 适用场景

- 要创建“产品族”
- 不希望混用不同风格产品
- 系统需要切换整套实现

## 3.5 优缺点

优点：

- 保证同一产品族的一致性
- 客户端无需关心具体类

缺点：

- 新增“产品等级结构”很麻烦
- 类数量会增加

---

# 4. 建造者模式（Builder）

## 4.1 核心思想

建造者模式解决的是：

> **一个复杂对象的创建过程很复杂，希望把“构建步骤”和“最终对象”分离。**

例如：

- 组装电脑
- 组装 SQL 查询
- 构造复杂配置对象
- 构造 HTTP 请求对象

---

## 4.2 为什么需要它

如果一个对象有很多参数，直接用构造函数会变成：

```cpp
Person p("Zhang", 20, "Xi'an", "student", true, false, 178, 70);
```

这种可读性很差，也容易传错顺序。

Builder 的思想是：一步一步构造。

---

## 4.3 代码示例

```cpp
#include <iostream>
#include <string>
using namespace std;

class Computer {
public:
    string cpu;
    string gpu;
    string memory;
    string disk;

    void show() const {
        cout << "CPU: " << cpu << endl;
        cout << "GPU: " << gpu << endl;
        cout << "Memory: " << memory << endl;
        cout << "Disk: " << disk << endl;
    }
};

class ComputerBuilder {
private:
    Computer computer;

public:
    ComputerBuilder& setCPU(const string& c) {
        computer.cpu = c;
        return *this; // 返回自身，支持链式调用
    }

    ComputerBuilder& setGPU(const string& g) {
        computer.gpu = g;
        return *this;
    }

    ComputerBuilder& setMemory(const string& m) {
        computer.memory = m;
        return *this;
    }

    ComputerBuilder& setDisk(const string& d) {
        computer.disk = d;
        return *this;
    }

    Computer build() {
        return computer;
    }
};

int main() {
    Computer pc = ComputerBuilder()
                    .setCPU("Intel i7")
                    .setGPU("RTX 4060")
                    .setMemory("16GB")
                    .setDisk("1TB SSD")
                    .build();

    pc.show();
    return 0;
}
```

---

## 4.4 适用场景

- 对象创建步骤多
- 参数多，构造函数可读性差
- 希望分步骤构造

## 4.5 优缺点

优点：

- 可读性强
- 构建过程清晰
- 适合复杂对象

缺点：

- 代码量会增加
- 对简单对象可能显得过度设计

---

# 5. 原型模式（Prototype）

## 5.1 核心思想

原型模式解决的是：

> **通过复制已有对象来创建新对象，而不是每次都重新构造。**

适用于：

- 创建对象成本高
- 某对象初始状态复杂
- 希望快速复制一份类似对象

---

## 5.2 代码示例

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Prototype {
public:
    virtual unique_ptr<Prototype> clone() const = 0;
    virtual void show() const = 0;
    virtual ~Prototype() = default;
};

class ConcretePrototype : public Prototype {
private:
    int value;

public:
    ConcretePrototype(int v) : value(v) {}

    unique_ptr<Prototype> clone() const override {
        // 调用拷贝构造复制自己
        return make_unique<ConcretePrototype>(*this);
    }

    void show() const override {
        cout << "value = " << value << endl;
    }
};

int main() {
    unique_ptr<Prototype> p1 = make_unique<ConcretePrototype>(42);
    unique_ptr<Prototype> p2 = p1->clone();

    p1->show();
    p2->show();

    return 0;
}
```

---

## 5.3 浅拷贝和深拷贝

这是原型模式常见坑点。

如果对象内部有指针、动态内存、文件句柄等资源，复制时必须考虑：

- **浅拷贝**：只复制指针值，两个对象指向同一块资源
- **深拷贝**：复制资源本身，每个对象有独立副本

所以原型模式经常和“拷贝控制”一起考。

---

# 三、结构型模式（Structural Patterns）

结构型模式关注：**类和对象如何组织得更合理**。

包括：

1. 适配器模式（Adapter）
2. 桥接模式（Bridge）
3. 组合模式（Composite）
4. 装饰器模式（Decorator）
5. 外观模式（Facade）
6. 享元模式（Flyweight）
7. 代理模式（Proxy）

---

# 6. 适配器模式（Adapter）

## 6.1 核心思想

适配器模式解决的是：

> **两个接口不兼容，但你又想让它们协同工作。**

它相当于“转接头”。

例如：

- 老接口不能改
- 新系统要求另一种接口
- 中间加一个适配器转换

---

## 6.2 代码示例

```cpp
#include <iostream>
using namespace std;

// 目标接口：客户端想使用的接口
class Target {
public:
    virtual void request() = 0;
    virtual ~Target() = default;
};

// 旧接口：已经存在，但接口不兼容
class Adaptee {
public:
    void specificRequest() {
        cout << "Old specific request\n";
    }
};

// 适配器：把旧接口包装成新接口
class Adapter : public Target {
private:
    Adaptee adaptee;

public:
    void request() override {
        adaptee.specificRequest();
    }
};
```

---

## 6.3 适用场景

- 旧代码不方便改
- 第三方库接口不符合当前系统要求
- 想复用旧类

---

# 7. 桥接模式（Bridge）

## 7.1 核心思想

桥接模式解决的是：

> **把抽象部分和实现部分分离，使它们可以独立变化。**

例如：

- 遥控器（抽象）
- 电视/空调（实现）

遥控器类型和设备类型都可能变化，如果直接继承组合会类爆炸。

桥接模式通过“组合”代替“多维继承”。

---

## 7.2 代码示例

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Device {
public:
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual ~Device() = default;
};

class TV : public Device {
public:
    void turnOn() override { cout << "TV on\n"; }
    void turnOff() override { cout << "TV off\n"; }
};

class Radio : public Device {
public:
    void turnOn() override { cout << "Radio on\n"; }
    void turnOff() override { cout << "Radio off\n"; }
};

class RemoteControl {
protected:
    shared_ptr<Device> device;

public:
    RemoteControl(shared_ptr<Device> d) : device(d) {}
    virtual void on() { device->turnOn(); }
    virtual void off() { device->turnOff(); }
    virtual ~RemoteControl() = default;
};
```

---

# 8. 组合模式（Composite）

## 8.1 核心思想

组合模式解决的是：

> **让“单个对象”和“对象组合”以统一方式使用。**

典型场景：树形结构。

例如：

- 文件和文件夹
- 菜单和子菜单
- 组织架构树
- 表达式树

---

## 8.2 代码示例

```cpp
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Component {
public:
    virtual void operation() = 0;
    virtual ~Component() = default;
};

class Leaf : public Component {
public:
    void operation() override {
        cout << "Leaf operation\n";
    }
};

class Composite : public Component {
private:
    vector<shared_ptr<Component>> children;

public:
    void add(shared_ptr<Component> child) {
        children.push_back(child);
    }

    void operation() override {
        cout << "Composite operation start\n";
        for (auto& child : children) {
            child->operation();
        }
        cout << "Composite operation end\n";
    }
};
```

---

# 9. 装饰器模式（Decorator）

## 9.1 核心思想

装饰器模式解决的是：

> **在不修改原类的情况下，动态地给对象增加功能。**

它比继承更灵活。

例如：

- 咖啡 + 牛奶 + 糖
- 输入流加缓冲
- 图形对象加边框、阴影

---

## 9.2 代码示例

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Coffee {
public:
    virtual double cost() const = 0;
    virtual ~Coffee() = default;
};

class BasicCoffee : public Coffee {
public:
    double cost() const override {
        return 10.0;
    }
};

class CoffeeDecorator : public Coffee {
protected:
    shared_ptr<Coffee> coffee;

public:
    CoffeeDecorator(shared_ptr<Coffee> c) : coffee(c) {}
};

class MilkDecorator : public CoffeeDecorator {
public:
    MilkDecorator(shared_ptr<Coffee> c) : CoffeeDecorator(c) {}

    double cost() const override {
        return coffee->cost() + 2.0;
    }
};

class SugarDecorator : public CoffeeDecorator {
public:
    SugarDecorator(shared_ptr<Coffee> c) : CoffeeDecorator(c) {}

    double cost() const override {
        return coffee->cost() + 1.0;
    }
};
```

---

# 10. 外观模式（Facade）

## 10.1 核心思想

外观模式解决的是：

> **为复杂子系统提供一个更简单的统一接口。**

例如：

- 一键启动电脑
- 一键看电影（打开投影、音响、播放器）
- SDK 对底层复杂模块做统一封装

---

## 10.2 代码示例

```cpp
#include <iostream>
using namespace std;

class CPU {
public:
    void start() { cout << "CPU start\n"; }
};

class Memory {
public:
    void load() { cout << "Memory load\n"; }
};

class Disk {
public:
    void read() { cout << "Disk read\n"; }
};

class ComputerFacade {
private:
    CPU cpu;
    Memory memory;
    Disk disk;

public:
    void startComputer() {
        cpu.start();
        memory.load();
        disk.read();
        cout << "Computer started\n";
    }
};
```

---

# 11. 享元模式（Flyweight）

## 11.1 核心思想

享元模式解决的是：

> **当系统中有大量相似小对象时，通过共享内部状态来节省内存。**

例如：

- 文本编辑器中的字符对象
- 游戏里的树木、子弹类型、粒子模板

---

## 11.2 关键概念

- **内部状态**：可共享，不随上下文变化
- **外部状态**：不能共享，由外部传入

---

# 12. 代理模式（Proxy）

## 12.1 核心思想

代理模式解决的是：

> **通过一个代理对象控制对真实对象的访问。**

代理可以实现：

- 权限控制
- 延迟加载
- 日志记录
- 远程调用
- 缓存

---

## 12.2 代码示例

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Subject {
public:
    virtual void request() = 0;
    virtual ~Subject() = default;
};

class RealSubject : public Subject {
public:
    void request() override {
        cout << "Real subject request\n";
    }
};

class Proxy : public Subject {
private:
    unique_ptr<RealSubject> realSubject;

public:
    void request() override {
        if (!realSubject) {
            // 懒加载：第一次用时才创建真实对象
            realSubject = make_unique<RealSubject>();
        }

        cout << "Proxy before request\n";
        realSubject->request();
        cout << "Proxy after request\n";
    }
};
```

---

# 四、行为型模式（Behavioral Patterns）

行为型模式关注：**对象之间如何分工协作**。

包括：

1. 责任链模式（Chain of Responsibility）
2. 命令模式（Command）
3. 解释器模式（Interpreter）
4. 迭代器模式（Iterator）
5. 中介者模式（Mediator）
6. 备忘录模式（Memento）
7. 观察者模式（Observer）
8. 状态模式（State）
9. 策略模式（Strategy）
10. 模板方法模式（Template Method）
11. 访问者模式（Visitor）

---

# 13. 责任链模式（Chain of Responsibility）

## 核心思想

把多个处理者串成一条链，请求沿着链传递，直到有一个对象处理它。

例如：

- 请假审批
- 日志级别处理
- HTTP 中间件

---

# 14. 命令模式（Command）

## 核心思想

把“请求”封装成一个对象，从而实现：

- 参数化请求
- 排队执行
- 撤销/重做

例如：

- 遥控器按钮
- 编辑器撤销功能

---

## 简单示例

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Receiver {
public:
    void action() {
        cout << "Receiver action\n";
    }
};

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

class ConcreteCommand : public Command {
private:
    Receiver* receiver;

public:
    ConcreteCommand(Receiver* r) : receiver(r) {}

    void execute() override {
        receiver->action();
    }
};
```

---

# 15. 解释器模式（Interpreter）

## 核心思想

给某种语言定义语法规则，并提供解释执行的方式。

常见于：

- 简单表达式解析
- 规则引擎
- DSL（领域专用语言）

这个模式在实际业务中不如其他模式常见，但在编译器、表达式引擎中常出现。

---

# 16. 迭代器模式（Iterator）

## 核心思想

提供一种方法顺序访问聚合对象中的元素，而不暴露其内部表示。

C++ STL 中你已经大量使用它了：

- `vector<int>::iterator`
- `begin()` / `end()`

所以这个模式在现代 C++ 里其实非常常见，只是常被隐藏在标准库容器中。

---

# 17. 中介者模式（Mediator）

## 核心思想

中介者模式解决的是：

> **多个对象之间不直接互相引用，而是通过一个中介者协调通信。**

这样能减少“网状耦合”。

例如：

- 聊天室服务器协调用户消息
- GUI 控件联动

---

# 18. 备忘录模式（Memento）

## 核心思想

在不破坏封装的前提下，保存对象的某个历史状态，以便之后恢复。

例如：

- 编辑器撤销
- 游戏存档

---

# 19. 观察者模式（Observer）

## 19.1 核心思想

观察者模式解决的是：

> **当一个对象状态变化时，自动通知所有依赖它的对象。**

典型场景：

- 发布订阅
- GUI 事件响应
- 消息通知系统

---

## 19.2 代码示例

```cpp
#include <iostream>
#include <vector>
using namespace std;

class Observer {
public:
    virtual void update(int value) = 0;
    virtual ~Observer() = default;
};

class Subject {
private:
    vector<Observer*> observers;
    int state = 0;

public:
    void attach(Observer* obs) {
        observers.push_back(obs);
    }

    void setState(int s) {
        state = s;
        notify();
    }

    void notify() {
        for (auto obs : observers) {
            obs->update(state);
        }
    }
};

class ConcreteObserver : public Observer {
public:
    void update(int value) override {
        cout << "Observer received: " << value << endl;
    }
};
```

---

# 20. 状态模式（State）

## 20.1 核心思想

状态模式解决的是：

> **对象在不同状态下表现出不同行为，把状态相关逻辑拆到不同状态类里。**

例如：

- 自动售货机
- TCP 连接状态
- 播放器（播放/暂停/停止）

这样可以避免超长的 `if-else` / `switch-case`。

---

# 21. 策略模式（Strategy）

## 21.1 核心思想

策略模式解决的是：

> **把一组可互换的算法封装起来，使它们可以在运行时自由替换。**

例如：

- 排序策略
- 支付方式
- 路径规划算法
- 压缩算法

---

## 21.2 代码示例

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Strategy {
public:
    virtual void execute() = 0;
    virtual ~Strategy() = default;
};

class ConcreteStrategyA : public Strategy {
public:
    void execute() override {
        cout << "Using strategy A\n";
    }
};

class ConcreteStrategyB : public Strategy {
public:
    void execute() override {
        cout << "Using strategy B\n";
    }
};

class Context {
private:
    unique_ptr<Strategy> strategy;

public:
    void setStrategy(unique_ptr<Strategy> s) {
        strategy = move(s);
    }

    void doWork() {
        if (strategy) {
            strategy->execute();
        }
    }
};
```

---

# 22. 模板方法模式（Template Method）

## 22.1 核心思想

模板方法模式解决的是：

> **在父类中定义算法骨架，把部分步骤延迟到子类实现。**

也就是：

- 大流程由父类定
- 具体某些步骤由子类定

例如：

- 做饮料：烧水、冲泡、倒杯子
- 编译流程：词法分析、语法分析、优化

---

# 23. 访问者模式（Visitor）

## 23.1 核心思想

访问者模式解决的是：

> **在不修改元素类的前提下，为一组对象结构增加新的操作。**

它适合：

- 对象结构较稳定
- 但操作经常增加

例如：

- 编译器 AST 节点处理
- 报表统计

缺点是：

- 结构复杂
- 不太适合频繁新增元素类型

---

# 五、常见模式之间的区别

## 1. 工厂方法 vs 抽象工厂

### 工厂方法

- 关注“创建一个产品”
- 例如：创建一个按钮

### 抽象工厂

- 关注“创建一组相关产品”
- 例如：一整套 Windows 风格控件

---

## 2. 装饰器 vs 代理

### 装饰器

- 重点在于“增强功能”

### 代理

- 重点在于“控制访问”

---

## 3. 策略模式 vs 状态模式

### 策略模式

- 外部主动选择算法
- 算法之间通常平级可替换

### 状态模式

- 状态随着内部条件自动切换
- 重点是状态驱动行为变化

---

## 4. 适配器 vs 外观

### 适配器

- 解决接口不兼容

### 外观

- 解决子系统太复杂，提供统一入口

---

# 六、现代 C++ 中学习设计模式的建议

## 1. 优先掌握这些高频模式

面试和实战最常见的通常是：

- 单例模式
- 工厂方法模式
- 抽象工厂模式
- 建造者模式
- 适配器模式
- 装饰器模式
- 代理模式
- 观察者模式
- 策略模式
- 状态模式

## 2. 写 C++ 代码时尽量结合现代特性

推荐：

- 用 `unique_ptr` 代替裸指针管理所有权
- 用 `shared_ptr` 表示共享拥有
- 用 RAII 管资源
- 用 `= delete` 禁止拷贝
- 用 `override` 明确重写虚函数
- 用 `default` 简化析构或默认构造

## 3. 不要把模式和“继承”绑定死

很多模式今天也可以用：

- 模板
- lambda
- `std::function`
- 组合
- 泛型编程

实现得更简洁。

---

# 七、面试复习版：一句话记忆

- **单例**：一个类全局只有一个对象
- **工厂方法**：把对象创建交给子类/工厂
- **抽象工厂**：创建一整套相关对象
- **建造者**：复杂对象分步骤构建
- **原型**：通过复制已有对象创建新对象
- **适配器**：把不兼容接口接起来
- **桥接**：抽象和实现分离，独立变化
- **组合**：统一处理单个对象和对象组合
- **装饰器**：不改原类，动态加功能
- **外观**：给复杂系统提供统一简单入口
- **享元**：共享相同内部状态，节省内存
- **代理**：通过代理控制访问真实对象
- **责任链**：请求沿链传递直到被处理
- **命令**：把请求封装成对象
- **解释器**：解释执行某种语言规则
- **迭代器**：顺序访问容器元素
- **中介者**：对象不直接互相联系，由中介协调
- **备忘录**：保存并恢复历史状态
- **观察者**：状态变化时通知订阅者
- **状态**：对象在不同状态下行为不同
- **策略**：封装一组可替换算法
- **模板方法**：父类定流程，子类定细节
- **访问者**：不改元素类，新增操作

---

# 八、最后总结

学习设计模式时，建议按这个顺序来：

1. 先学 **单例、工厂、策略、观察者、状态、适配器、装饰器**  
2. 每学一个模式都问自己：
   - 它解决什么问题？
   - 不用它会怎样？
   - 它的代价是什么？
3. 不要死背 UML 图，重点理解 **设计动机**  
4. 多写小例子，尤其要练习：
   - 怎么调用
   - 为什么这么调用
   - 客户端和具体类是如何解耦的

如果你后面还要继续深入，最值得继续扩展的方向是：

- 每个模式的 UML 类图
- 每个模式的真实工程案例
- 每个模式的 LeetCode / 项目类比
- 设计模式与 SOLID 原则的关系
- 现代 C++ 风格重写版

