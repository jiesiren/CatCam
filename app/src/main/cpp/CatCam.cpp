#include "CatCam.h"

#include <jni.h>
#include <string>
#include <android/looper.h>
#include <android_native_app_glue.h>

#include "CatLog.h"

// "Main" entry point function called from app glue
extern "C" void android_main(struct android_app* app) {
    CatCam catcam(app);

    // "Main loop" - Poll looper from app glue to process events
    while (true) {
        int events;
        struct android_poll_source* source;

        while (ALooper_pollAll(0, nullptr, &events, reinterpret_cast<void**>(&source)) >= 0) {
            if (source != nullptr) {
                LOGI("Process!");
                source->process(app, source);
            }

            if (app->destroyRequested != 0) {
                LOGI("Destroy!");
                return;
            }
        }
    }
}

CatCam::CatCam(struct android_app* app) : app_(app) {
    app_->userData = reinterpret_cast<void*>(this);
    app_->onAppCmd = CatCam::onAppCmd;
}

CatCam* CatCam::getInstanceFromApp(struct android_app* app) {
    return reinterpret_cast<CatCam*>(app->userData);
}

void CatCam::onAppCmd(struct android_app* app, int32_t cmd) {
    LOGI("onAppCmd %d", cmd);
    CatCam& catCam = *CatCam::getInstanceFromApp(app);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_jiesiren_catcam_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
