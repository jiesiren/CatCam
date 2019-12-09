#include "CatImageReader.h"

#include <stdexcept>
#include "CatLog.h"

CatImageReader::CatImageReader(int32_t width, int32_t height, AIMAGE_FORMATS format)
        : imageReader_(nullptr) {
    LOGI("CatImageReader::CatImageReader()");

    constexpr int32_t BUFFER_MAX_IMAGES = 4;
    media_status_t status = AImageReader_new(width, height, format, BUFFER_MAX_IMAGES,
                                             &imageReader_);
    if (status != AMEDIA_OK) {
        LOGE("Could not create new AImageReader (%d)", status);
        throw std::runtime_error("AImageReader_new");
    }
}

CatImageReader::~CatImageReader() {
    AImageReader_delete(imageReader_);
}

ANativeWindow* CatImageReader::getWindow() {
    ANativeWindow *window;
    media_status_t status = AImageReader_getWindow(imageReader_, &window);
    if (status != AMEDIA_OK) {
        LOGE("Could not get native window (%d)", status);
        throw std::runtime_error("AImageReader_getWindow");
    }
    return window;
}
