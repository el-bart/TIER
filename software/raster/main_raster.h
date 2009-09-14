/*
 * main_raster.h
 *
 * implementacja obrazu w postaci rastra X na Y pixeli.
 *
 * wszystkie wywolania zwracaja 0 w przypadku powodzenia, lub
 * wartosc ujemna w przypadku bledu.
 *
 */
#ifndef INCLUDE_MAIN_RASTER_H_FILE
#define INCLUDE_MAIN_RASTER_H_FILE

#include "bitmap/bitmap.h"


// definicja typu jakiego jest kolor
// 0 - czarny ; 255 - bialy
//#define color unsigned char


/*
// podstawowa struktura pixel'a
struct rgb
{
 color r;	// skladowa Red
 color g;	// skladowa Green
 color b;	// skladowa Blue
}; // struct rgb
*/


// struktura calego rastra
struct raster
{
 int dim_x;	// rozmiar x - ilosc kolumn
 int dim_y;	// rozmiar y - ilosc wierszy

 struct rgb24  *pixs;	// pixele - tablica w postaci
			// [x1;y1]...[xn;y1][x1;y2]...[xn;y2]...[x1;ym]...
 short int     *marker;	// tablica takiego samoego rozmiaru jak powyzsza. dla
			// kazdego pixela mozna dodac pewien stan - przydaje
			// sie do algorytmow rekurencyjnego chodzenia po
			// obrazkach. domyslnie NIE jest wlaczany!
};




// tworzy i zwraca strukture o zadanym rozmiarze
struct raster *raster_create(int dim_x, int dim_y);

// zwalnia pamiec zajmowana przez raster
int raster_free(struct raster *r);


// ustawia caly raster na kolor czarny
int raster_black(struct raster *r);


// zwraca nowa kopie struktury
struct raster *raster_clone(struct raster *r);


// zwraca wymiary obrazu
#define raster_dim_x(r)		( (r)->dim_x )
#define raster_dim_y(r)		( (r)->dim_y )


// makra sprawdzajace czy x/y jest w przedziale
#define raster_x_rng(r, x)	( 0<=x && x<raster_dim_x(r) )
#define raster_y_rng(r, y)	( 0<=y && y<raster_dim_y(r) )
#define raster_xy_rng(r, x, y)	( raster_x_rng(r, x) && raster_y_rng(r, y) )



// zamienia obraz na odcienie szarosci
struct raster *raster_grayscale(struct raster *r);

// odwraca kolory obrazka
struct raster *raster_inverse_colors(struct raster *r);


// zapis do bitmapy
struct bmp *raster_to_bmp(struct raster *r);

// odczyt z bitmapy
struct raster *raster_from_bmp(struct bmp *p);



// zamienia pozycje z postaci [x;y] na offset tablicy
#define raster_xy2off(r,x,y)	( (r)->dim_y*(x)+(y) )

// ustawia dany pixel [x;y] na zadana wartosc
#define raster_pix_set(r,x,y,p) \
			( (r)->pixs[raster_xy2off(r,x,y)]=*(p) )
// ustawia skladowa R
#define raster_pix_set_r(ra,x,y,red)	\
			( (ra)->pixs[raster_xy2off(ra,x,y)].r=(red)   )
// ustawia skladowa G
#define raster_pix_set_g(ra,x,y,green)	\
			( (ra)->pixs[raster_xy2off(ra,x,y)].g=(green) )
// ustawia skladowa B
#define raster_pix_set_b(ra,x,y,blue)	\
			( (ra)->pixs[raster_xy2off(ra,x,y)].b=(blue)  )



// zwraca warotsc danego pixela
#define raster_pix_get(r,x,y)	\
			( (r)->pixs[raster_xy2off(r,x,y)] )
// zwraca skladowa R
#define raster_pix_get_r(ra,x,y)	\
			( (ra)->pixs[raster_xy2off(ra,x,y)].r )
// zwraca skladowa G
#define raster_pix_get_g(ra,x,y)	\
			( (ra)->pixs[raster_xy2off(ra,x,y)].g )
// zwraca skladowa B
#define raster_pix_get_b(ra,x,y)	\
			( (ra)->pixs[raster_xy2off(ra,x,y)].b )


// zwraca raster powstaly na skutek odjecia a i b - wartosc odejmowania
// jest zwracana w postaci znormalizowanej [0;255], C=(A-B)
struct raster *raster_substract(struct raster *a, struct raster *b);

// zwraca raster bedacy suma 2 wejsciowych C=(A+B)
struct raster *raster_add(struct raster *a, struct raster *b);

// zwraca raster bedacy iloczynem 2 wejsciowych C=(A*B)
struct raster *raster_and(struct raster *a, struct raster *b);


//
// marker i operacje na nim
//

// dodaje marker do obrazka. operacje NIE powiedzie sie, jesli 
// jakis marker juz istnieje!
int raster_marker_create(struct raster *r);

// zwalnia marker, jezeli takowy zostal przydzielony
int raster_marker_free(struct raster *r);


// pokazuje na konkretny element marker'a - raczej nie nalezy uzywac
// tego poza tym modulem.
#define marker_elem(r, x,y)	( (r)->marker[ raster_xy2off(r, x,y) ] )

// ustawia marker z pozycji na zadana wartosc
#define raster_marker_set(r, x,y, v)	( marker_elem(r, x,y)=(v) )

// zwraca marker z danej pozycji
#define raster_marker_get(r, x,y)	( marker_elem(r, x,y) )


#endif

