#ifndef _LAB1_H_
#define _LAB1_H_

#include <cstdio>
#include <algorithm>
#include <cmath>
#include "image.h"

namespace Lab1
{
    template <class T>
    Image<T>* invert(Image<T>* src)
    {
        const size_t _width = src->width();
        const size_t _height = src->height();
        Image<T>* dst(new Image<T>(_width, _height));
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
                (*dst)(i, j) = 255 - (*src)(i, j);
        return dst;
    }

    template <class T>
    /* _dir: 0 - x, 1 - y */
    Image<T>* mirror(Image<T>* src, const char* dir)
    {
        const size_t _width = src->width();
        const size_t _height = src->height();
        const int _dir = !strcmp(dir, "x") ? 0 : 1;
        Image<T>* dst = new Image<T>(_width, _height);
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
                (*dst)(i, j) = (*src)(_dir ? i : _height - 1 - i, _dir ? _width - 1 - j : j);
        return dst;
    }
    
    template <class T>
    Image<T>* rotate(Image<T>* src, const char* dir, int angle)
    {
        const size_t _width = src->width();
        const size_t _height = src->height();
        Image<T>* dst = NULL;
        if (!(angle % 90))
        {
            if ((!strcmp(dir, "cw") && angle / 90 % 4 == 1) || (!strcmp(dir, "ccw") && angle / 90 % 4 == 3))
            {
                dst = new Image<T>(_height, _width);
                if (!dst)
                    throw "Lab1::rotate: Insufficient memory to allocate output image";
                for (int i = 0; i < _height; i++)
                    for (int j = 0; j < _width; j++)
                        (*dst)(j, _height - i - 1) = (*src)(i, j);
            }
            if ((!strcmp(dir, "cw") && angle / 90 % 4 == 3) || (!strcmp(dir, "ccw") && angle / 90 % 3 == 1))
            {
                dst = new Image<T>(_height, _width);
                if (!dst)
                    throw "Lab1::rotate: Insufficient memory to allocate output image";
                for (int i = 0; i < _height; i++)
                    for (int j = 0; j < _width; j++)
                        (*dst)(_width - j - 1, i) = (*src)(i, j);
            }
            if (angle / 90 % 4 == 2)
            {
                dst = new Image<T>(_width, _height);
                if (!dst)
                    throw "Lab1::rotate: Insufficient memory to allocate output image";
                for (int i = 0; i < _height; i++)
                    for (int j = 0; j < _width; j++)
                        (*dst)(_height - 1 - i, _width - 1 - j) = (*src)(i, j);
            }
            if (!(angle / 90 % 4))
            {
                dst = new Image<T>(_width, _height);
                if (!dst)
                    throw "Lab1::rotate: Insufficient memory to allocate output image";
                for (int i = 0; i < _height; i++)
                    for (int j = 0; j < _width; j++)
                        (*dst)(i, j) = (*src)(i, j);
            }
        }
        /* rotation on random angle */
        else
        {
            const float rad_angle = (float)(((!strcmp(dir, "ccw") ? angle : 90 - angle) % 90) * atan(1) / 45);
            const int new_height = (int)round(sqrt(pow(_width, 2) + pow(_height, 2)) *
                    sin(atan((float)_height / (float)_width) + rad_angle));
            const int new_width = (int)round(sqrt(pow(_width, 2) + pow(_height, 2)) *
                    sin(atan((float)_width / (float)_height) + rad_angle));
            const int dx = (new_width - _width) / 2;
            const int dy = (new_height - _height) / 2;
            Image<T>* med = new Image<T>(new_width, new_height);
            if (!med)
                throw "Lab1::rotate: Insufficient memory to allocate mediocre image";
            for (int i = 0; i < new_height; i++)
                for (int j = 0; j < new_width; j++)
                {
                    float len = (float)sqrt(pow(new_width / 2 - j, 2) + pow(new_height / 2 - i, 2));
                    float turn_angle =
                        (float)(len ? (i < new_height / 2 ? acos((new_width / 2 - j) / len) : -acos((new_width / 2 - j) / len)) : 0);
                    float src_x = new_width / 2 - cos(turn_angle + rad_angle) * len - dx;
                    float src_y = new_height / 2 - sin(turn_angle + rad_angle) * len - dy;
                    int new_x = (int)floor(src_x);
                    int new_y = (int)floor(src_y);
                    (*med)(i, j) = (new_x >= 0 && new_x < _width && new_y >= 0 && new_y < _height) ?
                        (*src)(new_y, new_x) * (new_x + 1 - src_x) * (new_y + 1 - src_y) +
                        (*src)(new_y, new_x + 1) * (src_x - new_x) * (new_y + 1 - src_y) +
                        (*src)(new_y + 1, new_x) * (new_x + 1 - src_x) * (src_y - new_y) +
                        (*src)(new_y + 1, new_x + 1) * (src_x - new_x) * (src_y - new_y)
                        : 0;
                }
            dst = rotate(med, dir, !strcmp(dir, "ccw") ? (angle / 90) * 90 : (angle / 90 + 1) * 90);
            delete med;
        }
        return dst;
    }

