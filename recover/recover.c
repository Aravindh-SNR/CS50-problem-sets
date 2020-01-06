#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // remember filename
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // output file pointer
    FILE *outptr;

    // output filename
    char *outfile = malloc(8 * sizeof(char));
    int counter = 0;

    // store one block of 512 bytes
    unsigned char block[512];

    while (1)
    {
        // read 512 bytes from infile
        fread(block, sizeof(block), 1, inptr);

        // break if end of file is reached
        if (feof(inptr))
        {
            break;
        }

        // check for jpeg beginning
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && block[3] >= 0xe0 && block[3] <= 0xef)
        {
            // close output file if it is already open
            if (outptr != NULL)
            {
                fclose(outptr);
            }

            // determine next output filename
            sprintf(outfile, "%.3i.jpg", counter);

            // open new output file
            outptr = fopen(outfile, "w");
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", outfile);
                return 3;
            }

            // write 512 bytes to outfile
            fwrite(block, sizeof(block), 1, outptr);

            counter++;
        }
        else
        {
            // write 512 bytes to outfile only if it is open
            if (outptr != NULL)
            {
                fwrite(block, sizeof(block), 1, outptr);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // free memory allocated for storing outfile name
    free(outfile);

    // success
    return 0;
}