# Ch01.Starting Vim

1. Vim为每打开一个文件会打开一个缓冲区，当缓冲区有内容时不允许直接退出，必须:q!或:wq

2. `vim +%s/can/can not/g file1.txt`

   再打开文件时立刻将所有包含can的文本替换为can not

3. vim编辑到一半想返回时可以用 ctrl+z 或者 :suspend暂时挂起，然后再终端输入 fg 时可以继续编辑

4. 使用man vim查看vim支持的操作





# Ch02.Buffers, Windows, and Tabs

1. 如果没有用-o参数打开分隔多个窗口，vim打开多个文件时只会显示第一个，但会为所有文件打开的文件创建缓冲区，`:buffers`可以看到多个文件缓冲区，`:buffer2`可以切换到第二个文件，`:bnext`跳转到下一个打开的文件缓冲区

2. 使用ctrl + o可以跳转到旧缓冲区的位置（包括之前打开但已经关闭的文件），使用ctrl+i可以跳转到新缓冲区的位置

3. 一旦打开一个缓冲区，它将保留在缓冲队列中，需要使用`:bdelete`删除

4. ctil + ^ 返回上一个缓冲区

5. `:qall!`离开所有缓冲区并放弃缓冲区数据

6. 也可以让缓冲区以window的形式一并显示出来，而不用`:buffer`进行切换才能看到内容，`:spilt file2.txt`

7. 窗口的光标切换

   ```
   Ctrl-W H    Moves the cursor to the left window
   Ctrl-W J    Moves the cursor to the window below
   Ctrl-W K    Moves the cursor to the window upper
   Ctrl-W L    Moves the cursor to the right window
   ```

8. `:vspilt file3.txt`划垂直线进行切分，使得此时光标可以左右切换，而不再只是上下

9. 一般都是一个window展示一个buffer的内容，也可以让多个window展示相同的buffer，仅仅是显示而已，并不把内容写入其他文件，毕竟只是显示相同的缓冲区

10. ```
    :vsplit filename    Split window vertically
    :split filename     Split window horizontally
    :new filename       Create new window
    ```

    Take your time to understand them. For more, check out `:h window`.

11. tab更像平时我们阅读多个浏览器页面，而不是像window一样全部显示出来，**不过关闭一个tab并不会关闭一个文件，更不会关闭其缓冲区**

12. ```
    :tabnew file.txt    Open file.txt in a new tab
    :tabclose           Close the current tab
    :tabnext 或者说:tabnext2/3          Go to next tab
    :tabprevious        Go to previous tab
    :tablast            Go to last tab
    :tabfirst           Go to first tab
    ```

13. vim -p 参数可以将所有文件以tab方式打开

14. 