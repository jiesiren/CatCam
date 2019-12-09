#include "CatNdkCamera.h"

#include "CatLog.h"

void cameraDeviceOnDisconnected(void* context, ACameraDevice* device) {
    LOGI("Camera device disconnected %s", reinterpret_cast<const char*>(context));
    ACameraDevice_close(device);
}

void cameraDeviceOnError(void* context, ACameraDevice* device, int error) {
    LOGI("Camera device errored %s (%d)", reinterpret_cast<const char*>(context), error);
    ACameraDevice_close(device);
}

CatNdkCamera::CatNdkCamera() : cameraManager_(nullptr), cameraDevice_(nullptr) {
    LOGI("CatNdkCamera::CatNdkCamera()");
    cameraManager_ = ACameraManager_create();

    cameraId_ = queryCameraId();
    ASSERT(cameraId_.size(), "Empty camera ID");
    LOGI("Camera ID found: %s", cameraId_.c_str());

    ACameraDevice_StateCallbacks callbacks = {
        .context = const_cast<char*>(cameraId_.c_str()),
        .onDisconnected = cameraDeviceOnDisconnected,
        .onError = cameraDeviceOnError,
    };
    camera_status_t status = ACameraManager_openCamera(cameraManager_, cameraId_.c_str(),
                                                       &callbacks, &cameraDevice_);
    if (status != ACAMERA_OK) {
        LOGE("Could not open camera with ID %s (%d)", cameraId_.c_str(), status);
        throw std::runtime_error("ACameraManager_openCamera");
    }
}

CatNdkCamera::~CatNdkCamera() {
    if (cameraDevice_ != nullptr) {
        ACameraDevice_close(cameraDevice_);
    }
    if (cameraManager_ != nullptr) {
        ACameraManager_delete(cameraManager_);
    }
}

std::string CatNdkCamera::queryCameraId() const {
    ACameraIdList* cameraIdList;
    camera_status_t status = ACameraManager_getCameraIdList(cameraManager_, &cameraIdList);
    if (status != ACAMERA_OK) {
        LOGE("Could not get camera ID list (%d)", status);
        return {};
    }

    // TODO: can iterate through cameras and pick most appropriate one
    std::string cameraId;
    if (cameraIdList->numCameras > 0) {
        cameraId = cameraIdList->cameraIds[0];
    }

    ACameraManager_deleteCameraIdList(cameraIdList);
    return cameraId;
}
