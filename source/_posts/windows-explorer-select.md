---
title: Windows打开资源管理器并选中文件
date: 2017-03-24 00:30:00
tags: Windows
---

在`windows`中，我们若想打开资源管理器，并选中文件，可以利用`explorer.exe`的`select`命令。

```cpp
#include <windows.h>
#include <shellapi.h>

ShellExecute(NULL,(LPCWSTR)L"open", (LPCWSTR)L"explorer.exe", (LPCTSTR)L" /select, E:\\a.txt", NULL, true);
```

<!--more-->

注意：

* 我们程序中自己写的代码路径，要加转义字符，**转义字符是给编译器看的**，实际存入字符串中的并非是`\\`，而是一个`\`。当然我们从变量中读取的路径则可直接使用。
* 在`cmd`命令中有些路径必须使用反斜杠`\`，虽然`windows`中路径反斜杠和斜杠均可，但`cmd`中，我们传递参数均用斜杠`/`，所以路径中假如使用`/`则会出错，产生歧义。