# 第二天大纲

* 关于MSDN的struct声明：

  ``` cpp
  typedef struct tagWNDCLASSA {
    UINT      style;
    WNDPROC   lpfnWndProc;
    int       cbClsExtra;
    int       cbWndExtra;
    HINSTANCE hInstance;
    HICON     hIcon;
    HCURSOR   hCursor;
    HBRUSH    hbrBackground;
    LPCSTR    lpszMenuName;
    LPCSTR    lpszClassName;
  } WNDCLASSA, *PWNDCLASSA, *NPWNDCLASSA, *LPWNDCLASSA;
  ```

  之所以写成不直接写成

  ``` cpp
  struct WNDCLASS {
      ...
  }
  ```

  的形式，是为了兼容c和cpp两种语言。同时以上声明表示

  ``` cpp
  WNDCLASSA -> tagWNDCLASSA
  PWNDCLASSA -> tagWNDCLASSA *
  ```

  

  

  