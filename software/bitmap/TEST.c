#include <stdlib.h>
#include <stdio.h>

//#include "bitmap.h"

struct bmp_core_header
{
 char           bfType[2];      // determinuje czy plik jest zapisany w
                                // formacie BMP -> powinno byc "BM"
 unsigned long  bfSize;         // okresla rozmiar CALEGO pliq
 unsigned short bfReserved1;    // zarezerwowane -> 0
 unsigned short bfReserved2;    // zarezerwowane -> 0
 unsigned long  bfOffBits;      // offset danych wzgledem poczatq pliq
};


struct x
{
 char a[14];
};


int main(void)
{
 struct bmp_core_header bh;
 struct x               xx;
 struct bmp_core_header *p;
 p=(struct bmp_core_header*)malloc(14);

 p->bfType[0]  ='?';
 p->bfType[1]  =0;
 p->bfSize     =10;
 p->bfReserved1=20;
 p->bfReserved2=30;
 p->bfOffBits  =40;

 printf("%d\n %d\n %d\n", sizeof(struct x), sizeof(bh), sizeof(xx) );

 printf("%d vs. %d\n", sizeof(struct bmp_core_header), /*SO_COREH*/14);
 printf("ul==%d us==%d\n", sizeof(unsigned long), sizeof(unsigned short) );
 return 0;
};