    template <class T>
    Image<T>* prewitt(Image<T>* src, const char* dir)
    {
        const size_t _width = src->width();
        const size_t _height = src->height();
        Image<T>* dst = new Image<T>(_width, _height);
        if (!dst)
            throw "Lab1::prewitt: Insufficient memory to allocate output image";
        if (!strcmp(dir, "x"))
            for (int i = 0; i < _height; i++)
                for (int j = 0; j < _width; j++)
                    (*dst)(i, j) = (*src)(i + 1, j - 1) + (*src)(i + 1, j) + (*src)(i + 1, j + 1) -
                        (*src)(i - 1, j - 1) - (*src)(i - 1, j) - (*src)(i - 1, j + 1) + 128;
        if (!strcmp(dir, "y"))
            for (int i = 0; i < _height; i++)
                for (int j = 0; j < _width; j++)
                    (*dst)(i, j) = (*src)(i - 1, j - 1) + (*src)(i, j - 1) + (*src)(i + 1, j - 1) -
                        (*src)(i - 1, j + 1) - (*src)(i, j + 1) - (*src)(i + 1, j + 1) + 128;
        return dst;
    }

    template <class T>
    Image<T>* sobel(Image<T>* src, const char* dir)
    {
        const size_t _width = src->width();
        const size_t _height = src->height();
        Image<T>* dst = new Image<T>(_width, _height);
        if (!dst)
            throw "Lab1::sobel: Insufficient memory to allocate output image";
        if (!strcmp(dir, "x"))
            for (int i = 0; i < _height; i++)
                for (int j = 0; j < _width; j++)
                    (*dst)(i, j) = (*src)(i + 1, j - 1) + 2*(*src)(i + 1, j) + (*src)(i + 1, j + 1) -
                        (*src)(i - 1, j - 1) - 2*(*src)(i - 1, j) - (*src)(i - 1, j + 1) + 128;
        if (!strcmp(dir, "y"))
            for (int i = 0; i < _height; i++)
                for (int j = 0; j < _width; j++)
                    (*dst)(i, j) = (*src)(i - 1, j - 1) + 2*(*src)(i, j - 1) + (*src)(i + 1, j - 1) -
                        (*src)(i - 1, j + 1) - 2*(*src)(i, j + 1) - (*src)(i + 1, j + 1) + 128;
        return dst;
    }
    
    template <class T>
    /* dir: 1 - main, 2 - sub */
    Image<T>* roberts(Image<T>* src, int dir)
    {
        const size_t _width = src->width();
        const size_t _height = src->height();
        Image<T>* dst = new Image<T>(_width, _height);
        if (!dst)
            throw "Lab1::roberts: Insufficient memory to allocate output image";
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
                (*dst)(i, j) = (dir - 1 ? (*src)(i + 1, j + 1) - (*src)(i, j) : (*src)(i + 1, j) - (*src)(i, j + 1)) + 128;
        return dst;
    }
    
    template <class T>
    Image<T>* median(Image<T>* src, int r)
    {
        const size_t _width = src->width();
        const size_t _height = src->height();
        Image<T>* dst = new Image<T>(_width, _height);
        if (!dst)
            throw "Lab1::median: Insufficient memory to allocate output image";
        T* array = new T[(2*r + 1) * (2*r + 1)];
        if (!array)
            throw "Lab1::median: Insufficient memory to allocate buffer";
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
            {
                int counter = 0;
                for (int k = i - r; k <= i + r; k++)
                    for (int n = j - r; n <= j + r; n++)
                        array[counter++] = (*src)(k, n);
                std::sort(array, array + (2*r + 1) * (2*r + 1));
                (*dst)(i, j) = array[2*r*(r + 1) + 1];
            }
        delete[] array;
        return dst;
    }
    
