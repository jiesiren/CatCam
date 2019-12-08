#include "CatNdkCamera.h"

#include "CatLog.h"

CatNdkCamera::CatNdkCamera() {
    LOGI("CatNdkCamera::CatNdkCamera()");
    cameraManager_ = ACameraManager_create();
}

CatNdkCamera::~CatNdkCamera() {
    ACameraManager_delete(cameraManager_);
}
