---
title: PImpl机制以及Qt的D-Pointer实现
date: 2017-03-13 00:00:00
tags: [C++, Qt]
---

`PImpl`是 `Pointer to implementation`的缩写， 是C++ 在构建导出库接口时特有的技术手段。 即是将类`Class`中所有私有变量以及私有方法，封装在一单独的实现类`ClassImpl`中。我们在`Class`中通过一指向`ClassImpl`的私有指针，访问这些私有数据。而`ClassImpl`类的具体定义和实现，我们放入`cpp`中。Qt中的`D-Pointer`技术，便是`PImpl`机制的一种实现方式。

优点：
* 使得程序接口有着稳定的`ABI`（应用程序二进制接口），即不会打破二进制兼容。
* 降低了程序编译依赖，从而缩短编译时间。
* 数据隐藏，使得头文件很干净，不包含实现细节，可以直接作为 API 参考。

缺点：
* 实现者需要做更多工作。
* 由于子类需要访问，此机制对`protected`方法不奏效。
* 由于数据的隐藏，多少造成了代码可读性下降。
* 运行时性能被轻微的连累，尤其调用的函数为虚函数时。

<!-- more --> 

关于二进制兼容的问题，在[C++的二进制兼容](http://zhangrunnan.com/cpp-binary-compatibility/)一文中有详细解释，这里不再赘述。下面我们通过介绍Qt的`D-Pointer`的实现细节，来理解PImpl机制。

## 参考

* [Pimpl idiom](http://wiki.c2.com/?PimplIdiom)
* [D-Pointer](https://wiki.qt.io/D-Pointer/zh)
