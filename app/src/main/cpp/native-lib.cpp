#include <jni.h>
#include <string>
#include "log.h"


extern "C" void android_main(struct android_app* state) {
    while (1) {
        LOGI("Loopy loop");
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_jiesiren_catcam_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
