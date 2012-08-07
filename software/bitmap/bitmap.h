/*
 * bitmap.h
 *
 * plik zawiera strukture naglowqw w bitmapach. struktura pliq:
 *
 * <coreheader><infoheader><raw_data>
 *
 * przy czym raw_data to linie, zapisane w kolejnosci od
 * najnizszej, do najwyzszej! (linia_n,....,linia_1)
 *
 */
#ifndef INCLUDE_BITMAP_H
#define INCLUDE_BITMAP_H


//struktura 1st naglowka
struct bmp_core_header
{
 char           bfType[2];	// determinuje czy plik jest zapisany w 
				// formacie BMP -> powinno byc "BM"
 unsigned long  bfSize;		// okresla rozmiar CALEGO pliq
 unsigned short bfReserved1;	// zarezerwowane -> 0
 unsigned short bfReserved2;	// zarezerwowane -> 0
 unsigned long  bfOffBits;	// offset danych wzgledem poczatq pliq
} __attribute__((packed));


// struktura 2nd naglowka
struct bmp_info_header
{
 unsigned long  biSize;		// dlugosc tego naglowka -> 40
 unsigned long  biWidth;	// szerokosc rysunq w punktach
 unsigned long  biHeight;	// wysokosc rysunq w punktach
 unsigned short biPlanes;	// "liczba planow w urzadzeniu docelowym" 
				// (??) -> 1 (zawsze!)
 unsigned short biBitCount;	// ile bitow opisuje punkt? -> 24==true color
 unsigned long  biCompression;	// rodzaj kompresji -> 0==brak kompresji
 unsigned long  biSizeImage;	// dlugosc obszaru danych W BAJTACH
 unsigned long  biXPelsPerMeter;	// rozdzielczosc pozioma obrazka w 
					// punktach na metr -> 0==OK
 unsigned long  biYPelsPerMeter;	// rozdzielczosc pionowa obrazka w 
					// punktach na metr -> 0==OK
 unsigned long  biClrUsed;	// okresla ile kolorow z palety jest 
				// wykozystywanych w rysunq -> 0==wszystkie
 unsigned long  biClrImportant;	// okresla ile kolorow ystarcza aby poprawnie 
				// wyswietlic rysunek -> 0==wszystkie
} __attribute__((packed));


//#define  SO_COREH	14				// dlaczego sizeof(core_h) nie dziala poprawnie????
#define  SO_COREH	sizeof(struct bmp_core_header)	// rozmiar
#define  SO_INFOH	sizeof(struct bmp_info_header)	// to z kolei jest ok!


// opis pojedynczego pixel'a dla 24-bit'ow
struct rgb24
{
 unsigned char b;
 unsigned char g;
 unsigned char r;
};


// glowna strktura - przchowuje wszystkie informacje o obrazie
struct bmp
{
 struct bmp_core_header  core_h;
 struct bmp_info_header  info_h;
 struct rgb24           *pixels;	// obrazek jest przechowywany
					// w pamieci tak samo jako na dysku,
					// czyli w postaci "odwroconej"
};


/*
 * bledy
 */
#define  BMP_ERR_OPEN	(-1)
#define  BMP_ERR_CLOSE	(-2)
#define  BMP_ERR_READ	(-3)
#define  BMP_ERR_WRITE	(-4)
#define  BMP_ERR_NOTBMP	(-5)
#define  BMP_ERR_MEM	(-6)


/*
 * FUNCKJE
 */

// wczytaj bitmape z pliku
int bmp_load_file(struct bmp *pic, const char *file);

// zapisz bitmape do pliku
int bmp_save_file(struct bmp *pic, const char *file, int mode);

// zwalnia zasoby zajmowane przez strukture, ZA WYJATKIEM STRUKTURY WLASCIWEJ
// (wystarczy po tym zabiegu zrobic free(obrazek) & bedzie juz komplet :))
int bmp_free(struct bmp *pic);




// pobiera wskaznik na pixel
#define  bmp_get_pixel_ptr(pic, w, h)	\
		( (pic)->pixels + (h)*(pic)->info_h.biWidth + (w) )

// pobiera wartosc pixela
#define  bmp_get_pixel(pic, w, h)	\
		( *bmp_get_pixel_ptr(pic, w, h) )

// zapisuje pixel
#define  bmp_set_pixel(pic, w, h, p)	\
		( bmp_get_pixel(pic, w, h)=*(p) )



#endif

