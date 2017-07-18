---
title: Mac下加密文件
date: 2017-07-18 00:00:00
tags: Mac
---

由于`Mac`系统下`Spotlight`工具的强大，我们如若想存储一些隐私文件，不被人所知，有如下两种方法：
* 单纯的隐藏文件，在`Finder`中看不到，但是`Spotlight`依然可以搜索到。
* 我们可以制作一个加密的映像文件——`dmg`格式。`dmg`格式相当于`windows`中的`iso`。可以随意拷贝，使用时类似加载光盘的效果。

<!-- more --> 

## Mac下隐藏文件

* 隐藏文件：`chflags hidden ~/Library`
* 显示文件 ：`chflags nohidden ~/Library` 


## 制作dmg映像文件

* 利用磁盘管理工具新建一磁盘映像。
  ![](/images/mac/new-dmg.png)

  ​

* 选择待加密的文件夹，随后选择加密方法以及映像格式，映像格式选择读写即可，选择压缩则会压缩文件，读写即是在原来基础上加密，并且可以随时扩展。
  ![](/images/mac/new-dmg-info.png)

  ​

* 点击存储后，开始制作映像文件，制作完毕后我们会得到一个`dmg`文件。使用时双击此文件，系统则会自动加载，输入密码后可访问加密内容。使用完毕后，去桌面推出此`dmg`光盘即可。
  ![](/images/mac/new-dmg-complete.png)

  ​
