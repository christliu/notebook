# 编译环境搭建

* 一些技巧:

  * subst w: C:\Users\Christliu\gitrepo\handmadehero 会将后面的目录挂载到w盘。
  * subst w: \d 去掉w盘的挂载
  * 找到系统vc2017安装目录下的vcvarsall.bat，运行之后会设置vs的环境变量，例如cl等。

  ​       在本系统下（win10）目录为c:\Program Files (x86)\Microsoft Visual         Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat

  * 找到startup目录，在本系统下位于C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp 目录的bat文件，在系统启动的时候会默认执行，类似于linux系统的.bashrc。

  * 桌面新建cmd.exe的快捷方式，右键，在目标这一项后面添加

    /k w:\handmadehero\startup.bat，会在启动cmd.exe的时候默认执行此startup.bat文件，从而设置好一些环境变量。

  * pushd  popd比cd命令好用。不用一层一层cd出来。

  * **%cd%**取到的是运行环境的路径， **%~dp0**取到的是当前可执行文件的目录

* MessageBox函数需要链接库user32.lib，这个信息在msdn文档MessageBox说明的最后会列出来，包括需要的头文件，库文件，操作系统版本等信息。

* MessageBox包括MessagBoxW和MessageBoxA两个版本，分别是宽字节和ANSI版本。

* wchat_t称为宽字节，每个字符都占两个字节。char为多字节字符，一个char只有一个字节，所以对于中文字符，需要用多个char存储。字符常量声明为宽字节，需要在前面加上L。