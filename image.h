#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cstddef>
#include <cmath>
#include <complex>

/* 
 * Template for base image type
 */
template<class PixelType>
   class Image
    {
        private:
            size_t __width;
            size_t __height;
            PixelType *__data;
        public:
            Image(size_t _width, size_t _height);
            ~Image();
            PixelType& operator() (size_t row, size_t col);
            size_t width();
            size_t height();
            PixelType norm(size_t row, size_t col);
    };

/* 
 * Availible types of images
 */
typedef Image<float> GrayscaleFloatImage;
typedef Image<std::complex<float> > GrayscaleComplexImage;

/*
 * Implementation of Image template
 */
template <class PixelType>
    Image<PixelType>::Image(size_t _width, size_t _height): __width(_width), __height(_height)
    {
        __data = new PixelType[__width * __height];
    }

template <class PixelType>
    Image<PixelType>::~Image()
    {
        delete[] __data;
    }

template <class PixelType>
    PixelType& Image<PixelType>::operator() (size_t row, size_t col)
    {
        size_t i = row;
        size_t j = col;
        if (row < 0) i = 0;
        if (row >= __height) i = __height - 1;
        if (col < 0) j = 0;
        if (col >= __width) j = __width - 1;
        return __data[i * __width + j];
    }

template <class PixelType>
    size_t Image<PixelType>::width()
    {
        return __width;
    }

template <class PixelType>
    size_t Image<PixelType>::height()
    {
        return __height;
    }

template <class PixelType>
    PixelType Image<PixelType>::norm(size_t row, size_t col)
    {
        return abs(__data[row * __width + col]);
    }

#endif /* _IMAGE_H_ */
