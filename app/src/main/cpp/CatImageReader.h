#pragma once

#include <media/NdkImageReader.h>

/**
 * Wrapper class for AImageReader.
 */
class CatImageReader {
public:
    CatImageReader(int32_t width, int32_t height, AIMAGE_FORMATS format);
    ~CatImageReader();

    ANativeWindow* getWindow();

private:
    AImageReader* imageReader_;
};
