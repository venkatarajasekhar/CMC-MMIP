#ifndef _IMAGEIO_H_
#define _IMAGEIO_H_

#include "image.h"
#include "bmpio.h"

namespace ImageIO
{
    template<class T>
    static Image<T>* _bmp_indexed_read(BmpRead& bmp_input)
    {
        const int _width = bmp_input.width();
        const int _height = bmp_input.height();
        Image<T>* _input = new Image<T>(_width, _height);
        unsigned char* _data = NULL;
        unsigned char* _rgb = NULL;
        try
        {
            _data = new unsigned char[_width];
            if (!_data)
                throw "ImageIO::read(indexed): Insufficient memory to allocate data buffer";
            _rgb = new unsigned char[3];
            if (!_rgb)
                throw "ImageIO::read(indexed): Insufficient memory to allocate palette buffer";
            for (int i = _height - 1; i >= 0; i--)
            {
                bmp_input.read(_data, i, 0, _width);
                for (int j = 0; j < _width; j++)
                {
                    bmp_input.Paleterize(_rgb, _data[j]);
                    (*_input)(i, j) = 0.1140 * _rgb[0] + 0.5871 * _rgb[1] + 0.2989 * _rgb[2];
                }
            }
            delete[] _data;
            delete[] _rgb;
            return _input;
        }
        catch (const char* ex)
        {
            if (_data)
               delete[] _data;
            if (_rgb)
                delete[] _rgb;
            throw;
        }
    }

    template<class T>
    static Image<T>* _bmp_rgb_read(BmpRead& bmp_input)
    {
        const int _width = bmp_input.width();
        const int _height = bmp_input.height();
        Image<T>* _input = new Image<T>(_width, _height);
        unsigned char* _data = NULL;
        try
        {
            _data = new unsigned char[_width * 3];
            if (!_data)
                throw "ImageIO::read(rgb): Insufficient memory to allocate data buffer";
            for (int i = _height - 1; i >= 0; i--)
            {
                bmp_input.read(_data, i, 0, _width * 3);
                for (int j = 0; j < _width; j++)
                    (*_input)(i, j) = 0.1140 * _data[3*j] + 0.5871 * _data[3*j + 1] + 0.2989 * _data[3*j + 2];
            }
            delete[] _data;
            return _input;
        }
        catch (const char* ex)
        {
            if (_data)
                delete[] _data;
            throw;
        }
    }

    template<class T>
    static Image<T>* _bmp_rgbx_read(BmpRead& bmp_input)
    {
        const int _width = bmp_input.width();
        const int _height = bmp_input.height();
        Image<T>* _input = new Image<T>(_width, _height);
        unsigned char* _data = NULL;
        try
        {
            _data = new unsigned char[_width * 4];
            if (!_data)
                throw "ImageIO::read(rgbx): Insufficient memory to allocate data buffer";
            for (int i = _height - 1; i >= 0; i--)
            {
                bmp_input.read(_data, i, 0, _width * 4);
                for (int j = 0; j < _width; j++)
                (*_input)(i, j) = 0.1140 * _data[4*j] + 0.5871 * _data[4*j + 1] + 0.2989 * _data[4*j + 2];
            }
            delete[] _data;
            return _input;
        }
        catch (const char* ex)
        {
            if (_data)
                delete[] _data;
            throw;
        }
    }

    template<class T>
    Image<T>* read(const char* filename)
    {
        try
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
        catch (const char* ex)
        {
            fprintf(stderr, "Image %s could not be loaded because of exception:\n%s\n", filename, ex);
            return NULL;
        }
    }

    template<class T>
    int write(const char* filename, Image<T>* output)
    {
        const int _width = output->width();
        const int _height = output->height();
        unsigned char* _data = NULL;
        try
        {
            BmpWrite _bmp_output(filename, _width, _height, BmpWrite::BT_INDEX);
            _data = new unsigned char[_width];
            if (!_data)
                throw "ImageIO::write: Insufficient memory to allocate data buffer";
            for (int i = _height - 1; i >= 0; i--)
            {
                for (int j = 0; j < _width; j++)
                {
                    T value = (*output)(i, j);
                    _data[j] = (unsigned char)(value < 0 ? 0 : (value > 255 ? 255 : value));
                }
                _bmp_output.write(_data, i, 0, _width);
            }
            delete[] _data;
            return 0;
        }
        catch (const char* ex)
        {
            if (_data)
                delete[] _data;
            fprintf(stderr, "Image %s could not be saved because of exception:\n%s\n", filename, ex);
            return -1;
        }
    }

} /* namespace ImageIO */
#endif /* IMAGEIO_H_ */
