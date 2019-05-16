# 第三天大纲

* 在windows退出的时候不执行析构，可以节省用户退出的时间

* 关于static

  ```cpp
  #define local_persist static  表示在局部作用域保持静态
  #define global_variable static  表示全局的变量
  #define internal static   表示定义的函数在别的文件不能调用
  ```

  