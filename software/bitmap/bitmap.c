/*
 * bitmap.c
 *
 * proste funkcje rd/wr bmp
 *
 */
#include <stdio.h>	// I/O
#include <stdlib.h>	// malloc();
#include <sys/types.h>	// open();
#include <sys/stat.h>	// open();
#include <fcntl.h>	// open();
#include <unistd.h>	// open(); read(); write();

#include "bitmap.h"	// nasza "biblioteka" do bmp'gow



// wczytuje obrazek z pliku
int bmp_load_file(struct bmp *pic, const char *file)
{
 int fd;					// FileDescriptor
 unsigned long y;				// pomocnicza...
 unsigned long y_max, rd_size, offset, x_max;	// przydadza sie ze wzgledow 
						// wydajnosciowych

 // otwieramy plik
 if( (fd=open(file, O_RDONLY))==-1 )
   return BMP_ERR_OPEN;

 // zczytujemy oba naglowki
 if( read(fd, &(pic->core_h), SO_COREH)!=SO_COREH ||
     read(fd, &(pic->info_h), SO_INFOH)!=SO_INFOH    )
 {
   close(fd);
   return BMP_ERR_READ;
 };

 // sprawdzamy zgodnosc z nasza biblioteka
 if( pic->info_h.biPlanes!=1      ||
     pic->info_h.biBitCount!=24   ||	// rgb?
     pic->info_h.biCompression!=0    )	// nie kodowane?
 {
   close(fd);
   return BMP_ERR_NOTBMP;
 };

 // przydzielamy pamiec na obrazek
 pic->pixels=(struct rgb24 *)malloc( 
                             pic->info_h.biHeight * pic->info_h.biWidth * 
                             sizeof(struct rgb24) );
 if( pic->pixels==NULL )
 {
   close(fd);
   return BMP_ERR_MEM;
 };

 // wczytujemy obrazek, pomijajac eventualne dopelnienie do 
 // mielowrotnosci 4 bajtow...
 for(y=0, y_max=pic->info_h.biHeight, x_max=pic->info_h.biWidth,
     rd_size=x_max*sizeof(struct rgb24), offset=x_max%4;
     y<y_max; y++)
 {
   if( read(fd, pic->pixels+y*x_max, rd_size)!=rd_size )	// wczytujemy 
								// linijkami
   {
     close(fd);
     bmp_free(pic);
     return BMP_ERR_READ;
   };
   lseek(fd, offset, SEEK_CUR);		// pomijamy te kretynskie bajty...
 }; // for(linijki)

 // wyglada na koniec zabawy! :-)
 close(fd);

 return 0;
}; // bmp_load_file()




// zapisuje obrazek do pliku
int bmp_save_file(struct bmp *pic, const char *file, int mode)
{
 int fd;					// FileDescriptor
 unsigned long y;				// pomocnicza...
 unsigned long y_max, wr_size, offset, x_max;	// przydadza sie ze wzgledow 
						// wydajnosciowych

 // otwieramy plik
 if( (fd=open(file, O_WRONLY|O_CREAT|O_TRUNC, mode))==-1 )
   return BMP_ERR_OPEN;

 if( write(fd, &(pic->core_h), SO_COREH)!=SO_COREH ||
     write(fd, &(pic->info_h), SO_INFOH)!=SO_INFOH    )
 {
   close(fd);
   return BMP_ERR_WRITE;
 };

 // wczytujemy obrazek, pomijajac eventualne dopelnienie do mielowrotnosci 4 
 // bajtow...
 for(y=0, y_max=pic->info_h.biHeight, x_max=pic->info_h.biWidth,
     wr_size=x_max*sizeof(struct rgb24), offset=x_max%4;
     y<y_max; y++)
 {
   if( write(fd, pic->pixels+y*x_max, wr_size)!=wr_size )	// zapisuj 
								// linijkami
   {
     close(fd);
     return BMP_ERR_WRITE;
   };
   if( write(fd, "\0\0\0", offset)!=offset )	// dodajemy te kretynskie 
						// bajty...
   {
     close(fd);
     return BMP_ERR_WRITE;
   };
 }; // for(linijki)

 // konczymy
 close(fd);

 return 0;
}; // bmp_save_file();




// zwalnia zasoby
int bmp_free(struct bmp *pic)
{
 free(pic->pixels);
 return 0;
};

