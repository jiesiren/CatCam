#pragma once

#include <media/NdkImageReader.h>

class CatImageReader {
public:
    CatImageReader(int32_t width, int32_t height, AIMAGE_FORMATS format);

private:
    AImageReader* reader_;
};
