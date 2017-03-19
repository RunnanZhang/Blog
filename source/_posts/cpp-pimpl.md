---
title: PImpl机制以及Qt的D-Pointer实现
date: 2017-03-19 10:00:00
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

## Qt中D-Pointer的实现

* Psrson.h

```cpp
class PersonPrivate;
class Person
{
    Q_DECLARE_PRIVATE(Person)

public:
    QString name();

signals:
    void calcRequested();

private:
    QScopedPointer<QObjectData> d_ptr;
};
```

* Person_p.h

```cpp
#include "Person.h"

class PersonPrivate
{
    Q_DECLARE_PUBLIC(PersonPrivate)

public:
    PersonPrivate(Person *parent);
    
    void calc();
    
    QString name;

private:
    Person * const q_ptr;
};
```

* Person.cpp

```cpp
#include "Person_p.h"

Person::Person() : d_ptr(new PersonPrivate(this))
{

}

Person::name()
{
    Q_D(Person);

    return d->name;
}

PersonPrivate::PersonPrivate(Person *parent) : q_ptr(parent)
{
  
}
void PersonPrivate::calc()
{
    Q_Q(Person);

    emit q->calcRequested();
}
```

## 相关宏定义及作用

以上所有用到的宏定义，均放在`qglobal.h`中。下面我们一一介绍。


### Q_DECLARE_PRIVATE、Q_D

```cpp
template <typename T> static inline T *qGetPtrHelper(T *ptr) { return ptr; }
template <typename Wrapper> static inline typename Wrapper::pointer qGetPtrHelper(const Wrapper &p) { return p.data(); }

#define Q_DECLARE_PRIVATE(Class) \
    inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(qGetPtrHelper(d_ptr)); } \
    inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(qGetPtrHelper(d_ptr)); } \
    friend class Class##Private;
    
#define Q_D(Class) Class##Private * const d = d_func()
```

其实`Q_DECLARE_PRIVATE`看似复杂，其实就是一层封装，目的就是让我们可以方便地拿到并使用私有类指针`d_ptr` 。利用`d_func()`函数，可以避免我们每次直接拿到`d_ptr`然后自己进行类型转换（因为我们有可能会在子类中使用此方法，具体我们将在后面的拓展中详述）。

那在`d_func()`中，我们为什么不直接使用`d_ptr` ，而要借助`qGetPtrHelper()`函数呢？其实利用这个函数，就是为了**适配我们使用智能指针的情况**，因为此时我们要拿到真正的指针，需要调用`d_ptr.data()`。

`Q_D`宏进行再次封装，让我们可以方便的拿到d指针，此指针即为我们想要的`d_ptr`。此宏的封装也是为了我们使用const的方便，我们可以通过`Q_D(const Person)`拿到const指针，从而进行只读操作，进而控制程序的正确性。


### Q_DECLARE_PUBLIC、Q_Q

```cpp
#define Q_DECLARE_PUBLIC(Class)                                    \
    inline Class* q_func() { return static_cast<Class *>(q_ptr); } \
    inline const Class* q_func() const { return static_cast<const Class *>(q_ptr); } \
    friend class Class;
    
#define Q_Q(Class) Class * const q = q_func()
```

同理，我们在私有类中，有时候需要调用主类的方法，这两个宏的作用就是为了可以在私有类中拿到主类的指针。我们在私有类的构造函数中传入主类指针，并赋值给`q_ptr`。因为这里是拿到主类的指针，并不存在智能指针的问题，所以此处并没有借助`qGetPtrHelper()`函数。

## 拓展

有了上面的讲解，我们到这里可以思考一个问题，假如我们的类有很多的子类，那么我们岂不是每一个子类都需要定义一个`d_ptr`。每一个`private`类都需要有一个`q_ptr`的指针么？Qt中当然不会如此实现，所以就有了下面的优化版本。

* 首先我们在基类`QObject`中将`d_ptr`变为`protected`类型，并在基类中添加一`protected`类型的构造函数，供子类使用。
```cpp
class QObject
{
protected：
    QObject(QObjectPrivate &dd, QObject *parent = 0); 
    QScopedPointer<QObjectData> d_ptr;
    ...
};
```

* 所有的私有类均继承于`QObjectPrivate`，
```cpp
class QWidgetPrivate : public QObjectPrivate
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QWidget)
    ...
};
```


下面我们在看看`QWidget`和`QObject`的构造函数：
```cpp
QWidget::QWidget(QWidget *parent, Qt::WindowFlags f)     
        : QObject(*new QWidgetPrivate, 0), QPaintDevice()  
{ 
    ... 
}

QObject::QObject(QObject *parent)
    : d_ptr(new QObjectPrivate)
{
    Q_D(QObject);
    d_ptr->q_ptr = this;
};

QObject::QObject(QObjectPrivate &dd, QObject *parent)
    : d_ptr(&dd)
{
    Q_D(QObject);
    d_ptr->q_ptr = this;
};
```

到这里，总算真相大白，`QWidget`中并没有出现`d_ptr`指针，原来是从`Qbject`继承而来。`QObject`中我们新添加的那个`protected`构造函数传入一个`QWidgetPrivate`，用此给`QObject`中的`d_ptr`赋值，而这便是我们唯一的`d_ptr`。现在总算真正理解之前`d_func()`中那些类型转换的作用，就是**保证我们可以拿到当前正确类型的private指针**。

那么同理，`ObjectPrivate`是继承于`QObjectData`，而在`QObjectData`中有着`QObject *q_ptr;`。 所有`QObject`子类的私有类，均继承于`ObjectPrivate`，故而子类中也不会出现`q_ptr`，在`QObject`的构造函数中，我们把`this`指针给其赋值，在通过使用`Q_Q`宏，我们**同样可以拿到正确类型的主类`q`指针**。

## 总结

我们完全可以不借助`Qt`这些宏来实现`PImpl`，其实只需要构建`private`类，将其放入`cpp`中，就已经实现了`PImpl`。不过利用这些宏，可以简单的实现出`Qt`风格的数据隐藏，我们可以利用上面`Person`类的简化版实现，当然假如我们的类需要被继承，我们也可以参考拓展中的方式，利用继承`ObjectPrivate`类的方式实现，不过需要注意，想要使用此类，我们需要在`pro`中添加`QT += core-private`。

## 参考

* [Pimpl idiom](http://wiki.c2.com/?PimplIdiom)
* [D-Pointer](https://wiki.qt.io/D-Pointer/zh)
