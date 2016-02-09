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

#ifndef _METRIC_H_
#define _METRIC_H_

#include "image.h"

namespace Metric
{
    template <class T>
    float mse_metric(Image<T>& orig, Image<T>& src)
    {
        float _result = 0.0f;
        const size_t _width = orig.width();
        const size_t _height = orig.height();
        if (_width != src.width() || _height != src.height())
            throw "Metric::mse_metric: Image's dimensions are not equal";
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
                _result += pow(orig(i, j) - src(i, j), 2);
        return _result / (_width * _height);
    }

    template <class T>
    float psnr_metric(Image<T>& orig, Image<T>& src)
    {
        const size_t _width = orig.width();
        const size_t _height = orig.height();
        if (_width != src.width() || _height != src.height())
            throw "Metric::psnr_metric: Image's dimensions are not equal";
        return (float)(10 * log10(255 * 255 / mse_metric(orig, src)));
    }

    template <class T>
    float ssim_metric(Image<T>& orig, Image<T>& src)
    {
        float _ev_x = 0.0f, _ev_y = 0.0f;
        float _var_x = 0.0f, _var_y = 0.0f;
        float _cov = 0.0f;
        const float _c1 = (float)(0.03 * 255);
        const float _c2 = (float)(0.01 * 255);
        const size_t _width = orig.width();
        const size_t _height = orig.height();
        if (_width != src.width() || _height != src.height())
            throw "Metric::ssim_metric: Image's dimensions are not equal";
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
            {
                _ev_x += orig(i, j);
                _ev_y += src(i, j);
            }
        _ev_x /= _width * _height;
        _ev_y /= _width * _height;
        for (int i = 0; i < _height; i++)
            for (int j = 0; j < _width; j++)
            {
                _var_x += pow(orig(i, j) - _ev_x, 2);
                _var_y += pow(src(i, j) - _ev_y, 2);
                _cov += (orig(i, j) - _ev_x) * (src(i, j) - _ev_y);
            }
        _var_x /= _width * _height - 1;
        _var_y /= _width * _height - 1;
        _cov /= _width * _height - 1;
        return (float)(((2 * _ev_x * _ev_y + _c1) * (2 * _cov + _c2)) /
                ((pow(_ev_x, 2) + pow(_ev_y, 2) + _c1) * (_var_x + _var_y + _c2)));
    }

    template <class T>
    float mssim_metric(Image<T>& orig, Image<T>& src)
    {
        float _result = 0.0f;
        const size_t _width = orig.width();
        const size_t _height = orig.height();
        if (_width != src.width() || _height != src.height())
            throw "Metric::mssim_metric: Image's dimensions are not equal";
        const int _blocks_per_x = _width / 8;
        const int _blocks_per_y = _height / 8;
        GrayscaleFloatImage _block_orig(8, 8);
        GrayscaleFloatImage _block_src(8, 8);
        for (int i = 0; i < _blocks_per_y; i++)
            for (int j = 0; j < _blocks_per_x; j++)
            {
                for (int y = 0; y < 8; y++)
                    for (int x = 0; x < 8; x++)
                    {
                        _block_orig(i, j) = orig(i * 8 + y, j * 8 + x);
                        _block_src(i, j) = src(i * 8 + y, j * 8 + x);
                    }
                _result += ssim_metric(_block_orig, _block_src);
            }
        return _result / (_blocks_per_x * _blocks_per_y);
    }
} /* namespace Metric */

#endif /* _METRIC_H_ */
