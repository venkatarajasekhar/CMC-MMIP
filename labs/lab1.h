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
            Image<T>* dst = new Image<T>(src->width(), src->height());
            for (int i = 0; i < src->height(); i++)
                for (int j = 0; j < src->width(); j++)
                    (*dst)(i, j) = 255 - (*src)(i, j);
            return dst;
        }
        
    template <class T>
        /* _dir: 0 - x, 1 - y */
        Image<T>* mirror(Image<T>* src, const char* dir)
        {
            int _dir = !strcmp(dir, "x") && strcmp(dir, "y") ? 0 : 1;
            Image<T>* dst = new Image<T>(src->width(), src->height());
            for (int i = 0; i < src->height(); i++)
                for (int j = 0; j < src->width(); j++)
                    (*dst)(i, j) = (*src)(_dir ? i : src->height() - 1 - i, _dir ? src->width() - 1 - j : j);
            return dst;
        }
    
    template <class T>
        Image<T>* rotate(Image<T>* src, const char* dir, int angle)
        {
            Image<T>* dst = NULL;
            if (!(angle % 90))
            {
                if ((!strcmp(dir, "cw") && angle / 90 % 4 == 1) || (!strcmp(dir, "ccw") && angle / 90 % 4 == 3))
                {
                    dst = new Image<T>(src->height(), src->width());
                    for (int i = 0; i < src->height(); i++)
                        for (int j = 0; j < src->width(); j++)
                            (*dst)(j, src->height() - i - 1) = (*src)(i, j);
                }
                if ((!strcmp(dir, "cw") && angle / 90 % 4 == 3) || (!strcmp(dir, "ccw") && angle / 90 % 3 == 1))
                {
                    dst = new Image<T>(src->height(), src->width());
                    for (int i = 0; i < src->height(); i++)
                        for (int j = 0; j < src->width(); j++)
                            (*dst)(src->width() - j - 1, i) = (*src)(i, j);
                }
                if (angle / 90 % 4 == 2)
                {
                    dst = new Image<T>(src->width(), src->height());
                    for (int i = 0; i < src->height(); i++)
                        for (int j = 0; j < src->width(); j++)
                            (*dst)(i, j) = (*src)(src->height() - 1 - i, src->width() - 1 - j);
                }
                if (!(angle / 90 % 4))
                {
                    dst = new Image<T>(src->width(), src->height());
                    for (int i = 0; i < src->height(); i++)
                        for (int j = 0; j < src->width(); j++)
                            (*dst)(i, j) = (*src)(i, j);
                }
            }
            /* rotation on random angle */
            else
            {
                float rad_angle = (float)(((!strcmp(dir, "ccw") ? angle : 90 - angle) % 90) * atan(1) / 45);
                int new_height = (int)round(sqrt(pow(src->width(), 2) + pow(src->height(), 2)) *
                        sin(atan((float)src->height() / (float)src->width()) + rad_angle));
                int new_width = (int)round(sqrt(pow(src->width(), 2) + pow(src->height(), 2)) *
                        sin(atan((float)src->width() / (float)src->height()) + rad_angle));
                Image<T>* med = new Image<T>(new_width, new_height);
                int dx = (new_width - src->width()) / 2;
                int dy = (new_height - src->height()) / 2;
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
                        (*med)(i, j) = (new_x >= 0 && new_x < src->width() && new_y >= 0 && new_y < src->height()) ?
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
            Image<T>* dst = new Image<T>(src->width(), src->height());
            if (!strcmp(dir, "x"))
                for (int i = 0; i < src->height(); i++)
                    for (int j = 0; j < src->width(); j++)
                        (*dst)(i, j) = (*src)(i + 1, j - 1) + (*src)(i + 1, j) + (*src)(i + 1, j + 1) -
                            (*src)(i - 1, j - 1) - (*src)(i - 1, j) - (*src)(i - 1, j + 1) + 128;
            if (!strcmp(dir, "y"))
                for (int i = 0; i < src->height(); i++)
                    for (int j = 0; j < src->width(); j++)
                        (*dst)(i, j) = (*src)(i - 1, j - 1) + (*src)(i, j - 1) + (*src)(i + 1, j - 1) -
                            (*src)(i - 1, j + 1) - (*src)(i, j + 1) - (*src)(i + 1, j + 1) + 128;
            return dst;
        }

        
    template <class T>
        Image<T>* sobel(Image<T>* src, const char* dir)
        {
            Image<T>* dst = new Image<T>(src->width(), src->height());
            if (!strcmp(dir, "x"))
                for (int i = 0; i < src->height(); i++)
                    for (int j = 0; j < src->width(); j++)
                        (*dst)(i, j) = (*src)(i + 1, j - 1) + 2*(*src)(i + 1, j) + (*src)(i + 1, j + 1) -
                            (*src)(i - 1, j - 1) - 2*(*src)(i - 1, j) - (*src)(i - 1, j + 1) + 128;
            if (!strcmp(dir, "y"))
                for (int i = 0; i < src->height(); i++)
                    for (int j = 0; j < src->width(); j++)
                        (*dst)(i, j) = (*src)(i - 1, j - 1) + 2*(*src)(i, j - 1) + (*src)(i + 1, j - 1) -
                            (*src)(i - 1, j + 1) - 2*(*src)(i, j + 1) - (*src)(i + 1, j + 1) + 128;
            return dst;
        }
        
    template <class T>
        /* dir: 1 - main, 2 - sub */
        Image<T>* roberts(Image<T>* src, int dir)
        {
            Image<T>* dst = new Image<T>(src->width(), src->height());
            for (int i = 0; i < src->height(); i++)
                for (int j = 0; j < src->width(); j++)
                    (*dst)(i, j) = (dir - 1 ? (*src)(i + 1, j + 1) - (*src)(i, j) : (*src)(i + 1, j) - (*src)(i, j + 1)) + 128;
            return dst;
        }
    
    template <class T>
        Image<T>* median(Image<T>* src, int r)
        {
            Image<T>* dst = new Image<T>(src->width(), src->height());
            T array[(2*r + 1)*(2*r + 1)];
            for (int i = 0; i < src->height(); i++)
                for (int j = 0; j < src->width(); j++)
                {
                    int counter = 0;
                    for (int k = i - r; k <= i + r; k++)
                        for (int n = j - r; n <= j + r; n++)
                            array[counter++] = (*src)(k, n);
                    std::sort(array, array + (2*r + 1) * (2*r + 1));
                    (*dst)(i, j) = array[2*r*(r + 1) + 1];
                }
            return dst;
        }
    
    template <class T>
        Image<T>* gauss(Image<T>* src, float sigma)
        {
            Image<T>* dst = new Image<T>(src->width(), src->height());
            Image<T>* med = new Image<T>(src->width(), src->height());
            int r = (int)(3*sigma);
            T kernel[2*r + 1];
            T norm = 0;
            for (int i = 0; i < 2*r + 1; i++)
            {
                kernel[i] = (T)exp(-pow(r - i, 2) / (2 * pow(sigma, 2)));
                norm += kernel[i];
            }
            for (int i = 0; i < 2*r + 1; i++)
                kernel[i] /= norm;
            for (int i = 0; i < src->height(); i++)
                for (int j = 0; j < src->width(); j++)
                    for (int n = j - r, k = 0; n <= j + r; n++, k++)
                        (*med)(i, j) += kernel[k] * (*src)(i, n);
            for (int j = 0; j < src->width(); j++)
                for (int i = 0; i < src->height(); i++)
                    for (int n = i - r, k = 0; n <= i + r; n++, k++)
                        (*dst)(i, j) += kernel[k] * (*med)(n, j);
            delete med;
            return dst;
        }
    
    template <class T>
        Image<T>* gradient(Image<T>* src, float sigma)
        {
            Image<T>* dst = new Image<T>(src->width(), src->height());
            Image<T>* x = new Image<T>(src->width(), src->height());
            Image<T>* y = new Image<T>(src->width(), src->height());
            int r = (int)(3*sigma);
            T kernel[2*r + 1];
            T norm = 0;
            for (int i = 0; i < 2*r + 1; i++)
            {
                kernel[i] = (T)((r - i) / sigma * exp(-pow(r - i, 2) / (2 * pow(sigma, 2))));
                norm += fabs(kernel[i]);
            }
            for (int i = 0; i < 2*r + 1; i++)
                kernel[i] /= norm;
            for (int i = 0; i < src->height(); i++)
                for (int j = 0; j < src->width(); j++)
                    for (int n = j - r, k = 0; n <= j + r; n++, k++)
                        (*x)(i, j) += kernel[k] * (*src)(i, n);
            for (int j = 0; j < src->width(); j++)
                for (int i = 0; i < src->height(); i++)
                    for (int n = i - r, k = 0; n <= i + r; n++, k++)
                        (*y)(i ,j) += kernel[k] * (*src)(n, j);
            for (int i = 0; i < src->height(); i++)
                for (int j = 0; j < src->width(); j++)
                    (*dst)(i, j) = (float)sqrt(pow((*x)(i, j), 2) + pow((*y)(i, j), 2));
            delete x;
            delete y;
            return dst;
        }

    template <class T>
        Image<T>* gabor(Image<T>* src, float sigma, float gamma, float theta, float lambda, float psi)
        {
            Image<T>* dst = new Image<T>(src->width(), src->height());
            int r = (int)(3*sigma);
            const float _theta = theta / 45 * atan(1);
            const float _psi = theta / 45 * atan(1);
            T kernel[2*r + 1][2*r + 1];
            for (int i = 0; i < 2*r + 1; i++)
                for (int j = 0; j < 2*r + 1; j++)
                {
                    float x = i * sin(_theta) + j * cos(_theta);
                    float y = i * cos(_theta) - j * sin(_theta);
                    kernel[i][j] = (T)(exp(-(pow(x, 2) + pow(gamma * y, 2)) / (2 * pow(sigma, 2))) * cos(8 * atan(1) / lambda * x + _psi));
                }
            for (int i = 0; i < src->height(); i++)
                for (int j = 0; j < src->width(); j++)
                    for (int _i = i - r, n = 0; _i <= i + r; _i++, n++)
                        for (int _j = j - r, k = 0; _j <= j + r; _j++, k++)
                            (*dst)(i, j) += kernel[n][k] * (*src)(_i, _j);
            return dst;
        }
}

#endif //_LAB1_H_
