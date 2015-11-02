#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cstddef>
#include <cstring>

/* 
 * Template for base image type
 */
template<class PixelType>
class Image
{
    private:
        size_t _width;
        size_t _height;
        PixelType *_data;
    public:
        Image(size_t width, size_t height);
        ~Image();
        PixelType& operator() (size_t row, size_t col);
        size_t width() const;
        size_t height() const;
};

/*
 * Implementation of Image template
 */
template <class PixelType>
Image<PixelType>::Image(size_t width, size_t height): _width(width), _height(height)
{
    _data = new PixelType[_width * _height];
    if (!_data)
        throw "Image: Insufficient memory to allocate raw data";
    std::memset(_data, 0, _width * _height * sizeof(PixelType));
}

template <class PixelType>
Image<PixelType>::~Image() { delete[] _data; }

template <class PixelType>
size_t Image<PixelType>::width() const { return _width; }

template <class PixelType>
size_t Image<PixelType>::height() const { return _height; }

template <class PixelType>
PixelType& Image<PixelType>::operator() (size_t row, size_t col)
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
