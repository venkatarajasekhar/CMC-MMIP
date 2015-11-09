#ifndef _LAB2_H_
#define _LAB2_H_

#include "image.h"
#include "lab1.h"

namespace Lab2
{
    template <class T>
    Image<T>* up_bilinear(Image<T>* src, float scale)
    {
        const int _width = src->width();
        const int _height = src->height();
        const int new_width = (int)(_width * scale);
        const int new_height = (int)(_height * scale);
        Image<T>* dst = new Image<T>(new_width, new_height);
        if (!dst)
            throw "Lab2::up_bilinear: Insufficient memory to allocate output image";
        for (int i = 0; i < new_height; i++)
        {
            float orig_y = (float)(i) / new_height * _height;
            int src_y = (int)floor(orig_y);
            for (int j = 0; j < new_width; j++)
            {
                float orig_x = (float)(j) / new_width * _width;
                int src_x = (int)floor(orig_x);
                (*dst)(i, j) = 
                    (*src)(src_y, src_x) * (src_x + 1 - orig_x) * (src_y + 1 - orig_y) +
                    (*src)(src_y, src_x + 1) * (orig_x - src_x) * (src_y + 1 - orig_y) +
                    (*src)(src_y + 1, src_x) * (src_x + 1 - orig_x) * (orig_y - src_y) +
                    (*src)(src_y + 1, src_x + 1) * (orig_x - src_x) * (orig_y - src_y);
            }
        }
        return dst;
    }

} /* namespace Lab2 */

#endif /* _LAB2_H_ */
