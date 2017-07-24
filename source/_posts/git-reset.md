---
title: Git中使远程仓库覆盖本地修改
date: 2016-12-14 00:00:00
tags: Git
---

由于Git是分布式版本控制系统，和SVN的集中式管理不同，每个人电脑上均为完整的库，不用必须与服务器连接。所以当我们做了某些修改，想要放弃此修改时，在SVN中，我们可以删除这部分代码，然后重新update即可。那么在git中如何操作呢，我们将利用`reset`、`clean`命令解决此问题。这些命令不指定路径则默认在当前路径下执行。

<!-- more --> 

## reset命令
`git reset [--hard|soft|mixed|merge|keep] [<commit>或HEAD]`：将当前的分支`reset`到指定的`<commit>`或者`HEAD`，如果不显示指定`commit`，默认是`HEAD`，即最新的一次提交。

* `--hard` 重设（reset） index和working directory，自从`<commit>`以来在working directory中的任何改变都被丢弃，并把`HEAD`指向`<commit>`。此为彻底回退。

* `--soft` index和working directory中的内容不作任何改变，仅仅把`HEAD`指向`<commit>`。这个模式的效果是，执行完毕后，自从`<commit>`以来的所有改变都会显示在git status的"Changes to be committed"中

```bash
#回退所有内容到上一个版本 
git reset HEAD^ 

#回退test.cpp这个文件的版本到上一个版本 
git reset HEAD^ test.cpp 

#向前回退到第n个版本 
git reset –-soft HEAD~n

#将本地的状态回退到和远程的一样,此将删除本地修改代码。
git reset –-hard origin/master 

#回退到某个版本
git reset '版本号' 

#回退到上一次提交的状态，按照某一次的commit完全反向的进行一次commit 
git revert HEAD  
```

## clean命令

`git clean`经常和`git reset --hard`一起结合使用。 `reset`只影响被track过的文件，`clean`可删除没有track过的文件。结合这两个命令能让你的工作目录完全回到一个指定的`<commit>`的状态。

```bash
#显示待删除文件.
git clean -n

#删除当前目录下所有没有track过的文件. 不会删除.gitignore里面指定的文件。
git clean -f

#删除当前目录下没有被track过的文件夹.
git clean -d

#删除.gitignore里所指定的文件. 
git clean -x

#组合使用，全部删除。
git clean -xdf
```