fndef _LAB1_H_
#define _LAB1_H_

#include <cstdio>
#include <algorithm>
#include <cmath>
#include "../image.h"

namespace Lab1
{
    template <class T>
        Image<T>* invert(Image<T>* src)
        {
            Image<T>* dst = new Image<T>(src->width(), src->height());
            for (int i = 0; i < src->height(); i++)
                for (int j = 0; j < src->width(); j++)
                    (*dst)(i, j) = 255 - (*src)(i, j);
            return dst;
        }
}

#endif //_LAB1_H_
