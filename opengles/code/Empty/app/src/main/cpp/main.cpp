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