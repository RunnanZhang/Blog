---
title: QDesigner中验证QSS格式是否有效的方法
date: 2016-12-12 00:00:00
tags: Qt
---

QDesigner中编辑样式表，如下图所示，可以及时验证所写QSS格式是否正确。这个功能是`QtGui`的内部功能，并未放开。我们可直接包含其头文件，使用其方法。

![](/images/qt/editStyle.png)

<!-- more --> 

## 使用QCss::Parser

使用QCss类相关代码：
* 方式：可包含`#include <QtGui/5.7.0/QtGui/private/qcssparser_p.h>` 
* 此法为`QtGui`中方法，正常链接`QT += gui`即可使用.

### 节选自QDesigner源码中函数

> 源码位置:` C:\Qt\Qt5.7.0\5.7\Src\qttools\src\designer\src\lib\shared\stylesheeteditor.cpp`

```C++
bool StyleSheetEditorDialog::isStyleSheetValid(const QString &styleSheet) {
	QCss::Parser parser(styleSheet);
  	QCss::StyleSheet sheet;
  	if (parser.parse(&sheet))
  	return true;
  	QString fullSheet = QStringLiteral("* { ");
  	fullSheet += styleSheet;
  	fullSheet += QLatin1Char('}');
  	QCss::Parser parser2(fullSheet);
  	return parser2.parse(&sheet);
}
```
