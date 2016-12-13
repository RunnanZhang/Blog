---
title: 字符集与编码
date: 2016-12-12 00:00:00
tags: encoding
---
首先要为大家澄清一个概念，Unicode是一个**编码字符集**，并不是一种**编码**，Unicode制定了可以容纳世界上所有文字和符号的一个字符的集合，并将这个集合映射到一个整数集合，此整数集合即**码位**。由于微软习惯把`UTF-16LE(Little Endian)`称做Unicode编码，所以误导了大部分人。`UTF-8` `UTF-16(LE/BE)` `UTF-32(LE/BE)`才是针对Unicode字符集的编码方式。

**编解码即是：将程序数据（字节序列）和字符集的码位进行互转的方法。**

<!-- more --> 

> 编码也算是历史悠久，最开始美国佬认为ASCII即可表示世界上所有字符，后来进行不断拓展，经历了ISO和Unicode从两败俱伤走向强强联手阶段，最终形成了现在的Unicode。目前的Unicode字符分为17组编排，0x0000 至 0x10FFFF，每组称为平面（Plane），而每平面拥有65536个码位，共1114112个。

## 几种常见编码
* ASCII（American Standard Code for Information Interchange，美国信息交换标准代码） ：标准形式为7bit，127个字符，`0xxxxxxx`格式。
* Latin1 ：同样为单字节编码，**向下兼容ASCII**，其编码范围是`0x00-0xFF`。`0x00-0x7F`之间完全和ASCII一致，`0x80-0x9F`之间是控制字符，`0xA0-0xFF`之间是文字符号。
* ANSI ： 为一个标准，不代表具体某种特定编码。在简体中文Windows操作系统中，ANSI 编码代表 GBK 编码；在繁体中文Windows操作系统中，ANSI编码代表Big5；不同 ANSI 编码之间互不兼容，当信息在国际间交流时，**无法将属于两种语言的文字，存储在同一段 ANSI 编码的文本中**。ANSI编码表示英文字符时用一个字节，表示中文用两个或四个字节。
* `UTF-8( 8-bit Unicode Transformation Format)` `UTF-16(LE/BE)` `UTF-32(LE/BE)`

## 几种UTF编码
下面讲解下几种UTF编码分别是如何工作的。
### UTF-8
| Unicode码位(十六进制)     | UTF-8 字节流(二进制)                           |
| ------------------- | ---------------------------------------- |
| 00000000 - 0000007F | 0xxxxxxx                                 |
| 00000080 - 000007FF | 110xxxxx 10xxxxxx                        |
| 00000800 - 0000FFFF | 1110xxxx 10xxxxxx 10xxxxxx               |
| 00010000 - 001FFFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx      |
| 00200000 - 03FFFFFF | 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx |
| 04000000 - 7FFFFFFF | 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx |

按照上述模板，可以将`Unicode`所有码位，换算成`UTF-8`模式的字节序列（将码位的二进制形式依次填充模板中的`x`）。并且**`UTF-8`完全向下兼容`ASCII` 和`Latin-1`。**



`UTF-8`的特点是对**不同码位范围**的字符使用**不同长度**的编码。对于`0x00-0x7F`之间的字符，`UTF-8`编码与`ASCII`编码完全相同。`UTF-8`编码的最大长度是6个字节。从上表可以看出，6字节模板有31个x，即可以容纳31位二进制数字。Unicode的最大码位`0x7FFFFFFF`也只有31位。

### UTF-16

`UTF-16`是用2字节表示一个`Unicode`的码位，所以**和ASCII并不兼容**，因为就算`0x00-0xFF`范围内的编码，也需要用2字节表示。`UTF-16`在表示上有大小端之分，即字节序。

### UTF-32

`UTF-32` 是一种非常费内存但是转换方法简单的编码，每个字符均为4字节，以32位无符号整数为单位 ，这种做法的好处是无需转换，正好对应了`Unicode`的码位。`Unicode`的`UTF-32`编码就是其对应的32位无符号整数。同样的`UTF-32`也有大小端之分。

## 关于字节序
通过上面的描述，我们知道字节序有`Little Endian` `Big Endian` 之分。在我们传输过程中，要根据字节序，进行相应的编解码，才可以的到准确的数据。

> `endian`一词来源于乔纳森·斯威夫特的小说格列佛游记。小说中，小人国为水煮蛋该从大的一端剥开还是小的一端剥开而争论，争论的双方分别被称为`Big Endian` `Little Endian`。同样的战争也爆发在我们字节传送的过程中。1980年，`Danny Cohen`在其著名的论文`On Holy Wars and a Plea for Peace`中为平息一场关于字节该以什么样的顺序传送的争论而引用了该词。

### BOM

`Unicode`标准建议用`BOM（Byte Order Mark）`来区分字节序，即在传输字节流前，先传输被作为`BOM`的字符“零宽无中断空格”。这个字符的编码是`FEFF`，而反过来的`FFFE（UTF-16）`和`FFFE0000（UTF-32）`在`Unicode`中都是未定义的码位，不应该出现在实际传输中。

下表是各种`UTF`编码的`BOM`：

| UTF编码             | Byte Order Mark (BOM) |
| ----------------- | --------------------- |
| UTF-8 without BOM | 无                     |
| UTF-8 with BOM    | EF BB BF              |
| UTF-16LE          | FF FE                 |
| UTF-16BE          | FE FF                 |
| UTF-32LE          | FF FE 00 00           |
| UTF-32BE          | 00 00 FE FF           |


**UTF-8本身并不需要用BOM来区分，因为他传输过程中没有字节序一说，仅仅是微软为其加上了BOM，用于识别UTF-8文件，其实并无作用。**








