#ifndef _BMPIO_H_
#define _BMPIO_H_

#include <cstdio>
#include <cstdlib>
#include <cstddef>

#pragma pack(push, 1)
struct BitmapFileHeader
{
    unsigned short _w_type;
    unsigned long _dw_file_size;
    unsigned short _w_reserved_1;
    unsigned short _w_reserved_2;
    unsigned long _dw_offset;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BitmapInfoHeader
{
    unsigned long _dw_size_of_header;
    unsigned long _dw_width;
    unsigned long _dw_height;
    unsigned short _w_planes;
    unsigned short _w_bit_count;
    unsigned long _dw_compression_method;
    unsigned long _dw_size_of_image;
    unsigned long _dw_pixels_per_X;
    unsigned long _dw_pixels_per_Y;
    unsigned long _dw_colors;
    unsigned long _dw_important_colors;
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
        int read(unsigned char* pixel, size_t i, size_t j);
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
        BmpWrite(const char* filename, int width, int height, BmpWrite::Type type);
        virtual ~BmpWrite();
        size_t width() const;
        size_t height() const;
        size_t bits() const;
        Type type() const;
        int write(unsigned char* pixel, size_t i, size_t j);
};

#endif /* _BMPIO_H_ */
