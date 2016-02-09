/*
 * This is a part of MMIP project
 * Copyright (C) 2016  Igor A. Munkin (munkin.igor@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
                        src(i - 1, j - 1) - 2 * src(i - 1, j) - src(i - 1, j + 1) + Image<T>::GRAY;
        if (!strcmp(dir, "y"))
            for (int i = 0; i < _height; i++)
                for (int j = 0; j < _width; j++)
                    dst(i, j) = src(i - 1, j - 1) + 2 * src(i, j - 1) + src(i + 1, j - 1) -
                        src(i - 1, j + 1) - 2 * src(i, j + 1) - src(i + 1, j + 1) + Image<T>::GRAY;
        return dst;
    }

} /* namespace Detection */

#endif /* _DETECTION_H_ */
