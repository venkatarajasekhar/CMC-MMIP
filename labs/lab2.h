#ifndef _LAB2_H_
#define _LAB2_H_

#include "image.h"
#include "lab1.h"

namespace Lab2
{
    template <class T>
    Image<T>* up_bilinear(Image<T>* src, float scale)
    {
        const int _width = src->width();
        const int _height = src->height();
        const int new_width = (int)(_width * scale);
        const int new_height = (int)(_height * scale);
        Image<T>* dst = new Image<T>(new_width, new_height);
        if (!dst)
            throw "Lab2::up_bilinear: Insufficient memory to allocate output image";
        for (int i = 0; i < new_height; i++)
        {
            float orig_y = (float)(i) / new_height * _height;
            int src_y = (int)floor(orig_y);
            for (int j = 0; j < new_width; j++)
            {
                float orig_x = (float)(j) / new_width * _width;
                int src_x = (int)floor(orig_x);
                (*dst)(i, j) = 
                    (*src)(src_y, src_x) * (src_x + 1 - orig_x) * (src_y + 1 - orig_y) +
                    (*src)(src_y, src_x + 1) * (orig_x - src_x) * (src_y + 1 - orig_y) +
                    (*src)(src_y + 1, src_x) * (src_x + 1 - orig_x) * (orig_y - src_y) +
                    (*src)(src_y + 1, src_x + 1) * (orig_x - src_x) * (orig_y - src_y);
            }
        }
        return dst;
    }

    template <class T>
    Image<T>* downsample(Image<T>* src, float scale)
    {
        const int _width = src->width();
        const int _height = src->height();
        const int new_width = (int)(_width / scale);
        const int new_height = (int)(_height / scale);
        Image<T>* dst = new Image<T>(new_width, new_height);
        if (!dst)
            throw "Lab2::downsample: Insufficient memory to allocate output image";
        Image<T>* med = Lab1::gauss(src, sqrt(scale*scale - 1));
        for (int i = 0, y = 0; i < new_height; i++, y += (int)(scale))
            for (int j = 0, x = 0; j < new_width; j++, x += (int)(scale))
                (*dst)(i, j) = (*med)(y, x);
        delete med;
        return dst;
    }

    template <class T>
    float mse_metric(Image<T>* orig, Image<T>* src)
    {
        float _result = 0.0f;
        const size_t _width = orig->width();
        const size_t _height = orig->height();
        if (_width != src->width() || _height != src->height())
            throw "Lab2::mse_metric: Image's dimensions are not equal";
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
                _result += pow((*orig)(i, j) - (*src)(i, j), 2);
        return _result / (_width * _height);
    }

    template <class T>
    float psnr_metric(Image<T>* orig, Image<T>* src)
    {
        const size_t _width = orig->width();
        const size_t _height = orig->height();
        if (_width != src->width() || _height != src->height())
            throw "Lab2::psnr_metric: Image's dimensions are not equal";
        return (float)(10 * log10(255 * 255 / mse_metric(orig, src)));
    }

    template <class T>
    float ssim_metric(Image<T>* orig, Image<T>* src)
    {
        float _ev_x = 0.0f, _ev_y = 0.0f;
        float _var_x = 0.0f, _var_y = 0.0f;
        float _cov = 0.0f;
        const float _c1 = (float)(0.03 * 255);
        const float _c2 = (float)(0.01 * 255);
        const size_t _width = orig->width();
        const size_t _height = orig->height();
        if (_width != src->width() || _height != src->height())
            throw "Lab2::ssim_metric: Image's dimensions are not equal";
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
            {
                _ev_x += (*orig)(i, j);
                _ev_y += (*src)(i, j);
            }
        _ev_x /= _width * _height;
        _ev_y /= _width * _height;
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
            {
                _var_x += pow((*orig)(i, j) - _ev_x, 2);
                _var_y += pow((*src)(i, j) - _ev_y, 2);
                _cov += ((*orig)(i, j) - _ev_x) * ((*src)(i, j) - _ev_y);
            }
        _var_x /= _width * _height - 1;
        _var_y /= _width * _height - 1;
        _cov /= _width * _height - 1;
        return (float)(((2 * _ev_x * _ev_y + _c1) * (2 * _cov + _c2)) /
                ((pow(_ev_x, 2) + pow(_ev_y, 2) + _c1) * (_var_x + _var_y + _c2)));
    }

} /* namespace Lab2 */

#endif /* _LAB2_H_ */
