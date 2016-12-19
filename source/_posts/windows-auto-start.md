---
title: Windows设置程序开机启动
date: 2016-12-19 00:00:00
tags: [Qt, Windows]
---

本篇文章为大家介绍，在Windows系统下，如何利用注册表设置开机启动程序。

<!-- more --> 

## 启动项在注册表中位置

用户启动项均存储在注册表中如下位置：
`"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"`

![](/images/windows/auto-start.png)

如图可知，我们只需在Run中添加一项，键值即是程序绝对路径以及启动参数，当系统启动时，会将Run中所有项对应程序启动。

## 利用QSetting写入注册表

```Cpp
#define REGEDIT_AUTO_START_PATH "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define REGEDIT_KEY "MagicAssistant"

void MagicAssistant::autoStart(bool is_start)
{
    // QSettings::NativeFormat可用于读取注册表.
    QSettings regedit(REGEDIT_AUTO_START_PATH, QSettings::NativeFormat);

    if(is_start) {
        QString sAppPath = QApplication::applicationFilePath();
        regedit.setValue(REGEDIT_KEY, QVariant(QDir::toNativeSeparators(sAppPath)));
    } else {
        regedit.setValue(REGEDIT_KEY, QVariant());
    }
}
```
