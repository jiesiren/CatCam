#pragma once

#include <string>

#include <camera/NDKCameraManager.h>

/**
 * Wrapper class for managing the camera related objects from the camera2 NDK module.
 */
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
    ACameraManager_AvailabilityCallbacks availCallbacks_;
    ACaptureSessionOutputContainer* outputContainer_;
};
