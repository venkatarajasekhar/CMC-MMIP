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

#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <cmath>
#include "image.h"

namespace Transform {

	template <class T>
	Image<T> invert(Image<T>& src) {
		const size_t _width = src.width();
		const size_t _height = src.height();
		Image<T> dst(_width, _height);
		for (int i = 0; i < _height; i++)
			for (int j = 0; j < _width; j++)
				dst(i, j) = 255 - src(i, j);
		return dst;
	}

	template <class T>
	/* _dir: 0 - x, 1 - y */
	Image<T> mirror(Image<T>& src, const char* dir) {
		const size_t _width = src.width();
		const size_t _height = src.height();
		const int _dir = !strcmp(dir, "x") ? 0 : 1;
		Image<T> dst(_width, _height);
		for (int i = 0; i < _height; i++)
			for (int j = 0; j < _width; j++)
				dst(i, j) = src(_dir ? i : _height - 1 - i, _dir ? _width - 1 - j : j);
		return dst;
	}

	template <class T>
	Image<T> rotate(Image<T>& src, const char* dir, int angle) {
		const size_t _width = src.width();
		const size_t _height = src.height();
		Image<T> dst;
		if (!(angle % 90)) {
			if ((!strcmp(dir, "cw") && angle / 90 % 4 == 1) ||
				(!strcmp(dir, "ccw") && angle / 90 % 4 == 3))
			{
				dst = Image<T>(_height, _width);
				for (int i = 0; i < _width; i++)
					for (int j = 0; j < _height; j++)
						dst(i, j) = src(_height - j - 1, i);
			}
			if ((!strcmp(dir, "cw") && angle / 90 % 4 == 3) ||
				(!strcmp(dir, "ccw") && angle / 90 % 3 == 1))
			{
				dst = Image<T>(_height, _width);
				for (int i = 0; i < _width; i++)
					for (int j = 0; j < _height; j++)
						dst(i, j) = src(j, _width - i - 1);
			}
			if (angle / 90 % 4 == 2) {
				dst = Image<T>(_width, _height);
				for (int i = 0; i < _height; i++)
					for (int j = 0; j < _width; j++)
						dst(i, j) = src(_height - 1 - i, _width - 1 - j);
			}
			if (!(angle / 90 % 4))
				dst = src;
		}
		/* rotation on random angle */
		else {
			const float rad_angle = (float) (
				((!strcmp(dir, "ccw") ? angle : 90 - angle) % 90) * atan(1) / 45
			);
			const int new_height = (int) round(
				sqrt(pow(_width, 2) + pow(_height, 2)) *
				sin(atan((float)_height / (float)_width) + rad_angle)
			);
			const int new_width = (int) round(
				sqrt(pow(_width, 2) + pow(_height, 2)) *
				sin(atan((float)_width / (float)_height) + rad_angle)
			);
			const int dx = (new_width - _width) / 2;
			const int dy = (new_height - _height) / 2;
			Image<T> med(new_width, new_height);
			for (int i = 0; i < new_height; i++)
				for (int j = 0; j < new_width; j++) {
					float len = (float) sqrt(
						pow(new_width / 2 - j, 2) + pow(new_height / 2 - i, 2)
					);
					float turn_angle = (float) (len
						?  (i < new_height / 2 ? acos((new_width / 2 - j) / len)
						: -acos((new_width / 2 - j) / len)) : 0
					);
					float src_x = new_width / 2 -
						cos(turn_angle + rad_angle) * len - dx;
					float src_y = new_height / 2 -
						sin(turn_angle + rad_angle) * len - dy;
					int new_x = (int)floor(src_x);
					int new_y = (int)floor(src_y);
					med(i, j) = (new_x >= 0 && new_x < _width && new_y >= 0 && new_y < _height) ?
						src(new_y, new_x) * (new_x + 1 - src_x) * (new_y + 1 - src_y) +
						src(new_y, new_x + 1) * (src_x - new_x) * (new_y + 1 - src_y) +
						src(new_y + 1, new_x) * (new_x + 1 - src_x) * (src_y - new_y) +
						src(new_y + 1, new_x + 1) * (src_x - new_x) * (src_y - new_y)
						: 0;
				}
			dst = rotate(med, dir, !strcmp(dir, "ccw")
				? (angle / 90) * 90 : (angle / 90 + 1) * 90
			);
		}
		return dst;
	}

