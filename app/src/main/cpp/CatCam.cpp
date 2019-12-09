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

    camera_ = nullptr;
    reader_ = nullptr;
}

CatCam::~CatCam() {
    deleteCamera();
}

void CatCam::initCamera() {
    camera_ = new CatNdkCamera();
    reader_ = new CatImageReader(640, 480, AIMAGE_FORMAT_YUV_420_888);
}

void CatCam::deleteCamera() {
    delete camera_;
    camera_ = nullptr;

    delete reader_;
    reader_ = nullptr;
}

CatCam* CatCam::getInstanceFromApp(struct android_app* app) {
    return reinterpret_cast<CatCam*>(app->userData);
}

void CatCam::onAppCmd(struct android_app* app, int32_t cmd) {
    LOGI("CatCam::onAppCmd %d", cmd);
    CatCam& catCam = *CatCam::getInstanceFromApp(app);

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            LOGI("CatCam::onAppCmd - APP_CMD_INIT_WINDOW");
            catCam.onAppInitWindow();
            break;
        case APP_CMD_TERM_WINDOW:
            LOGI("CatCam::onAppCmd - APP_CMD_TERM_WINDOW");
            catCam.onAppTermWindow();
            break;
        default:
            break;
    }
}

void CatCam::onAppInitWindow() {
    requestPermissions();
    showAppUi();
    initCamera(); // TODO: will need to be moved/called-later for when permission req needed
}

void CatCam::onAppTermWindow() {

}

void CatCam::showAppUi() {
    LOGI("CatCam::showAppUi");
    JNIEnv* env;
    ANativeActivity* activity = app_->activity;
    activity->vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

    activity->vm->AttachCurrentThread(&env, nullptr);

    jobject activityObj = env->NewGlobalRef(activity->clazz);
    jclass clz = env->GetObjectClass(activityObj);
    env->CallVoidMethod(activityObj, env->GetMethodID(clz, "showAppUi", "()V"));
    env->DeleteGlobalRef(activityObj);

    activity->vm->DetachCurrentThread();
}

void CatCam::requestPermissions() {
    LOGI("CatCam::requestPermissions");
    JNIEnv* env;
    ANativeActivity* activity = app_->activity;
    activity->vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

    activity->vm->AttachCurrentThread(&env, nullptr);

    jobject activityObj = env->NewGlobalRef(activity->clazz);
    jclass clz = env->GetObjectClass(activityObj);
    env->CallVoidMethod(activityObj, env->GetMethodID(clz, "requestPermissions", "()V"));
    env->DeleteGlobalRef(activityObj);

    activity->vm->DetachCurrentThread();
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_jiesiren_catcam_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
