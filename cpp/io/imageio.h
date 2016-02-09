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

#ifndef _IMAGEIO_H_
#define _IMAGEIO_H_

#include <stdint.h>
#include "image.h"
#include "bmpio.h"

namespace ImageIO
{
    template<class T>
    static Image<T> _bmp_indexed_read(BmpRead& bmp_input)
    {
        const int _width = bmp_input.width();
        const int _height = bmp_input.height();
        Image<T> _input(_width, _height);
        uint8_t* _row = nullptr;
        uint8_t _rgb[3];
        try
        {
            _row = new uint8_t[_width];
            for (int i = _height - 1; i >= 0; i--)
            {
                bmp_input.read(_row, i, 0, _width);
                for (int j = 0; j < _width; j++)
                {
                    bmp_input.Paleterize(_rgb, _row[j]);
                    _input(i, j) = 0.114 * _rgb[0] + 0.5871 * _rgb[1] + 0.2989 * _rgb[2];
                }
            }
            delete[] _row;
            return _input;
        }
        catch (const char* ex)
        {
            if (_row)
               delete[] _row;
            throw;
        }
    }

    template<class T>
    static Image<T> _bmp_rgb_read(BmpRead& bmp_input)
    {
        const int _width = bmp_input.width();
        const int _height = bmp_input.height();
        Image<T> _input(_width, _height);
        uint8_t* _row = nullptr;
        try
        {
            _row = new uint8_t[_width * 3];
            for (int i = _height - 1; i >= 0; i--)
            {
                bmp_input.read(_row, i, 0, _width * 3);
                for (int j = 0; j < _width; j++)
                    _input(i, j) = 0.114 * _row[3*j] + 0.5871 * _row[3*j + 1] + 0.2989 * _row[3*j + 2];
            }
            delete[] _row;
            return _input;
        }
        catch (const char* ex)
        {
            if (_row)
                delete[] _row;
            throw;
        }
    }

    template<class T>
    static Image<T> _bmp_rgbx_read(BmpRead& bmp_input)
    {
        const int _width = bmp_input.width();
        const int _height = bmp_input.height();
        Image<T> _input(_width, _height);
        uint8_t* _row = nullptr;
        try
        {
            _row = new uint8_t[_width * 4];
            for (int i = _height - 1; i >= 0; i--)
            {
                bmp_input.read(_row, i, 0, _width * 4);
                for (int j = 0; j < _width; j++)
                    _input(i, j) = 0.114 * _row[4*j] + 0.5871 * _row[4*j + 1] + 0.2989 * _row[4*j + 2];
            }
            delete[] _row;
            return _input;
        }
        catch (const char* ex)
        {
            if (_row)
                delete[] _row;
            throw;
        }
    }

    template<class T>
    Image<T> read(const char* filename)
    {
        BmpRead _bmp_input(filename);
        switch (_bmp_input.type())
        {
            case BmpRead::BT_INDEX:
                return _bmp_indexed_read<T>(_bmp_input);
            case BmpRead::BT_RGB:
                return _bmp_rgb_read<T>(_bmp_input);
            case BmpRead::BT_RGBX:
                return _bmp_rgbx_read<T>(_bmp_input);
            default:
                throw "ImageIO::read: Not supported type of pixel format";
        };
    }

    template<class T>
    int write(const char* filename, Image<T>&& output)
    {
        const int _width = output.width();
        const int _height = output.height();
        uint8_t* _row = nullptr;
        try
        {
            BmpWrite _bmp_output(filename, _width, _height, BmpWrite::BT_INDEX);
            _row = new uint8_t[_width];
            for (int i = _height - 1; i >= 0; i--)
            {
                for (int j = 0; j < _width; j++)
                {
                    T value = output(i, j);
                    _row[j] = (uint8_t)(value < 0 ? Image<T>::BLACK : (value > 255 ? Image<T>::WHITE : value));
                }
                _bmp_output.write(_row, i, 0, _width);
            }
            delete[] _row;
            return 0;
        }
        catch (const char* ex)
        {
            if (_row)
                delete[] _row;
            throw;
        }
    }

} /* namespace ImageIO */

#endif /* IMAGEIO_H_ */
