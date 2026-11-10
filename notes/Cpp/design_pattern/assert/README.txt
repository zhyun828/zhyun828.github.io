# UML 资源维护

主要参考：用户提供的 `00designpatternscard.pdf`，Jason S. McDonald，2007，两页 GoF 图解卡。

**阅读顺序约定**：PDF 是两栏排版，本文按“每栏从上到下，先左栏再右栏”阅读图解，跳过第一页左上角的字母索引。第一页先读左栏 Chain of Responsibility 至 Mediator，再读右栏 Memento 至 Visitor。第二页先读 Structural 左栏 Adapter 至 Flyweight，再接右上角 Proxy；随后读右栏 Creational 的 Abstract Factory 至 Singleton。这里没有采用 PDF 左上角索引的字母排序。

两份文档共享下表的 23 项顺序与英文锚点。模式名、分类和基础角色来自 PDF；案例、C++17 实现、工程边界、对比及原稿纠错属于额外补充。图形重绘保留 PDF 的角色结构；对原图不准确的签名和关系作明确修正，而不照搬错误。

|序号|分类|标准英文名称 / 中文名称|
|---:|---|---|
|01|Behavioral|[Chain of Responsibility / 责任链模式](../Design-Patterns.md#chain-of-responsibility)|
|02|Behavioral|[Command / 命令模式](../Design-Patterns.md#command)|
|03|Behavioral|[Interpreter / 解释器模式](../Design-Patterns.md#interpreter)|
|04|Behavioral|[Iterator / 迭代器模式](../Design-Patterns.md#iterator)|
|05|Behavioral|[Mediator / 中介者模式](../Design-Patterns.md#mediator)|
|06|Behavioral|[Memento / 备忘录模式](../Design-Patterns.md#memento)|
|07|Behavioral|[Observer / 观察者模式](../Design-Patterns.md#observer)|
|08|Behavioral|[State / 状态模式](../Design-Patterns.md#state)|
|09|Behavioral|[Strategy / 策略模式](../Design-Patterns.md#strategy)|
|10|Behavioral|[Template Method / 模板方法模式](../Design-Patterns.md#template-method)|
|11|Behavioral|[Visitor / 访问者模式](../Design-Patterns.md#visitor)|
|12|Structural|[Adapter / 适配器模式](../Design-Patterns.md#adapter)|
|13|Structural|[Bridge / 桥接模式](../Design-Patterns.md#bridge)|
|14|Structural|[Composite / 组合模式](../Design-Patterns.md#composite)|
|15|Structural|[Decorator / 装饰器模式](../Design-Patterns.md#decorator)|
|16|Structural|[Facade / 外观模式](../Design-Patterns.md#facade)|
|17|Structural|[Flyweight / 享元模式](../Design-Patterns.md#flyweight)|
|18|Structural|[Proxy / 代理模式](../Design-Patterns.md#proxy)|
|19|Creational|[Abstract Factory / 抽象工厂模式](../Design-Patterns.md#abstract-factory)|
|20|Creational|[Builder / 建造者模式](../Design-Patterns.md#builder)|
|21|Creational|[Factory Method / 工厂方法模式](../Design-Patterns.md#factory-method)|
|22|Creational|[Prototype / 原型模式](../Design-Patterns.md#prototype)|
|23|Creational|[Singleton / 单例模式](../Design-Patterns.md#singleton)|

每种模式一份 kebab-case.puml 与对应 SVG。源文件使用 PlantUML 1.2025.2 验证和渲染；不依赖在线图形服务。统一字体 Arial，行为型浅绿、结构型浅橙、创建型浅蓝。关系的领域含义与代码所有权差异以教材说明为准。

重新渲染命令（在本目录运行，替换实际 jar 路径）：

```text
java -jar /path/to/plantuml.jar -charset UTF-8 -tsvg *.puml
```
