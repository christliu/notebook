# NativeActivity代码框架

为了探索Andriod Engine的代码开发框架，本节给出最基本的NativeActivity用法。由于Android Studio对于ndk的迭代已经官方推荐cmake的方式，但是Messiah引擎是基于ndk的Android.mk编译方式制作的，所以本节给出基于Android.mk的最基本的NativeActivity框架，相关代码上传至 https://github.com/christliu/notebook/tree/master/opengles/code/Empty 。

1. 首先创建一个Emtpy Project。删掉src/main/java当中的代码。

2. 新建src/main/cpp目录，新建代码main.cpp，如下：

   ~~~cpp
   //
   // Created by gzliuyu2014 on 2020/1/13.
   //
   
   #include <android_native_app_glue.h>
   #include <android/log.h>
   
   #define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
   #define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
   
   void ChristliuHandleCmd(struct android_app* app, int32_t cmd)
   {
       LOGI("cmd run ", cmd);
   }
   
   void android_main(struct android_app* state)
   {
       state->onAppCmd = ChristliuHandleCmd;
      while(1)
      {
           int id;
           int events;
           android_poll_source* source;
           while((id == ALooper_pollAll(-1, NULL, &events, (void**)&source))>= 0)
           {
               if(source != NULL)
               {
                   source->process(state, source);
               }
           }
      }
   }
   ~~~

   新建src/main/cpp/Android.mk，

   ~~~cmake
   LOCAL_PATH := $(call my-dir)
   
   include $(CLEAR_VARS)
   
   LOCAL_MODULE    := native-activity
   LOCAL_SRC_FILES := main.cpp
   LOCAL_LDLIBS    := -llog -landroid
   LOCAL_STATIC_LIBRARIES := android_native_app_glue
   
   include $(BUILD_SHARED_LIBRARY)
   
   $(call import-module,android/native_app_glue)
   
   ~~~

3. 修改AndroidManifest.xml

   ~~~xml
   android:hasCode="false"
   ...
   <activity android:name="android.app.NativeActivity">
               <meta-data android:name="android.app.lib_name"
                   android:value="native-activity" />
   ~~~

4. 在Android视图下，Link project with c++，选择ndk-build, 并选择Android.mk路径。

   或者直接修改build.gradle，添加

   ~~~cmake
   externalNativeBuild {
           ndkBuild {
               path file('src/main/cpp/Android.mk')
           }
       }
   ~~~

5. 编译运行即可。