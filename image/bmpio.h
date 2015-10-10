#ifndef _BMPIO_H_
#define _BMPIO_H_

#include <cstdio>
#include <cstdlib>
#include <cstddef>

#pragma pack(push, 1)
struct BitmapFileHeader
{
    unsigned short _w_type;
    unsigned int _dw_file_size;
    unsigned short _w_reserved_1;
    unsigned short _w_reserved_2;
    unsigned int _dw_offset;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BitmapInfoHeader
{
    unsigned int _dw_size_of_header;
    unsigned int _dw_width;
    unsigned int _dw_height;
    unsigned short _w_planes;
    unsigned short _w_bit_count;
    unsigned int _dw_compression_method;
    unsigned int _dw_size_of_image;
    unsigned int _dw_pixels_per_X;
    unsigned int _dw_pixels_per_Y;
    unsigned int _dw_colors;
    unsigned int _dw_important_colors;
};
#pragma pack(pop)

class BmpRead
{
    public:
        /* Type of data: 8 or 24 bits for one pixel */
        enum Type
        {
            BT_INDEX,
            BT_RGB
        };
    private:
        FILE* _file;
        BitmapFileHeader _header;
        BitmapInfoHeader _info;
        size_t _data_offset;
        size_t _bytes_per_row;
        Type _image_type;
    public:
        BmpRead(const char* filename);
        virtual ~BmpRead();
        size_t width() const;
        size_t height() const;
        size_t bits() const;
        Type type() const;
        int read(unsigned char* pixel, size_t i, size_t j, ptrdiff_t length);
};

class BmpWrite
{
    public:
        /* Type of data: 8 or 24 bits for one pixel */
        enum Type
        {
            BT_INDEX,
            BT_RGB
        };
    private:
        FILE* _file;
        BitmapFileHeader _header;
        BitmapInfoHeader _info;
        size_t _data_offset;
        size_t _bytes_per_row;
        Type _image_type;
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
