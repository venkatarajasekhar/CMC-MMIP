#include <string.h>
#include <cstdio>
#include "image.h"
#include "imageio.h"
#include "lab1/lab1.h"

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
