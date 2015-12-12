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
    private:
        size_t _width;
        size_t _height;
        T *_data;
    public:
        Image(const size_t width, const size_t height);
        Image(const size_t width, const size_t height, const uint8_t* data);
        ~Image();
        T& operator() (const size_t row, const size_t col);
        size_t width() const;
        size_t height() const;
};

/*
 * Implementation of Image template
 */
template <class T>
Image<T>::Image(const size_t width, const size_t height):
    _width(width), _height(height)
{
    _data = new T[_width * _height];
    std::memset(_data, 0, _width * _height * sizeof(T));
}

template <class T>
Image<T>::Image(const size_t width, const size_t height, const uint8_t* data):
    _width(width), _height(height)
{
    _data = new T[_width * _height];
    std::memcpy(_data, data, _width * _height);
}

template <class T>
Image<T>::~Image() { delete[] _data; }

template <class T>
size_t Image<T>::width() const { return _width; }

template <class T>
size_t Image<T>::height() const { return _height; }

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
