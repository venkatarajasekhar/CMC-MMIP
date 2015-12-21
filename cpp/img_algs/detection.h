#ifndef _DETECTION_H_
#define _DETECTION_H_

#include <cstring>
#include "image.h"

namespace Detection
{
    template <class T>
    Image<T> sobel(Image<T>& src, const char* dir)
    {
        const size_t _width = src.width();
        const size_t _height = src.height();
        Image<T> dst(_width, _height);
        if (!strcmp(dir, "x"))
            for (int i = 0; i < _height; i++)
                for (int j = 0; j < _width; j++)
                    dst(i, j) = src(i + 1, j - 1) + 2 * src(i + 1, j) + src(i + 1, j + 1) -
                        src(i - 1, j - 1) - 2 * src(i - 1, j) - src(i - 1, j + 1) + 128;
        if (!strcmp(dir, "y"))
            for (int i = 0; i < _height; i++)
                for (int j = 0; j < _width; j++)
                    dst(i, j) = src(i - 1, j - 1) + 2 * src(i, j - 1) + src(i + 1, j - 1) -
                        src(i - 1, j + 1) - 2 * src(i, j + 1) - src(i + 1, j + 1) + 128;
        return dst;
    }

} /* namespace Detection */

#endif /* _DETECTION_H_ */
