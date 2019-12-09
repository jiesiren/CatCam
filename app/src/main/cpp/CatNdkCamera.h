#pragma once

#include <string>

#include <camera/NDKCameraManager.h>

class CatNdkCamera {
public:
    CatNdkCamera();
    ~CatNdkCamera();

    CatNdkCamera(const CatNdkCamera&) = delete;
    CatNdkCamera& operator=(const CatNdkCamera&) = delete;

private:
    std::string queryCameraId() const;

    std::string cameraId_;
    ACameraManager* cameraManager_;
    ACameraDevice* cameraDevice_;
};
