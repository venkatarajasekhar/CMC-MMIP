#include "image.h"
#include "bmpio.h"

namespace ImageIO
{
    template<class T>
        Image<T>* read(const char* filename)
        {
            try
            {
                BmpRead _bmp_input(filename);
                const int _width = _bmp_input.width();
                const int _height = _bmp_input.height();
                Image<T>* _input = new Image<T>(_width, _height);
                unsigned char* _data = NULL;
                switch (_bmp_input.type())
                {
                    case BmpRead::BT_INDEX:
                        _data = new unsigned char[_width];
                        for (int i = _height - 1; i >= 0; i--)
                        {
                            _bmp_input.read(_data, i, 0, _width);
                            for (int j = 0; j < _width; j++)
                                (*_input)(i, j) = _data[j];
                        }
                        delete[] _data;
                        break;
                    case BmpRead::BT_RGB:
                        _data = new unsigned char[_width * 3];
                        for (int i = _height - 1; i >= 0; i--)
                        {
                            _bmp_input.read(_data, i, 0, _width * 3);
                            for (int j = 0; j < _width; j++)
                                (*_input)(i, j) = 
                                    0.1140 * _data[3*j] + 0.5871 * _data[3*j + 1] + 0.2989 * _data[3*j + 2];
                        }
                        delete[] _data;
                        break;
                    case BmpRead::BT_RGBX:
                        _data = new unsigned char[_width * 4];
                        for (int i = _height - 1; i >= 0; i--)
                        {
                            _bmp_input.read(_data, i, 0, _width * 4);
                            for (int j = 0; j < _width; j++)
                                (*_input)(i, j) =
                                    0.1140 * _data[4*j] + 0.5871 * _data[4*j + 1] + 0.2989 * _data[4*j + 2];
                        }
                        delete[] _data;
                        break;
                    default:
                        delete _input;
                        _input = NULL;
                };
                return _input;
            }
            catch (const char* ex)
            {
                fprintf(stderr, "Image %s could not be loaded because of exception:\n%s\n", filename, ex);
                return NULL;
            }
        }

    template<class T>
        int write(const char* filename, Image<T>* _output)
        {
            try
            {
                if (!_output)
                    throw "ImageIO::write: Nothing could be saved\n";
                const int _width = _output->width();
                const int _height = _output->height();
                BmpWrite _bmp_output(filename, _width, _height, BmpWrite::BT_RGB);
                unsigned char* _data = new unsigned char[_width * 3];
                for (int i = _height; i >= 0; i--)
                {
                    for (int j = 0; j < _width; j++)
                    {
                        T value = (*_output)(i, j);
                        _data[3*j + 2] = _data[3*j + 1] = _data[3*j] = 
                            (unsigned char)(value < 0 ? 0 : (value > 255 ? 255 : value));
                    }
                    _bmp_output.write(_data, i, 0, _width * 3);
                }
                delete[] _data;
                return 0;
            }
            catch (const char* ex)
            {
                fprintf(stderr, "Image %s could not be saved because of exception:\n%s\n", filename, ex);
                return -1;
            }
        }
}
