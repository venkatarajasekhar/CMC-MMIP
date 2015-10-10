#include <string.h>
#include <cstdio>
#include "image.h"
#include "imageio.h"
#include "labs/lab1.h"

int main(int argc, char **argv)
{
        /* Check count of arguments */
        if (argc < 4)
        { 
            printf("Usage: ./out (input_image) (output_image) (command) [parameters...]\n");
            return -1;
        }

        /* Read image from file */
        GrayscaleFloatImage *input = ImageIO::read<float>(argv[1]);
        if (!input)
            /* Nothing should be freed */
            return -1;
        
        /* Methods from Lab1 */
        GrayscaleFloatImage *output = NULL;
        if (!strcmp(argv[3], "invert"))
            output = Lab1::invert(input);
        if (!strcmp(argv[3], "mirror") && argc == 5)
            output = Lab1::mirror(input, argv[4]);
        if (!strcmp(argv[3], "rotate") && argc == 6)
            output = Lab1::rotate(input, argv[4], atoi(argv[5]));
        if (!strcmp(argv[3], "prewitt") && argc == 5)
            output = Lab1::prewitt(input, argv[4]);
        if (!strcmp(argv[3], "sobel") && argc == 5)
            output = Lab1::sobel(input, argv[4]);
        if (!strcmp(argv[3], "roberts") && argc == 5)
            output = Lab1::roberts(input, atoi(argv[4]));
        if (!strcmp(argv[3], "median") && argc == 5)
            output = Lab1::median(input, atoi(argv[4]));
        if (!strcmp(argv[3], "gauss") && argc == 5)
            output = Lab1::gauss(input, atof(argv[4]));
        if (!strcmp(argv[3], "gradient") && argc == 5)
            output = Lab1::gradient(input, atof(argv[4]));

        /* Free input buffer */
        delete input;
        /* Default case for bad usage */
        if (!output)
        {
            printf("Usage: ./out (input_image) (output_image) (command) [parameters...]\n");
            return -1;
        }
        
        /* Writing image to file */
        if (ImageIO::write(argv[2], output))
            return -1;
        /* Free output buffer */
        delete output;
        return 0;
}
