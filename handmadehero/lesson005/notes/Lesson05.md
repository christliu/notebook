# 第五天大纲



* ``` python
  WindowClass.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
  ```

  * [CS_CLASSDC](<https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-class-styles>): 同一个进程中的所有的同一个窗口类共享同一个[Device Context](<https://docs.microsoft.com/en-us/windows/desktop/gdi/device-contexts>)，当不同线程同时使用DC的时候，操作系统只允许一个线程完成绘制制作。
  * HREDRAW: 表示y方向变化的时候是不是要重绘整个窗口。
  * VREDRAW: 表示x方向变化的时候是不是要重绘整个窗口。

* 当函数形参不会改变指针行为的时候，尽量避免使用指针形参。

* 将一系列的global_variables写在一个Buffer结构体中。

* 将GetClientRect多次调用封装成函数，返回一个自定义的结构体

  ~~~ cpp
  struct win32_window_dimension
  {
      int Width;
      int Height;
  };
  ~~~
  
  ~~~ cpp
  internal win32_window_dimension
  GetWindowDimension(HWND window)
  {
      RECT ClientRect;
      win32_window_dimension Result;
      GetClientRect(window, &ClientRect);
      Result.Width = ClientRect.right - ClientRect.left;
      Result.Height = ClientRect.bottom - ClientRect.top;
      return Result;
  }
  
  ~~~
  
* 将BackBuff在程序启动的时候初始化为固定大小的buffer， WM_SIZE消息中不再改变BackBuffer的Size。