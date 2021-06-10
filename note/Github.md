# clone使用已有仓库





## 初始化仓库

创建一个目录，并将其作为git仓库，在该目录下`git init`。

如果执行成功，该目录下会生成`.git`目录，该目录下的内容我们称为“附属于该仓库的工作树"。文件的编辑等操作在工作树中进行，然后记录到仓库中，以此管理文件的历史快照。**如果想将文件恢复到原先的状态，可以从仓库中调取之前的快照。**

## 查看仓库状态

在工作树和仓库被操作过程中，状态会不断发生变化。可用`git status`	查看。

```shell
On branch master
Your branch is up-to-date with 'origin/master'.

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
	modified:   "\346\225\260\346\215\256\347\273\223\346\236\204\344\270\216\347\256\227\346\263\225.md"

Untracked files:
  (use "git add <file>..." to include in what will be committed)
	../practice/.threadsafe_stack.cpp.swp
	../practice/threadsafe_stack.cpp

no changes added to commit (use "git add" and/or "git commit -a")

```

显示我们在master分支下，没有可提交Commit的内容，提交是值“记录工作树中所有文件的当前状态”，没有可提交即这个仓库没有记录到任何文件的任何状态。

## 暂存区添加文件

如果只是Git仓库的工作树创建了文件，那么该文件不会被计入Git仓库的版本管理对象中（如 Untracked files中的文件一样），此时使用`git add`命令将文件添加到暂存区，暂存区属于提交之前的一个临时区域。

## 保存仓库记录

`git commit`将当前在暂存区中的文件实际保存到仓库的历史记录中，通过这些记录我们可以在工作树中恢复文件。

`git commit `后面可以加 -m 参数添加简短信息，若要详细信息不使用参数，此时关闭编译器或终端会终止提交。



## 查看提交日志

`git log`显示提交的相关日志

- 使用`--prety=short`可以让程序显示第一行简述信息
- 后面添加目录名，只显示该目录下的提交日志
- 后面添加文件吗，只显示该文件下的提交日志
- 使用`-p 文件名` 参数会显示文件前后改变的区别

## 查看更改前后的区别

`git diff`可以查看工作树、暂存区、最新提交之间的差别

- 查看工作树和暂存区的差别

  新加的行前面+号，删除的行前面-号

- 查看工作树和最新提交的差别

  `git diff HEAD` （HEAD 是指向当前分支中最新一次提交的指针）

不妨养成一个好习惯：在执行git commit之前先执行 git diff HEAD命令，查看本次提交于上次提交之间有什么差别，确认后再提交。

## 分支操作

