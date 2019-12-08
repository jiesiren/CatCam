#pragma once

#include <android_native_app_glue.h>

extern "C" void android_main(struct android_app* app);

class CatCam {
public:
    CatCam(struct android_app* app);

    static CatCam* getInstanceFromApp(struct android_app* app);

    // app cmd callback for app glue, see android_native_app_glue.h
    static void onAppCmd(struct android_app* app, int32_t cmd);

private:
    struct android_app* app_;
};
