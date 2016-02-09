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

#include <string.h>
#include <cstdio>
#include "image.h"
#include "imageio.h"
#include "transform.h"
#include "metric.h"
#include "detection.h"

int main(int argc, char **argv)
{
    try
    {
        if (argc < 4)
            throw "Usage: ./out (first_image) (second_image) (command) [parameters...]";

        GrayscaleFloatImage&& first_image = ImageIO::read<float>(argv[1]);
        
        /* Methods from Transform */
        if (!strcmp(argv[3], "invert"))
            ImageIO::write(argv[2], Transform::invert(first_image));
        else
        if (!strcmp(argv[3], "mirror") && argc == 5)
            ImageIO::write(argv[2], Transform::mirror(first_image, argv[4]));
        else
        if (!strcmp(argv[3], "rotate") && argc == 6)
            ImageIO::write(argv[2],
                    Transform::rotate(first_image, argv[4],
                        strtof(argv[5], nullptr)));
        else
        if (!strcmp(argv[3], "up_bilinear") && argc == 5)
            ImageIO::write(argv[2],
                    Transform::up_bilinear(first_image,
                        strtof(argv[4], nullptr)));
        else
        if (!strcmp(argv[3], "up_bicubic") && argc == 5)
            ImageIO::write(argv[2],
                    Transform::up_bicubic(first_image,
                        strtof(argv[4], nullptr)));
        else
        if (!strcmp(argv[3], "metric") && argc == 5)
        {
            GrayscaleFloatImage&& second_image = ImageIO::read<float>(argv[2]);
            if (!strcmp(argv[4], "mse"))
                fprintf(stdout, "mse = %f\n",
                        Metric::mse_metric(first_image, second_image));
            else
            if (!strcmp(argv[4], "psnr"))
                fprintf(stdout, "psnr = %f\n",
                        Metric::psnr_metric(first_image, second_image));
            else
            if (!strcmp(argv[4], "ssim"))
                fprintf(stdout, "ssim = %f\n",
                        Metric::ssim_metric(first_image, second_image));
            else
            if (!strcmp(argv[4], "mssim"))
                fprintf(stdout, "mssim = %f\n",
                        Metric::mssim_metric(first_image, second_image));
            else
                throw "Incorrect usage for 'metric' method";
        }
        else
        if (!strcmp(argv[3], "sobel") && argc == 5)
            ImageIO::write(argv[2],
                    Detection::sobel(first_image, argv[4]));
        else
            throw "Usage: ./out (first_image) (second_image) (command) [parameters...]";
        return 0;
    }
    catch (const char* ex)
    {
        fprintf(stderr, "Image %s could not be processed because of exception:\n%s\n"
                "Output image %s has not been created\n", argv[1], ex, argv[2]);
        return -1;
    }
}
