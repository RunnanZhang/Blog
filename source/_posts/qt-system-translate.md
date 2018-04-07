---
title: Qt中系统相关翻译
date: 2018-04-07 00:00:00
tags: Qt
---

Qt中系统控件如何进行翻译呢，Qt的发布包中其实是有相关的翻译文件，源文件`.ts`以及二进制版本`.qm`都是有提供的。我们可以在发布路径下找到qm文件，以`Qt5.7`为例，即`C:/Qt/5.7/msvc2015_64/translations/qt_zh_CN.qm`

但是这个文件时有缺失的，所以我们为了翻译的完全，以及更好地体验，需要自行修改`ts`文件，在原有的`ts`文件末尾追加一些翻译，然后利用`lrelease`自行生成`.qm`文件。官方的`.ts`文件一般位于源码路径下，例如`C:/Qt/5.9.1/Src/qttranslations/translations/qt_zh_CN.ts`。下面列举三个Qt并未进行翻译的地方，我们只需将下面给出的`xml`源码，复制到`ts`文件中相应位置即可。

<!-- more --> 

## 1、关于`QColorDialog`中屏幕选择操作

``` xml
   <message>
        <source>&amp;Pick Screen Color</source>
        <translation>获取屏幕颜色</translation>
    </message>
    <message>
        <source>Cursor at %1, %2
Press ESC to cancel</source>
        <translation>坐标: %1, %2
按ESC键取消</translation>
    </message>
```
注意因为源码中有`\n`换行，所以格式必须按上述来写，换行不可省略。将此段代码，嵌入到原`ts`文件的`QColorDialog`中的`context`段即可。


## 2、关于一些右键菜单（如树节点的右键菜单）

将如下代码直接复制到`ts`文件尾部即可。

``` xml
<context>
    <name>QWidgetTextControl</name>
    <message>
        <source>&amp;Undo</source>
        <translation>撤销(&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>恢复(&amp;R)</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>剪切(&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>复制(&amp;C)</translation>
    </message>
    <message>
        <source>Copy &amp;Link Location</source>
        <translation>复制链接位置(&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>粘贴(&amp;P)</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>删除</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>选择全部</translation>
    </message>
</context>
```



## 3、`QMessageBox`中的按钮

一个很困惑的事情，`QMessageBox`中的按钮的翻译并不在`QDialogButton`中，费了一番周折，查找源码发现，在`QPlatformTheme`这个类中进行的翻译。将如下代码直接复制到`ts`文件尾部即可。

``` xml
<context>
    <name>QPlatformTheme</name>
    <message>
        <source>OK</source>
        <translation>确定</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>保存</translation>
    </message>
    <message>
        <source>Save All</source>
        <translation>保存全部</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打开</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;是</translation>
    </message>
    <message>
        <source>Yes to &amp;All</source>
        <translation>全部&amp;是</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;否</translation>
    </message>
    <message>
        <source>N&amp;o to All</source>
        <translation>&amp;全部否</translation>
    </message>
    <message>
        <source>Abort</source>
        <translation>终止</translation>
    </message>
    <message>
        <source>Retry</source>
        <translation>重试</translation>
    </message>
    <message>
        <source>Ignore</source>
        <translation>忽略</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Discard</source>
        <translation>放弃</translation>
    </message>
    <message>
        <source>Help</source>
        <translation>帮助</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>应用</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>重置</translation>
    </message>
    <message>
        <source>Restore Defaults</source>
        <translation>恢复默认</translation>
    </message>
</context>
```