    template <class T>
    Image<T>* gauss(Image<T>* src, float sigma)
    {
        const size_t _width = src->width();
        const size_t _height = src->height();
        Image<T>* dst = new Image<T>(_width, _height);
        if (!dst)
            throw "Lab1::gauss: Insufficient memory to allocate output image";
        Image<T>* med = new Image<T>(_width, _height);
        if (!med)
            throw "Lab1::gauss: Insufficient memory to allocate mediocre image";
        int r = (int)(3*sigma);
        T* kernel = new T[2*r + 1];
        if (!kernel)
            throw "Lab1::gauss: Insufficient memory to allocate kernel";
        T norm = 0;
        for (int i = 0; i < 2*r + 1; i++)
        {
            kernel[i] = (T)exp(-pow(r - i, 2) / (2 * pow(sigma, 2)));
            norm += kernel[i];
        }
        for (int i = 0; i < 2*r + 1; i++)
            kernel[i] /= norm;
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
                for (int n = j - r, k = 0; n <= j + r; n++, k++)
                    (*med)(i, j) += kernel[k] * (*src)(i, n);
        for (int j = 0; j < _width; j++)
            for (int i = 0; i < _height; i++)
                for (int n = i - r, k = 0; n <= i + r; n++, k++)
                    (*dst)(i, j) += kernel[k] * (*med)(n, j);
        delete[] kernel;
        delete med;
        return dst;
    }

    template <class T>
    Image<T>* gradient(Image<T>* src, float sigma)
    {
        const size_t _width = src->width();
        const size_t _height = src->height();
        Image<T>* dst = new Image<T>(_width, _height);
        if (!dst)
            throw "Lab1::gradient: Insufficient memory to allocate output image";
        Image<T>* x = new Image<T>(_width, _height);
        Image<T>* y = new Image<T>(_width, _height);
        if (!x || !y)
            throw "Lab1::gradient: Insufficient memory to allocate mediocre image";
        int r = (int)(3*sigma);
        T* kernel = new T[2*r + 1];
        if (!kernel)
            throw "Lab1::gradient: Insufficient memory to allocate kernel";
        T norm = 0;
        for (int i = 0; i < 2*r + 1; i++)
        {
            kernel[i] = (T)((r - i) / sigma * exp(-pow(r - i, 2) / (2 * pow(sigma, 2))));
            norm += fabs(kernel[i]);
        }
        for (int i = 0; i < 2*r + 1; i++)
            kernel[i] /= norm;
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
                for (int n = -r, k = 0; n <= r; n++, k++)
                {
                    (*x)(i, j) += kernel[k] * (*src)(i, j + n);
                    (*y)(i ,j) += kernel[k] * (*src)(i + n, j);
                }
        delete[] kernel;
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
                (*dst)(i, j) = (float)sqrt(pow((*x)(i, j), 2) + pow((*y)(i, j), 2));
        delete x;
        delete y;
        return dst;
    }

    template <class T>
    Image<T>* gabor(Image<T>* src, float sigma, float gamma, float theta, float lambda, float psi)
    {
        const size_t _width = src->width();
        const size_t _height = src->height();
        Image<T>* dst = new Image<T>(_width, _height);
        if (!dst)
            throw "Lab1::gabor: Insufficient memory to allocate output image";
        int r = (int)(3 * std::max(sigma, sigma / gamma));
        const float _theta = theta / 45 * atan(1);
        const float _psi = psi / 45 * atan(1);
        T* kernel = new T[(2*r + 1) * (2*r + 1)];
        if (!kernel)
            throw "Lab1::gabor: Insufficient memory to allocate kernel";
        for (int i = 0; i < 2*r + 1; i++)
            for (int j = 0; j < 2*r + 1; j++)
            {
                float x = (r - i) * sin(_theta) + (r - j) * cos(_theta);
                float y = (r - i) * cos(_theta) - (r - j) * sin(_theta);
                kernel[i * (2*r + 1) + j] =
                    (T)(exp(-(pow(x, 2) + pow(gamma * y, 2)) / (2 * pow(sigma, 2))) * cos(8 * atan(1) / lambda * x + _psi));
            }
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
                for (int y = i - r, n = 0; y <= i + r; y++, n++)
                    for (int x = j - r, k = 0; x <= j + r; x++, k++)
                        (*dst)(i, j) += kernel[n * (2*r + 1) + k] * (*src)(y, x);
        delete[] kernel;
        return dst;
    }

    template <class T>
    Image<T>* vessels(Image<T>* src, float sigma)
    {
        const size_t _width = src->width();
        const size_t _height = src->height();
        const float _lambda = 2 * sigma;
        const float _gamma = 1;
        float _theta = 0;
        Image<T>* dst = gabor(src, sigma, _gamma, _theta, _lambda, 0);
        for (_theta += 10; _theta < 180; _theta += 10)
        {
            Image<T>* cur = gabor(src, sigma, _gamma, _theta, _lambda, 0);
            for (int i = 0; i < _height; i++)
                for (int j = 0; j < _width; j++)
                    (*dst)(i, j) = std::min((*dst)(i, j), (*cur)(i, j));
            delete cur;
        }
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
                (*dst)(i, j) = (*dst)(i, j) < 0 ? fabs((*dst)(i, j)) : 0;
        return dst;
    }

} /* namespace Lab1 */

#endif /* _LAB1_H_ */
