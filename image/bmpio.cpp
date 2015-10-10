#include "bmpio.h"

size_t _row_length(size_t width, int bits)
{
    size_t bytes_per_row = 0;
    switch(bits)
    {
        case 8:
            bytes_per_row = width;
            break;
        case 24:
            bytes_per_row = 3 * width;
            break;
        case 32:
            bytes_per_row = 4 * width;
            break;
        default:
            throw "Not supported type of pixel format";
    };
    return (bytes_per_row + 3) /4 * 4;    
}

BmpRead::BmpRead(const char* filename)
{
    try
    {
        _file = fopen(filename, "rb");
        if (!_file)
            throw "BmpRead: File could not be opened";
        fseek(_file, 0, SEEK_SET);
        /* file header loading */
        if (!fread(&_header, sizeof(_header), 1, _file))
            throw "BmpRead: File header could not be read";
        /* check if file is BMP image */
        if (_header._w_type != 0x4d42)
            throw "BmpRead: File is not BMP file";
        /* info header loading */
        if (!fread(&_info, sizeof(_info), 1, _file))
            throw "BmpRead: Info header could not be read";

        /* field initialization */
        _data_offset = _header._dw_offset;
        _bytes_per_row = _row_length(_info._dw_width, _info._w_bit_count);
        switch(_info._w_bit_count)
        {
            case 8:
                _image_type = BT_INDEX;
                break;
            case 24:
                _image_type = BT_RGB;
                break;
            case 32:
                _image_type = BT_RGBX;
                break;
            default:
                throw "BmpRead: Not supported type of pixel format";
        };
    }
    catch (...)
    {
        if (_file)
            fclose(_file);
        throw;
    }
}

BmpRead::~BmpRead()
{
    fclose(_file);
}

size_t BmpRead::width() const { return _info._dw_width; }
size_t BmpRead::height() const { return _info._dw_height; }
size_t BmpRead::bits() const { return _info._w_bit_count; }
BmpRead::Type BmpRead::type() const { return _image_type; }
int BmpRead::read(unsigned char* pixel, size_t i, size_t j, ptrdiff_t length)
{
    fseek(_file, _data_offset + (_info._dw_height - 1 - i) * _bytes_per_row + j, SEEK_SET);
    return fread(pixel, sizeof(*pixel), length, _file);
}

BmpWrite::BmpWrite(const char* filename, size_t width, size_t height, BmpWrite::Type type)
{
    try
    {
        /* field initialization */
        int bits = 0;
        _image_type = type;
        switch(_image_type)
        {
            case BT_INDEX:
                bits = 8;
                break;
            case BT_RGB:
                bits = 24;
                break;
            case BT_RGBX:
                bits = 32;
                break;
            default:
                throw "BmpWrite: Not supported type of pixel format";
        };
        _bytes_per_row = _row_length(width, bits);
        _data_offset = sizeof(_header) + sizeof(_info) + (bits == 24 ? 0 : 4 * (1 << bits));
        
        /* file header and info header creating */
        _header._w_type = 0x4d42;
        _header._w_reserved_1 = 0;
        _header._w_reserved_2 = 0;
        _header._dw_offset = _data_offset;
        _info._dw_size_of_header = sizeof(_info);
        _info._dw_width = (unsigned int) width;
        _info._dw_height = (unsigned int) height;
        _info._w_planes = 1;
        _info._w_bit_count = bits;
        _info._dw_compression_method = 0;
        _info._dw_size_of_image = _bytes_per_row * height;
        _info._dw_pixels_per_X = 0;
        _info._dw_pixels_per_Y = 0;
        _info._dw_colors = 0;
        _info._dw_important_colors = 0;
        _header._dw_file_size = _header._dw_offset + _info._dw_size_of_image;

        _file = fopen(filename, "wb");
        if (!_file)
            throw "BmpWrite: File could not be opened";
        fseek(_file, 0, SEEK_SET);
        /* file header saving */
        if (!fwrite(&_header, sizeof(_header), 1, _file))
            throw "BmpWrite: File header could not be saved";
        /* info header saving */
        if (!fwrite(&_info, sizeof(_info), 1, _file))
            throw "BmpWrite: Info header could not be saved";
    }
    catch (...)
    {
        if (_file)
            fclose(_file);
        throw;
    }
}

BmpWrite::~BmpWrite()
{
    fclose(_file);
}

size_t BmpWrite::width() const { return _info._dw_width; }
size_t BmpWrite::height() const { return _info._dw_height; }
size_t BmpWrite::bits() const { return _info._w_bit_count; }
BmpWrite::Type BmpWrite::type() const { return _image_type; }
int BmpWrite::write(unsigned char* pixel, size_t i, size_t j, ptrdiff_t length)
{
    fseek(_file, _data_offset + (_info._dw_height - 1 - i) * _bytes_per_row + j, SEEK_SET);
    return fwrite(pixel, sizeof(*pixel), length, _file);
}
