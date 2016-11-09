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

void BmpWrite::_write_file_header() {
	fseek(_file, 0, SEEK_SET);
	if (!fwrite(&_header._w_type, sizeof(uint16_t), 1, _file))
		throw "BmpWrite: File header could not be written";
	if (!fwrite(&_header._dw_file_size, sizeof(uint32_t), 1, _file))
		throw "BmpWrite: File header could not be written";
	if (!fwrite(&_header._w_reserved_1, sizeof(uint16_t), 1, _file))
		throw "BmpWrite: File header could not be written";
	if (!fwrite(&_header._w_reserved_2, sizeof(uint16_t), 1, _file))
		throw "BmpWrite: File header could not be written";
	if (!fwrite(&_header._dw_offset, sizeof(uint32_t), 1, _file))
		throw "BmpWrite: File header could not be written";
	return;
}

void BmpWrite::_write_info_header() {
	fseek(_file, 0xe, SEEK_SET);
	if (!fwrite(&_info._dw_size_of_header, sizeof(uint32_t), 1, _file))
		throw "BmpWrite: Info header could not be written";
	if (!fwrite(&_info._dw_width, sizeof(uint32_t), 1, _file))
		throw "BmpWrite: Info header could not be written";
	if (!fwrite(&_info._dw_height, sizeof(uint32_t), 1, _file))
		throw "BmpWrite: Info header could not be written";
	if (!fwrite(&_info._w_planes, sizeof(uint16_t), 1, _file))
		throw "BmpWrite: Info header could not be written";
	if (!fwrite(&_info._w_bit_count, sizeof(uint16_t), 1, _file))
		throw "BmpWrite: Info header could not be written";
	if (!fwrite(&_info._dw_compression_method, sizeof(uint32_t), 1, _file))
		throw "BmpWrite: Info header could not be written";
	if (!fwrite(&_info._dw_size_of_image, sizeof(uint32_t), 1, _file))
		throw "BmpWrite: Info header could not be written";
	if (!fwrite(&_info._dw_pixels_per_X, sizeof(uint32_t), 1, _file))
		throw "BmpWrite: Info header could not be written";
	if (!fwrite(&_info._dw_pixels_per_Y, sizeof(uint32_t), 1, _file))
		throw "BmpWrite: Info header could not be written";
	if (!fwrite(&_info._dw_colors, sizeof(uint32_t), 1, _file))
		throw "BmpWrite: Info header could not be written";
	if (!fwrite(&_info._dw_important_colors, sizeof(uint32_t), 1, _file))
		throw "BmpWrite: Info header could not be written";
	return;
}

BmpWrite::BmpWrite(const char* filename, size_t width, size_t height,
	BmpWrite::Type type)
{
	try {
		/* field initialization */
		int bits = 0;
		_palette = nullptr;
		_image_type = type;
		switch(_image_type) {
			case BT_INDEX:
				bits = 8;
				_size_of_palette = 0x1FF & (1 << bits);
				_palette = new uint8_t[_size_of_palette * 4];
				std::memset(_palette, 0, _size_of_palette * 4);
				for (size_t i = 0; i < _size_of_palette; i++)
					_palette[4*i] = _palette[4*i + 1] = _palette[4*i + 2] = i;
				break;
			case BT_RGB:
				_size_of_palette = 0;
				bits = 24;
				break;
			case BT_RGBX:
				bits = 32;
				_size_of_palette = 0;
				break;
			default:
				throw "BmpWrite: Not supported type of pixel format";
		};
		_bytes_per_row = _row_length(width, bits);
		_data_offset = _size_of_header + _size_of_info +
			(bits > 8 ? 0 : 4 * (1 << bits));
		/* file header and info header creating */
		_header._w_type = 0x4d42;
		_header._w_reserved_1 = 0;
		_header._w_reserved_2 = 0;
		_header._dw_offset = _data_offset;
		_info._dw_size_of_header = _size_of_info;
		_info._dw_width = (uint32_t) width;
		_info._dw_height = (uint32_t) height;
		_info._w_planes = 1;
		_info._w_bit_count = bits;
		_info._dw_compression_method = 0;
		_info._dw_size_of_image = _bytes_per_row * height;
		_info._dw_pixels_per_X = 0;
		_info._dw_pixels_per_Y = 0;
		_info._dw_colors = _size_of_palette;
		_info._dw_important_colors = 0;
		_header._dw_file_size = _header._dw_offset + _info._dw_size_of_image;

		_file = fopen(filename, "wb");
		if (!_file)
			throw "BmpWrite: File could not be opened";
		fseek(_file, 0, SEEK_SET);
		_write_file_header();
		_write_info_header();
		if (_palette && !fwrite(_palette, sizeof(*_palette), _size_of_palette * 4, _file))
			throw "BmpWrite: Color palette could not be saved";
	} catch (...) {
		if (_file)
			fclose(_file);
		if (_palette)
			delete[] _palette;
		throw;
	}
}

BmpWrite::~BmpWrite() {
	if (_file)
		fclose(_file);
	if (_palette)
		delete[] _palette;
}

size_t BmpWrite::width() const { return _info._dw_width; }
size_t BmpWrite::height() const { return _info._dw_height; }
size_t BmpWrite::bits() const { return _info._w_bit_count; }
BmpWrite::Type BmpWrite::type() const { return _image_type; }

int BmpWrite::write(uint8_t* pixel, size_t i, size_t j, ptrdiff_t length) {
	fseek(_file,
		_data_offset + (_info._dw_height - 1 - i) * _bytes_per_row + j,
		SEEK_SET
	);
	return fwrite(pixel, sizeof(*pixel), _bytes_per_row, _file);
}