	template <class T>
	Image<T> up_bilinear(Image<T>& src, float scale) {
		const int _width = src.width();
		const int _height = src.height();
		const int new_width = (int)(_width * scale);
		const int new_height = (int)(_height * scale);
		Image<T> dst(new_width, new_height);
		for (int i = 0; i < new_height; i++) {
			float orig_y = (float)(i) / new_height * _height;
			int src_y = (int)floor(orig_y);
			for (int j = 0; j < new_width; j++) {
				float orig_x = (float)(j) / new_width * _width;
				int src_x = (int)floor(orig_x);
				dst(i, j) =
					src(src_y, src_x) * (src_x + 1 - orig_x) * (src_y + 1 - orig_y) +
					src(src_y, src_x + 1) * (orig_x - src_x) * (src_y + 1 - orig_y) +
					src(src_y + 1, src_x) * (src_x + 1 - orig_x) * (orig_y - src_y) +
					src(src_y + 1, src_x + 1) * (orig_x - src_x) * (orig_y - src_y);
			}
		}
		return dst;
	}

	template <class T>
	Image<T> up_bicubic(Image<T>& src, float scale) {
		const int _width = src.width();
		const int _height = src.height();
		const int new_width = (int)(_width * scale);
		const int new_height = (int)(_height * scale);
		Image<T> dst(new_width, new_height);
		for (int i = 0; i < new_height; i++) {
			int src_y = (int)floor((float)(i) / new_height * _height);
			float y = (float)(i) / new_height * _height - src_y;
			for (int j = 0; j < new_width; j++) {
				int src_x = (int)floor((float)(j) / new_width * _width);
				float x = (float)(j) / new_width * _width - src_x;
				dst(i, j) =
					src(src_y - 1, src_x - 1) * x*(x-1)*(x-2)*y*(y-1)*(y-2) / 36 -
					src(src_y - 1, src_x) * (x+1)*(x-1)*(x-2)*y*(y-1)*(y-2) / 12 +
					src(src_y - 1, src_x + 1) * (x+1)*x*(x-2)*y*(y-1)*(y-2) / 12 -
					src(src_y - 1, src_x + 2) * (x+1)*x*(x-1)*y*(y-1)*(y-2) / 36 -
					src(src_y, src_x - 1) * x*(x-1)*(x-2)*(y+1)*(y-1)*(y-2) / 12 +
					src(src_y, src_x) * (x+1)*(x-1)*(x-2)*(y+1)*(y-1)*(y-2) / 4 -
					src(src_y, src_x + 1) * (x+1)*x*(x-2)*(y+1)*(y-1)*(y-2) / 4 +
					src(src_y, src_x + 2) * (x+1)*x*(x-1)*(y+1)*(y-1)*(y-2) / 12 +
					src(src_y + 1, src_x - 1) * x*(x-1)*(x-2)*(y+1)*y*(y-2) / 12 -
					src(src_y + 1, src_x) * (x+1)*(x-1)*(x-2)*(y+1)*y*(y-2) / 4 +
					src(src_y + 1, src_x + 1) * (x+1)*x*(x-2)*(y+1)*y*(y-2) / 4 -
					src(src_y + 1, src_x + 2) * (x+1)*x*(x-1)*(y+1)*y*(y-2) / 12 -
					src(src_y + 2, src_x - 1) * x*(x-1)*(x-2)*(y+1)*y*(y-1) / 36 +
					src(src_y + 2, src_x) * (x+1)*(x-1)*(x-2)*(y+1)*y*(y-1) / 12 -
					src(src_y + 2, src_x + 1) * (x+1)*x*(x-2)*(y+1)*y*(y-1) / 12 +
					src(src_y + 2, src_x + 2) * (x+1)*x*(x-1)*(y+1)*y*(y-1) / 36;
			}
		}
		return dst;
	}

	template <class T>
	Image<T> downsample(Image<T>& src, float scale) {
		const int _width = src.width();
		const int _height = src.height();
		const int new_width = (int)(_width / scale);
		const int new_height = (int)(_height / scale);
		Image<T> dst(new_width, new_height);
		Image<T>&& med = src; /* here have to be placed gauss blur */
		float orig_x = 0, orig_y = 0;
		for (int i = 0; i < new_height; i++, orig_y += scale, orig_x = 0)
			for (int j = 0; j < new_width; j++, orig_x += scale) {
				int src_x = (int)floor(orig_x);
				int src_y = (int)floor(orig_y);
				dst(i, j) =
					med(src_y, src_x) * (src_x + 1 - orig_x) * (src_y + 1 - orig_y) +
					med(src_y, src_x + 1) * (orig_x - src_x) * (src_y + 1 - orig_y) +
					med(src_y + 1, src_x) * (src_x + 1 - orig_x) * (orig_y - src_y) +
					med(src_y + 1, src_x + 1) * (orig_x - src_x) * (orig_y - src_y);
			}
		return dst;
	}

} /* namespace Transform */

#endif /* _TRANSFORM_H_ */
