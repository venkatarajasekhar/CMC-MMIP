#include <string.h>
#include <cstdio>
#include "image.h"
#include "imageio.h"
#include "lab1.h"
#include "lab2.h"

int main(int argc, char **argv)
{
    GrayscaleFloatImage* first_image = NULL;
    GrayscaleFloatImage* second_image = NULL;
    try
    {
        if (argc < 4)
            throw "Usage: ./out (first_image) (second_image) (command) [parameters...]";

        first_image = ImageIO::read<float>(argv[1]);
        
        /* Methods from Lab1 */
        if (!strcmp(argv[3], "invert"))
        {
            second_image = Lab1::invert(first_image);
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "mirror") && argc == 5)
        {
            second_image = Lab1::mirror(first_image, argv[4]);
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "rotate") && argc == 6)
        {
            second_image = Lab1::rotate(first_image, argv[4], atoi(argv[5]));
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "prewitt") && argc == 5)
        {
            second_image = Lab1::prewitt(first_image, argv[4]);
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "sobel") && argc == 5)
        {
            second_image = Lab1::sobel(first_image, argv[4]);
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "roberts") && argc == 5)
        {
            second_image = Lab1::roberts(first_image, atoi(argv[4]));
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "median") && argc == 5)
        {
            second_image = Lab1::median(first_image, atoi(argv[4]));
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "gauss") && argc == 5)
        {
            second_image = Lab1::gauss(first_image, atof(argv[4]));
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "gradient") && argc == 5)
        {
            second_image = Lab1::gradient(first_image, atof(argv[4]));
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "gabor") && argc == 9)
        {
            second_image = Lab1::gabor(first_image, atof(argv[4]), atof(argv[5]),
                    atof(argv[6]), atof(argv[7]), atof(argv[8]));
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "vessels") && argc == 5)
        {
            second_image = Lab1::vessels(first_image, atof(argv[4]));
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "up_bilinear") && argc == 5)
        {
            second_image = Lab2::up_bilinear(first_image, atof(argv[4]));
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "downsample") && argc == 5)
        {
            second_image = Lab2::downsample(first_image, atof(argv[4]));
            ImageIO::write(argv[2], second_image);
        } else
        if (!strcmp(argv[3], "metric") && argc == 5)
        {
            second_image = ImageIO::read<float>(argv[2]);
            if (!strcmp(argv[4], "mse"))
                fprintf(stdout, "mse = %f\n",
                    Lab2::mse_metric(first_image, second_image));
            else
            if (!strcmp(argv[4], "psnr"))
                fprintf(stdout, "psnr = %f\n",
                    Lab2::psnr_metric(first_image, second_image));
            else
            if (!strcmp(argv[4], "ssim"))
                fprintf(stdout, "ssim = %f\n",
                    Lab2::ssim_metric(first_image, second_image));
            else
            if (!strcmp(argv[4], "mssim"))
                fprintf(stdout, "mssim = %f\n",
                    Lab2::mssim_metric(first_image, second_image));
            else
                throw "Incorrect usage for 'metric' method";
        }
        else
            throw "Usage: ./out (first_image) (second_image) (command) [parameters...]";

        delete first_image;
        delete second_image;
        return 0;
    }
    catch (const char* ex)
    {
        if (first_image)
            delete first_image;
        if (second_image)
            delete second_image;
        fprintf(stderr, "Image %s could not be processed because of exception:\n%s\n"
                "Output image %s has not been created\n", argv[1], ex, argv[2]);
        return -1;
    }
}
