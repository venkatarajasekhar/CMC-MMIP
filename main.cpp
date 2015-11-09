#include <string.h>
#include <cstdio>
#include "image.h"
#include "imageio.h"
#include "lab1.h"
#include "lab2.h"

int main(int argc, char **argv)
{
    GrayscaleFloatImage* input = NULL;
    GrayscaleFloatImage* output = NULL;
    try
    {
        if (argc < 4)
            throw "Usage: ./out (input_image) (output_image) (command) [parameters...]";

        input = ImageIO::read<float>(argv[1]);
        
        /* Methods from Lab1 */
        if (!strcmp(argv[3], "invert"))
            output = Lab1::invert(input);
        else
        if (!strcmp(argv[3], "mirror") && argc == 5)
            output = Lab1::mirror(input, argv[4]);
        else
        if (!strcmp(argv[3], "rotate") && argc == 6)
            output = Lab1::rotate(input, argv[4], atoi(argv[5]));
        else
        if (!strcmp(argv[3], "prewitt") && argc == 5)
            output = Lab1::prewitt(input, argv[4]);
        else
        if (!strcmp(argv[3], "sobel") && argc == 5)
            output = Lab1::sobel(input, argv[4]);
        else
        if (!strcmp(argv[3], "roberts") && argc == 5)
            output = Lab1::roberts(input, atoi(argv[4]));
        else
        if (!strcmp(argv[3], "median") && argc == 5)
            output = Lab1::median(input, atoi(argv[4]));
        else
        if (!strcmp(argv[3], "gauss") && argc == 5)
            output = Lab1::gauss(input, atof(argv[4]));
        else
        if (!strcmp(argv[3], "gradient") && argc == 5)
            output = Lab1::gradient(input, atof(argv[4]));
        else
        if (!strcmp(argv[3], "gabor") && argc == 9)
            output = Lab1::gabor(input, atof(argv[4]), atof(argv[5]),
                    atof(argv[6]), atof(argv[7]), atof(argv[8]));
        else
        if (!strcmp(argv[3], "vessels") && argc == 5)
            output = Lab1::vessels(input, atof(argv[4]));
        else
        if (!strcmp(argv[3], "up_bilinear") && argc == 5)
            output = Lab2::up_bilinear(input, atof(argv[4]));
        else
            throw "Usage: ./out (input_image) (output_image) (command) [parameters...]";

        delete input;
        /* Writing image to file */
        ImageIO::write(argv[2], output);
        delete output;
        return 0;
    }
    catch (const char* ex)
    {
        if (input)
            delete input;
        if (output)
            delete output;
        fprintf(stderr, "Image %s could not be processed because of exception:\n%s\n"
                "Output image %s has not been created\n", argv[1], ex, argv[2]);
        return -1;
    }
}
