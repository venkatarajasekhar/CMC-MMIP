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

#include "bmpio.h"

static size_t _row_length(size_t width, int bits) {
	size_t bytes_per_row = 0;
	switch(bits) {
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
	return (bytes_per_row + 3) / 4 * 4;
}

void BmpRead::_read_file_header() {
	fseek(_file, 0, SEEK_SET);
	if (!fread(&_header._w_type, sizeof(uint16_t), 1, _file))
		throw "BmpRead: File header could not be read";
	if (!fread(&_header._dw_file_size, sizeof(uint32_t), 1, _file))
		throw "BmpRead: File header could not be read";
	if (!fread(&_header._w_reserved_1, sizeof(uint16_t), 1, _file))
		throw "BmpRead: File header could not be read";
	if (!fread(&_header._w_reserved_2, sizeof(uint16_t), 1, _file))
		throw "BmpRead: File header could not be read";
	if (!fread(&_header._dw_offset, sizeof(uint32_t), 1, _file))
		throw "BmpRead: File header could not be read";
	return;
}

void BmpRead::_read_info_header() {
	fseek(_file, 0xe, SEEK_SET);
	if (!fread(&_info._dw_size_of_header, sizeof(uint32_t), 1, _file))
		throw "BmpRead: Info header could not be read";
	if (!fread(&_info._dw_width, sizeof(uint32_t), 1, _file))
		throw "BmpRead: Info header could not be read";
	if (!fread(&_info._dw_height, sizeof(uint32_t), 1, _file))
		throw "BmpRead: Info header could not be read";
	if (!fread(&_info._w_planes, sizeof(uint16_t), 1, _file))
		throw "BmpRead: Info header could not be read";
	if (!fread(&_info._w_bit_count, sizeof(uint16_t), 1, _file))
		throw "BmpRead: Info header could not be read";
	if (!fread(&_info._dw_compression_method, sizeof(uint32_t), 1, _file))
		throw "BmpRead: Info header could not be read";
	if (!fread(&_info._dw_size_of_image, sizeof(uint32_t), 1, _file))
		throw "BmpRead: Info header could not be read";
	if (!fread(&_info._dw_pixels_per_X, sizeof(uint32_t), 1, _file))
		throw "BmpRead: Info header could not be read";
	if (!fread(&_info._dw_pixels_per_Y, sizeof(uint32_t), 1, _file))
		throw "BmpRead: Info header could not be read";
	if (!fread(&_info._dw_colors, sizeof(uint32_t), 1, _file))
		throw "BmpRead: Info header could not be read";
	if (!fread(&_info._dw_important_colors, sizeof(uint32_t), 1, _file))
		throw "BmpRead: Info header could not be read";
	return;
}

BmpRead::BmpRead(const char* filename) {
	try {
		_palette = nullptr;
		_file = fopen(filename, "rb");
		if (!_file)
			throw "BmpRead: File could not be opened";
		_read_file_header();
		if (_header._w_type != 0x4d42)
			throw "BmpRead: File is not BMP file";
		_read_info_header();
		if (_info._dw_compression_method)
			throw "BmpRead: Incompatible compression method";
		/* field initialization */
		_data_offset = _header._dw_offset;
		_bytes_per_row = _row_length(_info._dw_width, _info._w_bit_count);
		switch(_info._w_bit_count) {
			case 8:
				_image_type = BT_INDEX;
				_size_of_palette = 0x1FF & (1 << _info._w_bit_count);
				_palette = new uint8_t[_size_of_palette * 4];
				fseek(_file, _size_of_header + _size_of_info, SEEK_SET);
				if (!fread(_palette, sizeof(*_palette), _size_of_palette * 4, _file))
					throw "BmpRead: Color palette could not be read";
				break;
			case 24:
				_image_type = BT_RGB;
				_size_of_palette = 0;
				break;
			case 32:
				_image_type = BT_RGBX;
				_size_of_palette = 0;
				break;
			default:
				throw "BmpRead: Not supported type of pixel format";
		};
	} catch (...) {
		if (_file)
			fclose(_file);
		if (_palette)
			delete[] _palette;
		throw;
	}
}

BmpRead::~BmpRead() {
	if (_file)
		fclose(_file);
	if (_palette)
		delete[] _palette;
}

size_t BmpRead::width() const { return _info._dw_width; }
size_t BmpRead::height() const { return _info._dw_height; }
size_t BmpRead::bits() const { return _info._w_bit_count; }
BmpRead::Type BmpRead::type() const { return _image_type; }

void BmpRead::Paleterize(uint8_t* rgb, short int index) const {
	memcpy(rgb, _palette + 4 * index, 3);
}

int BmpRead::read(uint8_t* pixel, size_t i, size_t j, ptrdiff_t length) {
	fseek(_file,
		_data_offset + (_info._dw_height - 1 - i) * _bytes_per_row + j,
		SEEK_SET
	);
	return fread(pixel, sizeof(*pixel), length, _file);
}
