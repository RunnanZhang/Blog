---
title: 修改QMenu中的Icon大小
date: 2018-08-30 00:00:00
tags: Qt
---

提到`QMenu`中如何定制`Icon`的大小，第一个想到的是使用`qss`来修改样式，便尝试了如下语句：

```CSS
QMenu::icon
{ 
    width: 24px;
    height: 24px;
}
```

但是经尝试，这个办法并未奏效，`QMenu`中图标大小，默认为16\*16，是从`QStyle`中读取而来，我们可以通过自定义一个`QStyle`来实现修改图标大小，下面给出具体实现方式。

<!-- more --> 

```C++
class CustomStyle : public QProxyStyle
{
    Q_OBJECT
    
public:
    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
    {
        if (metric == QStyle::PM_SmallIconSize) {
            return 24;
        }  
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
};
```

然后将自定义风格设置到`QMenu`上即可：

```C++
    QMenu *menu = new QMenu(this);  
    CustomStyle *style = new CustomStyle();  
    menu->setStyle(style);  
```