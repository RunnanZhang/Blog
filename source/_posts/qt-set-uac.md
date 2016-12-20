---
title: Qt中设置程序UAC启动权限
date: 2016-12-20 00:00:00
tags: [Qt,  Windows]
---

`UAC(User Account Control)` 在Windows中，当某程序需要以管理员身份运行时（程序图标上有一个小盾牌），系统会弹出UAC提示。本篇我们谈一下，如何利用`qmake`，设置UAC。

<!-- more --> 

## VS中设置UAC

![](/images/qt/set-uac.png)

## Qt中设置UAC

在Qt中，我们可以通过变量`QMAKE_LFLAGS`，来为Linker中增加Flags。这样，我们`qmake`后，VS中相对项也会对应修改。下面为`QMAKE_LFLAGS`文档中解释：

> Specifies a general set of flags that are passed to the linker. If you need to change the flags used for a particular platform or type of project, use one of the specialized variables for that purpose instead of this variable.

### 利用QMAKE_LFLAGS设置UAC权限等级：

经测试，下面两种情况无通用做法，第一种必须加转义符号，而第二种不可以加。

* 直接利用Qt Creator编译：
  `QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"`

* 利用qmake转vcproj的情况：
  `QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator'uiAccess='false'"`
