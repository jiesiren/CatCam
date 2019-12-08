#pragma once

#include <camera/NDKCameraManager.h>

class CatNdkCamera {
public:
    CatNdkCamera();
    ~CatNdkCamera();

    CatNdkCamera(const CatNdkCamera&) = delete;
    CatNdkCamera& operator=(const CatNdkCamera&) = delete;

private:
    ACameraManager* cameraManager_;
};
