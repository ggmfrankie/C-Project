#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../Utils/CString.h"


#include "libminibmp.h"

#define HEADERSIZE 54
#define IMAGEGRANULARITY 2

#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define SGN(x) ( (x)>0 ? 1 : (x)<0 ? -1 : 0 )
#define ABS(x) ( (x)>0 ? (x) : -(x) )

const pixelcolor COL_black = {0, 0, 0};
const pixelcolor COL_white = {255, 255, 255};
const pixelcolor COL_green = {0, 255, 0};
const pixelcolor COL_red = {0, 0, 255};
const pixelcolor COL_blue = {255, 0, 0};
const pixelcolor COL_yellow = {0, 255, 255};
const pixelcolor COL_cyan = {255, 255, 0};
const pixelcolor COL_purple = {100, 0, 150};
const pixelcolor COL_magenta = {255, 0, 255};

void BMPclear(const bmp *image, const pixelcolor color)
{
    int i=0;
    for (i = 0; i < image->Width * image->Height; *(image->img + i++) = color);
}

bmp newBMP(long dx, const long dy)
{
    bmp image;
    dx = (dx%IMAGEGRANULARITY)?((dx>>IMAGEGRANULARITY)+1)<<IMAGEGRANULARITY : dx;
    image.Type = 'B' | ('M' << 8);

    image.Reserved = 0;
    image.Offset = HEADERSIZE;
    image.headerSize = 40;
    image.Width = dx;
    image.Height = dy;
    image.Planes = 1;
    image.BitsPerPixel = 24;
    image.Compression = 0;
    image.SizeImage = dx * dy * 3;
    image.Size = HEADERSIZE + image.SizeImage;
    image.XPixelsPerMeter = 0;
    image.YPixelsPerMeter = 0;
    image.ColorsUsed = 0;
    image.ColorsImportant = 0;
    image.img = (pixelcolor*) malloc(sizeof(pixelcolor) * dx * dy);
    BMPclear(&image, COL_white);
    return image;
}

void save_BMP(char * name, const bmp *image)
{
    const String fileName = stringOf(name);
    const String defaultPath = stringOf("../Extern/Informatik/Data/");
    String fullPath = Strings.combine(&defaultPath, &fileName);
    FILE* f = fopen(fullPath.content,"wb");
    fwrite(image, HEADERSIZE, 1, f);
    fwrite(image->img, image->SizeImage, 1, f);
    fclose(f);
    Strings.delete_(&fullPath);
}


void setPixel(const bmp *image, const int x, const int y, const pixelcolor color)
{
    const int radr = y * image->Width + x;
    if (x>=0 && x<image->Width && y>=0 && y<image->Height)
        {
            *(image->img + radr) = color;
        }
}

void drawLine(const bmp *image, const Vec2i c1, const Vec2i c2, const pixelcolor color)
{
    int xs = c1.x;
    int ys = c1.y;
    const int dx = c2.x - c1.x;
    const int dy = c2.y - c1.y;
    /* Sonderfall senkrechte Linie*/
    if (dx == 0)
    {
        int i;
        for(i=MIN(c1.y, c2.y); i<=MAX(c1.y, c2.y); i++)
        {
            setPixel(image, c1.x, i, color);
        }
        return;
    }
    /* Sonst mit einfach mit Steigung arbeiten... */
    setPixel(image, xs, ys, color);
    const double steigung = (double) dy / (double) dx;
    while (xs != c2.x || ys != c2.y)
    {
        if (ABS(dx)>=ABS(dy))
        {
            xs += SGN(dx);
            if (ys * SGN(dy) < (floor((xs-c1.x)*steigung + 0.5) + c1.y)*SGN(dy))
                ys += SGN(dy);
        }
        else
        {
            ys += SGN(dy);
            if (xs * SGN(dx) < (floor((ys-c1.y)/steigung + 0.5) + c1.x)*SGN(dx))
                xs += SGN(dx);
        }
        setPixel(image, xs, ys, color);
    }

}
