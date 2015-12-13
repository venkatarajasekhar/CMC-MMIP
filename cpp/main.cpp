#include <string.h>
#include <cstdio>
#include "image.h"
#include "imageio.h"
#include "transform.h"
#include "metric.h"

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
                    Transform::rotate(first_image, argv[4], atoi(argv[5])));
        else
        if (!strcmp(argv[3], "up_bilinear") && argc == 5)
            ImageIO::write(argv[2],
                    Transform::up_bilinear(first_image, atof(argv[4])));
        else
        if (!strcmp(argv[3], "up_bicubic") && argc == 5)
            ImageIO::write(argv[2],
                    Transform::up_bicubic(first_image, atof(argv[4])));
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
