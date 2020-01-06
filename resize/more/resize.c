// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // remember factor and filenames
    float factor = atof(argv[1]);
    if (factor <= 0 || factor > 100)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // remember width of input bitmap
    LONG old_width = bi.biWidth;

    // remember height of input bitmap
    LONG old_height = abs(bi.biHeight);

    // remember padding for scanlines of input bitmap
    int old_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine width of resized bitmap
    bi.biWidth = round(bi.biWidth * factor);

    // determine height of resized bitmap
    bi.biHeight = round(bi.biHeight * factor);

    // determine padding for scanlines of resized bitmap
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine size of resized bitmap excluding headers
    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + padding) * abs(bi.biHeight);

    // determine size of resized bitmap file including headers
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0; i < old_height; i++)
    {
        // resize bitmap height-wise
        for (int j = 0; j < factor; j++)
        {
            // iterate over pixels in scanline
            for (int k = 0; k < old_width; k++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

                if (factor < 1)
                {
                    // skip over necessary number of RGB triples to downscale bitmap horizontally
                    fseek(inptr, (old_width / bi.biWidth - 1) * sizeof(RGBTRIPLE), SEEK_CUR);
                    k += old_width / bi.biWidth - 1;
                }
                else
                {
                    // write more RGB triples to upscale bitmap horizontally
                    for (int l = 1; l < factor; l++)
                    {
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                }
            }

            // add padding, if any, to outfile
            for (int p = 0; p < padding; p++)
            {
                fputc(0x00, outptr);
            }

            // rewind file pointer position to beginning of line to upscale bitmap vertically
            if (factor >= 1)
            {
                if (j < factor - 1)
                {
                    fseek(inptr, -old_width * sizeof(RGBTRIPLE), SEEK_CUR);
                }
            }
            else
            {
                break;
            }
        }

        // skip over padding, if any
        fseek(inptr, old_padding, SEEK_CUR);

        // skip over necessary number of scanlines to downscale bitmap vertically
        if (factor < 1)
        {
            fseek(inptr, (old_height / abs(bi.biHeight) - 1) * (sizeof(RGBTRIPLE) * old_width + old_padding), SEEK_CUR);
            i += old_height / abs(bi.biHeight) - 1;
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}