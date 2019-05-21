# 第六天大纲



* XInput API可以输入手柄数据

  ~~~ cpp
  DWORD XInputGetState(
    DWORD        dwUserIndex,
    XINPUT_STATE *pState
  );
  ~~~

  * dwUserIndex: 一个应用支持同时插入4个360XBOX手柄，此参数标识是哪个手柄。
  * pState 返回此手柄当前的状态。其中包括哪些按键被按下。

* 依赖静态库xinput.lib，动态库xinput1_4.dll（教程中使用的是xinput1_3.dll）。为了解决可能在客户端中不存在dll动态库而crash，采用了在代码中动态加载dll的策略，同时处理了加载不到Dll的异常情况，避免crash。

  ~~~ cpp
  #define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
  typedef X_INPUT_GET_STATE(x_input_get_state);
  X_INPUT_GET_STATE(XInputGetStateStub)
  {
      return 0;
  }
  global_variable x_input_get_state *XInputGetState_ = XInputGetStateStub;
  #define XInputGetState XInputGetState_
  ~~~

  定义一系列的宏，其中X_INPUT_GET_STATE可以定义一种函数的声明。用它定义过的x_input_get_state表示了一种函数类型。XInputGetState_是函数指针，代表了实际上使用的XInputGetState函数。

* ~~~ cpp
  internal void
  Win32LoadXInput(void)
  {
      HMODULE XInputLibrary = LoadLibrary("xinput1_3.dll");
      if(XInputLibrary)
      {
          XInputGetState_ = (x_input_get_state *)GetProcAddress(XInputLibrary, "XInputGetState");
          XInputSetState_ = (x_input_set_state *)GetProcAddress(XInputLibrary, "XInputSetState");
      }
      else
      {
      }
  }
  
  ~~~

  这段代码展示了如何动态加载dll，同时将XInputGetState指针赋予真正的函数地址。