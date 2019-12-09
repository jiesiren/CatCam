#include <camera/NdkCameraMetadata.h>
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

void cameraManagerOnCameraAvailable(void* context, const char* cameraId) {
    LOGI("Camera available %s", reinterpret_cast<const char*>(context));
    // TODO: set available somewhere.
}

void cameraManagerOnCameraUnavailable(void* context, const char* cameraId) {
    LOGI("Camera available %s", reinterpret_cast<const char*>(context));
    // TODO: set unavailable somewhere.
}

CatNdkCamera::CatNdkCamera() : cameraManager_(nullptr), cameraDevice_(nullptr) {
    LOGI("CatNdkCamera::CatNdkCamera()");
    cameraManager_ = ACameraManager_create();

    cameraId_ = queryCameraId();
    ASSERT(cameraId_.size(), "Empty camera ID");
    LOGI("Camera ID found: %s", cameraId_.c_str());

    ACameraDevice_StateCallbacks stateCallbacks = {
        .context = const_cast<char*>(cameraId_.c_str()),
        .onDisconnected = cameraDeviceOnDisconnected,
        .onError = cameraDeviceOnError,
    };
    camera_status_t status = ACameraManager_openCamera(cameraManager_, cameraId_.c_str(),
                                                       &stateCallbacks, &cameraDevice_);
    if (status != ACAMERA_OK) {
        LOGE("Could not open camera with ID %s (%d)", cameraId_.c_str(), status);
        throw std::runtime_error("ACameraManager_openCamera");
    }

    availCallbacks_ = {
        .context = const_cast<char*>(cameraId_.c_str()),
        .onCameraAvailable = cameraManagerOnCameraAvailable,
        .onCameraUnavailable = cameraManagerOnCameraUnavailable,
    };
    ACameraManager_registerAvailabilityCallback(cameraManager_, &availCallbacks_);
}

CatNdkCamera::~CatNdkCamera() {
    ACameraManager_unregisterAvailabilityCallback(cameraManager_, &availCallbacks_);
    if (cameraDevice_ != nullptr) {
        ACameraDevice_close(cameraDevice_);
    }
    if (cameraManager_ != nullptr) {
        ACameraManager_delete(cameraManager_);
    }
}

/**
 * Find the camera ID of the most suitable camera on the device.
 */
std::string CatNdkCamera::queryCameraId() const {
    ACameraIdList* cameraIdList;
    camera_status_t status = ACameraManager_getCameraIdList(cameraManager_, &cameraIdList);
    if (status != ACAMERA_OK) {
        LOGE("Could not get camera ID list (%d)", status);
        return {};
    }

    std::string cameraId;
    // Find first back facing camera, or the first camera in the list otherwise.
    for (int i = 0; i < cameraIdList->numCameras; i++) {
        const char* id = cameraIdList->cameraIds[i];

        if (cameraId.empty()) {
            cameraId = id;
        }

        ACameraMetadata* metadata;
        status = ACameraManager_getCameraCharacteristics(cameraManager_, id, &metadata);
        if (status != ACAMERA_OK) {
            LOGE("Could not get camera characteristics for camera ID %s", id);
            return {};
        }

        ACameraMetadata_const_entry entry;
        status = ACameraMetadata_getConstEntry(metadata, ACAMERA_LENS_FACING, &entry);
        if (status != ACAMERA_OK) {
            LOGE("Could not get metadata entry for camera ID %s, tag %d", id, ACAMERA_LENS_FACING);
            return {};
        }

        auto facing =
            static_cast<acamera_metadata_enum_android_lens_facing_t>(*(entry.data.u8));

        ACameraMetadata_free(metadata);

        LOGI("LENS_FACING: %d", facing);

        if (facing == ACAMERA_LENS_FACING_BACK) {
            cameraId = id;
            break;
        }

    }

    ACameraManager_deleteCameraIdList(cameraIdList);
    return cameraId;
}
