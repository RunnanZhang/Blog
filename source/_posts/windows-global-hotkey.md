---
title: Windows注册全局热键
date: 2016-12-16 21:30:12
tags: [Windows, Qt]
---

说起热键，大家一定不陌生，程序的热键有两种：
* 程序内部：当程序处于`active`状态时，才响应的键盘事件。
* 全局热键：顾名思义，任何地方均可调用的热键。比如 我们最常用的QQ截图`Ctrl+Alt+A`。

下面我们就介绍下，在`Windows`操作系统中，如何使用全局热键。

<!--more-->

## 1、向Windows注册

向操作系统注册快捷键，本着先到先得的原则，先注册的程序即可抢得此快捷键。
```cpp
#include <windows.h>

void Class::initHotKey()
{
    RegisterHotKey((HWND)winId(), 1, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 'M');
    RegisterHotKey((HWND)winId(), 2, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 'N');
}
```

### 函数原型
`BOOL RegisterHotKey(HWND hWnd，int id，UINT fsModifiers，UINT vk);`
* `hWnd` : 窗口句柄
* `id` : 一个唯一标识号，我上面代码随便写的1,2，目前虽并未出错，但正确使用方法如下：

> 定义热键的标识符。调用线程中的其他热键，不能使用同样的标识符。应用程序必须定义一个`0X0000-0xBFFF`范围的值。一个共享的动态链接库（DLL）必须定义一个范围为`0xC000-0xFFFF`的值(`GlobalAddAtom`函数返回该范围）。为了避免与其他动态链接库定义的热键冲突，一个DLL必须使用`GlobalAddAtom`函数获得热键的标识符。

* `fsModifiers` : 若要产生WM_HOTKEY消息，必须同时按下fsModifiers和vk参数定义的键。

| 键            | 值      | 说明                 |
| ------------ | ------ | ------------------ |
| MOD_ALT      | 0x0001 | 按下的可以是任一`Alt`键     |
| MOD_SHIFT    | 0x0004 | 按下的可以是任一`Shift`键   |
| MOD_WIN      | 0x0008 | 按下的可以是任一`Windows`键 |
| MOD_NOREPEAT | 0x4000 | 键盘自动重复不会产生多个热键通知   |
| MOD_CONTROL  | 0x0002 | 按下的可以是任一`Ctrl`键    |

* `vk` : 定义热键的虚拟键码。

## 2、程序中处理`WM_HOTKEY`消息
我们注册了快捷键，当按下时，系统就会通知到我们的指定程序，我们只需要接收，辨别Id来识别我们注册的快捷键，做相应处理即可：
```cpp
bool Class::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(result);
    // 通过消息类型，判断操作系统。
    if(eventType == "windows_generic_MSG")
    {
        MSG* msg = static_cast<MSG*>(message);

        if(msg->message == WM_HOTKEY) {
            switch(msg->wParam) {
            case 1:
            	doSomething1();
            	break;

            case 2:
                doSomething2();
                break;
            }
        }
    }
    return false;
}
```

## 3、程序退出时，注销相关注册。

在程序退出时，我们一定要注销我们所注册的快捷键，否则将无法再次注册，直到重启为止。

```cpp
Class::~Class()
{
    UnregisterHotKey((HWND)winId(), 1);
    UnregisterHotKey((HWND)winId(), 2);
}
```
