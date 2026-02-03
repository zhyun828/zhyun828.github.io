
## 📄 对应页

* **Lecture des données**
* **Rotation du moteur et initialisation de l’angle**
* **Commandes et modes de fonctionnement**

---

## 🇨🇳 中文（你的版本，轻微整理）

### 🔹 数据读取（Lecture des données）

首先，温度由 **MPL3115A2 温度传感器** 通过 **I²C 通信** 读取，
得到稳定的温度值后，再线性转换为目标角度。

其次，电机的旋转角度是无法直接测量的，
因此我们通过 **电机编码器的计数值** 来计算角度。

通过实验我们得到：

* 驱动轮旋转一圈，对应 **12 个编码器计数**
* 指针轴旋转 **360°**，需要 **52.734 圈驱动轮**

因此，我们可以通过这个关系式，
来控制电机旋转到对应的角度。

---

### 🔹 电机旋转与角度初始化

（Rotation du moteur et initialisation de l’angle）

本系统中没有物理意义上的**绝对零位**，
电机和传动机构只能测量角度的变化量。

因此在系统上电后，
我们设计了一套**软件初始化流程**。

用户可以通过串口命令手动调整指针位置：

* `r / l`：正向或反向旋转一圈
* `r5 / l5`：正向或反向旋转五圈

当指针被调整到正确位置后，
使用 **`sync`** 命令，
将当前角度设定为软件参考零位，从而完成标定。

---

### 🔹 命令与工作模式

（Commandes et modes de fonctionnement）

系统主要有两种工作模式。

**STOP 模式** 是默认模式，
此时电机不会自动运动。
通常在该模式下，
使用指令调整电机的初始角度。

当指针指向实际温度后，
输入指令 **`sync、g`**，
系统进入 **GO 模式**。

在 **GO 模式** 下，
系统会持续跟随温度变化。
每当温度发生变化时，
程序会重新计算目标角度，
并且只有在目标发生变化时，
才触发电机运动。

---

## 🇫🇷 法语版本（**简单词汇 + 口语化**）

### 🔹 Lecture des données

> D’abord, la température est mesurée par le capteur **MPL3115A2**
> grâce à une communication **I²C**.
>
> La valeur de température est stable,
> puis elle est convertie de façon linéaire en un angle cible.
>
> Ensuite, l’angle de rotation du moteur
> ne peut pas être mesuré directement.
>
> On utilise donc le **comptage de l’encodeur**
> pour calculer l’angle.
>
> Expérimentalement, on a obtenu que :
>
> * un tour du volant d’entraînement correspond à **12 counts**,
> * et **52,734 tours** du volant correspondent à **360 degrés**
>   de rotation de l’aiguille.
>
> Grâce à cette relation,
> on peut commander la rotation du moteur
> pour atteindre l’angle souhaité.

---

### 🔹 Rotation du moteur et initialisation de l’angle

> Le moteur n’a **pas de position zéro physique**.
>
> Le moteur et le mécanisme permettent seulement de mesurer la variation de l’angle.
>
> C’est pourquoi, après la mise sous tension, on a mis en place une **initialisation logicielle**.
>
> L’utilisateur peut ajuster manuellement la position de l’aiguille avec des commandes série :
>
> * `r / l` pour une rotation d’un tour,
> * `r5 / l5` pour une rotation de cinq tours.
>
> Une fois la position correcte atteinte,
> la commande **`sync`** permet
> de définir la position actuelle
> comme référence logicielle.

---

### 🔹 Commandes et modes de fonctionnement

> Le système fonctionne avec deux modes principaux.
>
> Le mode **STOP** est le mode par défaut.
> Dans ce mode, le moteur ne bouge pas automatiquement.
>
> En général, on utilise les commandes
> `r / l / r5 / l5`
> pour régler la position initiale de l’aiguille.
>
> Quand l’aiguille indique la température réelle,
> on entre la commande **`sync`** et **`g`**
> pour passer en mode **GO**.
>
> En mode **GO**,
> le système suit en continu les variations de température.
>
> À chaque changement de température,le programme recalcule l’angle cible,
> et le moteur ne bouge que si la consigne change.

---
