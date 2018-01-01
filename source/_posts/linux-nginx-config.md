---
title: Nginx使用以及相关配置
date: 2018-01-01 00:00:00
tags: Linux
---

Nginx（发音同engine x）是一个 Web服务器，也可以用作反向代理，负载平衡器和 HTTP缓存。本文主要介绍下nginx的使用方法，以及如何在一台服务器上配置多个域名和端口。

> 关于nginx的相关应用场景我们这里不做详细说明，介绍两个相关名词，常见用于搭建web服务器的两种架构：
> * LNMP指的是：Linux + Nginx + MySQL + PHP这种网站服务器架构。
> * LAMP指的是：Linux + Apache + MySQL  + PHP（有时也是指Perl或Python）。

<!-- more --> 

## Nginx命令

* -?,-h ：查看帮助
* -v ：显示版本号
* -V : 显示版本号以及详细配置参数
* -t  : 测试nginx配置文件是否正确，-t后可加具体文件路径，如若不加参数，可用此命令显示默认配置文件所在路径（/etc/nginx/nginx.conf）
* -s signal : 发送信号来控制程序，信号有: stop, quit, reopen, reload
  * `nginx -s reload` ：修改配置后重新加载生效
  * `nginx -s stop` ：快速停止nginx，立刻退出
  * `nginx -s quit` ：完整有序的停止nginx，完成已接受的连接请求再退出
* -p prefix ：设置前缀路径 (default: /etc/nginx/)
* -c filename ：设置配置文件 (默认: /etc/nginx/nginx.conf)
* -g directives ：设置全局指令

## 配置文件
我们可以通过配置文件，来实现在一台服务器上配置多个域名或者端口，我们每配置一个`server`，其实可理解为，就是将我们配置的路径（不同域名和端口）指向我们配置中的指定路径（服务器上的某文件夹），从而可以让客户端访问到此路径上的文件。默认解析当前路径下的`index.html`作为呈现页面。

### 修改配置文件，加载所有子配置
nginx的总配置文件默认是`/etc/nginx/nginx.conf`我们想加载开启多个服务配置，可用多个配置文件实现，每一个配置文件对应一个服务，我们先要在`nginx.conf`中加载所有配置。
```bash
http {
    include       /etc/nginx/mime.types;
    default_type  application/octet-stream;
    #gzip  on
    ...
    # 加入此句话，即可加载conf.d文件夹下，所有conf后缀的文件。
    include /etc/nginx/conf.d/*.conf;
}
```

### 添加一新的子配置文件
每个`sever`的配置单独对应`conf.d`下的一个文件，以`.conf`为后缀。默认有一个`default.conf`我们可使用`cp default.conf custom.conf`复制出一份配置，然后修改即可。下面我们看下默认配置：

```bash
server {
    listen       80; # 1、默认的80端口
    server_name  localhost; # 2、服务名称，默认为本机，可改为相关域名
    
    #charset koi8-r;
    #access_log  /var/log/nginx/host.access.log  main;
    
    location / {
        root   /usr/share/nginx/html; # 3、将sever解析到那个路径下
        index  index.html; # 4、访问时默认加载解析的文件
    }
    
    #error_page  404              /404.html;
    
    # redirect server error pages to the static page /50x.html
    #
    error_page   500 502 503 504  /50x.html;
    location = /50x.html {
        root   /usr/share/nginx/html;
    }
}
```

我们只需要修改上述4处配置即可：
* listen：端口修改为非默认的80端口后，访问时需在域名后加`:port`来进行访问
* server_name：默认的`localhost`即代表本机，可通过本机`ip`或指向此`ip`的域名来访问，我们如果有多个指向服务器`ip`的域名，可以通过配置此项，并且修改`配置3`的路径，来达到多个域名分别做不同解析的目的。


