#ifndef _BMPIO_H_
#define _BMPIO_H_

#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <stdint.h>

const size_t _size_of_header = 0xe;
const size_t _size_of_info = 0x28;

struct BitmapFileHeader
{
    uint16_t _w_type;
    uint32_t _dw_file_size;
    uint16_t _w_reserved_1;
    uint16_t _w_reserved_2;
    uint32_t _dw_offset;
};

struct BitmapInfoHeader
{
    uint32_t _dw_size_of_header;
    uint32_t _dw_width;
    uint32_t _dw_height;
    uint16_t _w_planes;
    uint16_t _w_bit_count;
    uint32_t _dw_compression_method;
    uint32_t _dw_size_of_image;
    uint32_t _dw_pixels_per_X;
    uint32_t _dw_pixels_per_Y;
    uint32_t _dw_colors;
    uint32_t _dw_important_colors;
};

class BmpRead
{
    public:
        /* Type of data: 8 or 24 bits for one pixel */
        enum Type
        {
            BT_INDEX,
            BT_RGB,
            BT_RGBX
        };
    private:
        FILE* _file;
        BitmapFileHeader _header;
        BitmapInfoHeader _info;
        size_t _data_offset;
        size_t _bytes_per_row;
        size_t _size_of_palette;
        unsigned char* _palette;
        Type _image_type;
        void _read_file_header();
        void _read_info_header();
    public:
        BmpRead(const char* filename);
        virtual ~BmpRead();
        size_t width() const;
        size_t height() const;
        size_t bits() const;
        Type type() const;
        void Paleterize(unsigned char* rgb, short int index) const;
        int read(unsigned char* pixel, size_t i, size_t j, ptrdiff_t length);
};

class BmpWrite
{
    public:
        /* Type of data: 8 or 24 bits for one pixel */
        enum Type
        {
            BT_INDEX,
            BT_RGB,
            BT_RGBX
        };
    private:
        FILE* _file;
        BitmapFileHeader _header;
        BitmapInfoHeader _info;
        size_t _data_offset;
        size_t _bytes_per_row;
        size_t _size_of_palette;
        unsigned char* _palette;
        Type _image_type;
        void _write_file_header();
        void _write_info_header();
    public:
        BmpWrite(const char* filename, size_t width, size_t height, BmpWrite::Type type);
        virtual ~BmpWrite();
        size_t width() const;
        size_t height() const;
        size_t bits() const;
        Type type() const;
        int write(unsigned char* pixel, size_t i, size_t j, ptrdiff_t length);
};

#endif /* _BMPIO_H_ */
