---
title: QSS自定义滚动条
date: 2017-07-07 00:00:00
tags: Qt
---

本文主要讲述如何通过`qss`自定义滚动条`QScrollBar`，我们先以实现一个扁平化的滚动条为例，效果如下图所示。

![](/images/qt/flat-qscrollbar.png)

<!-- more --> 

## 扁平化滚动条实现

```CSS
QScrollBar:vertical {
    width: 8px;
    background: #f0f0f0;
}
QScrollBar:horizontal {
    height: 8px;
    background: #f0f0f0;
}
QScrollBar::handle:vertical {
    background: #cdcdcd;
    min-height: 30px;
}
QScrollBar::handle:horizontal {
    background: #cdcdcd;
    min-width: 30px;
}
QScrollBar::handle:vertical:hover,
QScrollBar::handle:horizontal:hover {
    background: #a6a6a6;
}
QScrollBar::sub-line:vertical, QScrollBar::add-line:vertical,
QScrollBar::sub-line:horizontal, QScrollBar::add-line:horizontal {
    width: 0;
    height: 0;
}
/*make sure the background is solid style.*/
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical,
QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
    background: none;
}
```

在`windows`中，如果不设置`add-page,sub-page:background: none;`，则背景将会变为网格样式。`mac`系统下无此特性。

## 自定义背景图片

通过上面的例子，我们可以用设置背景色的方式，来实现扁平化风格的滚动条，如果我们想有更绚丽的效果，可以通过修改背景图片，来自定义滚动条。我们以纵向为例，参考代码如下：

```CSS
QScrollBar:vertical {
    background: white;
    width: 8px;
}
QScrollBar::handle:vertical {
    min-height: 40px;
    border-width: 0px 4px 0px 4px;
    border-image: url(:/images/scrollbar.png) 0 4 0 4 repeat repeat;
}
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
    width: 8px;
    border-width: 0px 4px 0px 4px;
    border-image: url(:/images/scrollbar_bg.png) 0 4 0 4 repeat repeat;
}
```

## 默认隐藏滚动条

现代风格的软件中，很多情况下滚动条默认是隐藏的，当鼠标进入相关区域后，显示当前区域的滚动条，这样做节省了空间，提高了整体观感。如微信的PC版本就是这种风格。实现方式也很简单，就是在鼠标进入和离开目标区域时，显示和隐藏滚动条即可，参考代码如下：

```C++
ui->listWidget->installEventFilter(this);
ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

bool Widget::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == ui->listWidget)
    {
        if(e->type() == QEvent::Enter)
        {
            obj->setProperty("verticalScrollBarPolicy", "ScrollBarAsNeeded");
        }
        else if(e->type() == QEvent::Leave)
        {
            obj->setProperty("verticalScrollBarPolicy", "ScrollBarAlwaysOff");
        }
    }

    return QWidget::eventFilter(obj, e);
}
```

使用`QObject::setProperty`方法是为了适配多个窗体的情况，代码可复用，只需条件中添加`obj == ui->listWidget2`即可。

## 参考

* [QSS 实现的扁平滚动条](http://qtdebug.com/qt-flat-scrollbar/)