---
title: Mac系统中截图
date: 2017-07-21 00:00:00
tags: Mac
---

`Mac`系统下截图大体有两种方法：

* `command + shift + 3` ：全屏幕截图
* `command + shift + 4` ：区域截图

默认情况下，截图会保存到桌面，默认格式为`png`，以上两个操作，如果你同时按住`Ctrl`，截图会保存在剪切板中，可以使用`command + v`来进行粘贴。下面我们介绍如何进行更多个性化的操作与设置。

<!-- more --> 

## 区域截图

当我们按下`command + shift + 4`后，则开始了区域截图，我们可以拖动选择一片区域进行自定义截取，我们也可以进行如下后续辅助操作：

* 按下空格键，鼠标会变成一个小相机，此时可利用鼠标点击指定窗体，便完成了对特定窗体截图。

* 我们在移动选择区域的过程中，不要松开鼠标或触摸板，可以进行如下操作：
    * 按住空格可以对当前区域进行拖动。
    * 按住`shift`将锁定`X`轴或`Y`轴进行拖动。
    * 按住`option`将按照区域圆心进行放大。

## 个性化命令

### 修改生成截图名称

默认截图名称是由前缀名称加上截图时间命名的，我们可通过如下指令，进行个性化前缀：

``` bash
defaults write com.apple.screencapture name CustomPreName
# 重启相关服务，使得设置立即生效
killall SystemUIServer
```

### 修改默认截图存放位置

``` bash
defaults write com.apple.screencapture location ~/Pictures/Screenshots
killall SystemUIServer
```

### 修改默认截图格式

``` bash
defaults write com.apple.screencapture type jpg
killall SystemUIServer
```

### 去掉/恢复阴影

``` bash
defaults write com.apple.screencapture disable-shadow -bool true
defaults write com.apple.screencapture disable-shadow -bool false
killall SystemUIServer
```
