# Questions

## What's `stdint.h`?

stdint.h is a header that declares integer types having specified widths. The `width` of an integer type is the number of bits used to store its value in a pure binary system; the actual type may use more bits than that (for example, a 28-bit type could be stored in 32 bits of actual storage).

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

The size of a data type might vary from machine to machine based on its architecture, e.g. an integer might be of 4 bytes in a machine and 2 bytes in another. The types defined in stdint.h such as `uint8_t`, `uint32_t`, etc. on the other hand will be of the same size in any machine irrespective of its architecture since they specify the width of the type.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

BYTE  - uint8_t  - 8 bytes (0 to 2^8 - 1)  
DWORD - uint32_t - 32 bytes (0 to 2^32 - 1)  
LONG  - int32_t  - 32 bytes (-2^31 to -1 and 0 to 2^31 - 1)  
WORD  - uint16_t - 16 bytes (0 to 2^16 - 1)

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

'B' and 'M' in ASCII  
66 and 77 in decimal  
0x42 and 0x4D in hexadecimal

## What's the difference between `bfSize` and `biSize`?

'bfSize' is the size, in bytes, of the bitmap file.  
bfSize = biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
'biSize' is the size, in bytes, of BITMAPINFOHEADER.  
biSize = sizeof(BITMAPINFOHEADER);

## What does it mean if `biHeight` is negative?

If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount.

## Why might `fopen` return `NULL` in `copy.c`?

`fopen` might return `NULL` if the file does not exist or a permission error occurred while accessing the file because it's read-only or write-protected.

## Why is the third argument to `fread` always `1` in our code?

Because a .bmp file will only have 1 `BITMAPFILEHEADER`, 1 `BITMAPINFOHEADER` and 1 `RGBTRIPLE`, and they are being read sequentially using `fread`.

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;  
padding = (4 - (3 * 3) % 4) % 4;  
padding = 3

## What does `fseek` do?

`fseek` sets the position in the file to the given offset.

## What is `SEEK_CUR`?

Current position in the file.
