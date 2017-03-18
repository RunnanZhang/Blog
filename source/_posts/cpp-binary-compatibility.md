---
title: C++的二进制兼容
date: 2017-03-19 00:00:00
tags: C++
---
在深入了解前，首先我们要明确两个概念：二进制兼容和源码兼容。

* 二进制兼容：在升级库文件的时候，不必**重新编译**使用此库的可执行文件或其他库文件，并且程序的功能不被破坏。

* 源码兼容：在升级库文件的时候，不必**修改**使用此库的可执行文件或其他库文件的**源代码**，只需重新编译应用程序，即可使程序的功能不被破坏。

<!-- more --> 

## ABI和API
> 应用二进制接口（application binary interface，缩写为 ABI）描述了应用程序（或者其他类型）和操作系统之间或其他应用程序的低级接口。`ABI`涵盖了各种细节，如：数据类型的大小、布局和对齐；调用约定等。

在了解二进制兼容和源码兼容两个定义以后，我们再看与其类似且对应的两个概念：`ABI`和`API`。`ABI`不同于`API`（应用程序接口），`API`定义了源代码和库之间的接口，因此同样的代码可以在支持这个`API`的任何系统中编译，然而`ABI`允许编译好的目标代码在使用兼容`ABI`的系统中无需改动就能运行。

举个例子，在Qt和Java两种跨平台程序中，`API`像是Qt的接口，Qt有着通用接口，源代码只需要在支持Qt的环境下编译即可。`ABI`更像是Jvm，只要支持Jvm的系统上，都可以运行已有的Java程序。

## C++的ABI
`ABI`更像是一个产品的使用说明书，同理C++的`ABI`就是如何使用C++生成可执行程序的一张说明书。编译器会根据这个说明书，生成二进制代码。C++的`ABI`在不同的编译器下会略有不同。

C++`ABI`的部分内容举例：
* 函数参数传递的方式，比如 x86-64 用寄存器来传函数的前 4 个整数参数 
* 虚函数的调用方式，通常是 vptr/vtbl 然后用 vtbl[offset] 来调用 
* struct 和 class 的内存布局，通过偏移量来访问数据成员 

综上所述，**如果可执行程序通过以上说明书访问动态链接库A，以及此库的升级版本A+，若按此说明书上的方法，可以无痛的使用A和A+，那么我们就称库A的这次升级是二进制兼容的。**

## 破坏二进制兼容的几种常见方式

*　不导出或者移除一个导出类
*　改变类的继承
*　添加新的虚函数
*　改变虚函数声明时的顺序（偏移量改变，导致调用失败）
*　添加新的非静态成员变量（类的内存布局改变，偏移量也发生变化）
*　改变非静态成员变量的声明顺序

## 不会破坏二进制兼容的几种常见方式

* 添加非虚函数（包括构造函数）
* 添加新的类
* 添加Qt中的信号槽
* 在已存在的枚举类型中添加一个枚举值
* 添加新的静态成员变量
* 修改成员变量名称（偏移量未改变）
* 添加`Q_OBJECT`,`Q_PROPERTY`, `Q_ENUMS` ,`Q_FLAGS`宏，添加这些宏都是修改了moc生成的文件，而不是类本身

只要我们知道了程序是以什么方式访问动态库的（C++的ABI)，那么我们就很好判断，哪些操作会破坏二进制兼容。更多方式请参见[Policies/Binary Compatibility Issues With C++ ](https://community.kde.org/Policies/Binary_Compatibility_Issues_With_C%2B%2B)  


## 解决二进制兼容问题的相关方法

* 使用`Bitflags`即位域

```cpp
uint m1 : 1;
uint m2 : 3;
uint m3 : 1;
```
```cpp
uint m1 : 1;
uint m2 : 3;
uint m3 : 1;
uint m4 : 2; // new member without breaking binary compatibility.
```
* 使用`PImpl`机制，详情参见[PImpl机制以及Qt的D-Pointer实现](http://zhangrunnan.com/cpp-pimpl/)
* 使用静态库（当然也随之带来一系列弊端）

## C++抽象类和Java的接口
读到这里大家会奇怪，作者是不是放错地方了？其实并不是，只是在我们了解二进制兼容后，可以更好地理解这组概念。之前我一直认为C++的抽象类就类似于Java的接口，现在发现，如果把一个C++的抽象类作为动态库的接口发布，那将是毁灭的。因为你无法增加虚函数，无法增加成员变量，这使得这个接口变得非常的不友好。这也就是Java接口的优势所在。Java 实际上把 C/C++ 的 linking 这一步骤推迟到 class loading 的时候来做，便不存在上述二进制兼容的问题。

> 理解Java二进制兼容的关键是要理解延迟绑定（Late Binding）。延迟绑定是指Java直到运行时才检查类、域、方法的名称，而不象C/C++的编译器那样在编译期间就清除了类、域、方法的名称，代之以偏移量数值——这是Java二进制兼容得以发挥作用的关键。
> 由于采用了延迟绑定技术， 方法、域、类的名称直到运行时才解析，意味着只要域、方法等的名称（以及类型）一样，类的主体可以任意替换

## 参考

* [Policies/Binary Compatibility Issues With C++ ](https://community.kde.org/Policies/Binary_Compatibility_Issues_With_C%2B%2B)
* [C++ 工程实践(4)：二进制兼容性](http://www.cppblog.com/Solstice/archive/2011/03/09/141401.html)