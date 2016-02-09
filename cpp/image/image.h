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

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cstddef>
#include <cstring>
#include <stdint.h>

/* 
 * Template for base image type
 */
template<class T>
class Image
{
    public:
        static const uint8_t BLACK = 0;
        static const uint8_t GRAY = 128;
        static const uint8_t WHITE = 255;
    private:
        size_t _width;
        size_t _height;
        T *_data;
    public:
        Image();
        Image(const size_t width, const size_t height);
        Image(const size_t width, const size_t height, const uint8_t* data);
        Image(const Image& image);
        Image(Image&& image);
        ~Image();
        Image& operator=(const Image& image);
        Image& operator=(Image&& image);
        T& operator() (const size_t row, const size_t col);
        size_t width() const;
        size_t height() const;
};

/*
 * Implementation of Image template
 */
template <class T>
Image<T>::Image(): _width(0), _height(0) { _data = nullptr; }

template <class T>
Image<T>::Image(const size_t width, const size_t height):
    _width(width), _height(height)
{
    _data = new T[_width * _height];
    std::memset(_data, Image<T>::BLACK, _width * _height * sizeof(T));
}

template <class T>
Image<T>::Image(const size_t width, const size_t height, const uint8_t* data):
    _width(width), _height(height)
{
    _data = new T[_width * _height];
    std::memcpy(_data, data, _width * _height * sizeof(T));
}

template <class T>
Image<T>::Image(const Image<T>& image):
    _width(image._width), _height(image._height)
{
    _data = new T[_width * _height];
    std::memcpy(_data, image._data, _width * _height * sizeof(T));
}

template <class T>
Image<T>::Image(Image<T>&& image): _width(image._width), _height(image._height)
{
    _data = image._data;
    image._data = nullptr;
    image._width = 0;
    image._height = 0;
}

template <class T>
Image<T>::~Image() { delete[] _data; }

template <class T>
size_t Image<T>::width() const { return _width; }

template <class T>
size_t Image<T>::height() const { return _height; }

template <class T>
Image<T>& Image<T>::operator=(const Image<T>& image)
{
    if (_data)
        delete[] _data;
    _width = image._width;
    _height = image._height;
    _data = new T[_width * _height];
    std::memcpy(_data, image._data, _width * _height * sizeof(T));
    return *this;
}

template <class T>
Image<T>& Image<T>::operator=(Image<T>&& image)
{
    if (this != &image)
    {
        delete[] _data;
        _width = image._width;
        _height = image._height;
        _data = image._data;
        image._data = nullptr;
        image._width = 0;
        image._height = 0;
    }
    return *this;
}

template <class T>
T& Image<T>::operator() (const size_t row, const size_t col)
{
    size_t i = row;
    size_t j = col;
    if (row < 0) i = 0;
    if (row >= _height) i = _height - 1;
    if (col < 0) j = 0;
    if (col >= _width) j = _width - 1;
    return _data[i * _width + j];
}

/*
 * Availible types of images
 */
typedef Image<float> GrayscaleFloatImage;

#endif /* _IMAGE_H_ */
