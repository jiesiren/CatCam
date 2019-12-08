#include <jni.h>
#include <string>
#include <android/looper.h>
#include <android_native_app_glue.h>
#include "CatLog.h"

static void appGlueOnAppCmd(struct android_app* app, int32_t cmd) {
    LOGI("onAppCmd %d", cmd);
}

extern "C" void android_main(struct android_app* state) {
    state->onAppCmd = appGlueOnAppCmd;

    while (1) {
        // Read all pending events.
        int events;
        struct android_poll_source* source;

        while (ALooper_pollAll(0, nullptr, &events, (void**) &source) >= 0) {
            // Process this event.
            if (source != nullptr) {
                LOGI("Process!");
                source->process(state, source);
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                LOGI("Destroy!");
                return;
            }
        }
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_jiesiren_catcam_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
