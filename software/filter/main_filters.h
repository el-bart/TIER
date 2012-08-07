/*
 * main_filters.h
 *
 * modul implementujacy filtr na bibliotece "raster".
 * istnieje mozliwosc korzystania z predefiniowanych
 * filtrow, jak i tworzenie wlasnych.
 *
 */
#ifndef INCLUDE_MAIN_FILTERS_H_FILE
#define INCLUDE_MAIN_FILTERS_H_FILE

#include "raster/raster.h"
#include "req_col_unif.h"


// mozliwe odmiany filtra Frei-Chen
#define FILTER_FREI_CHEN_HORIZONTAL	-1
#define FILTER_FREI_CHEN_VERTICAL	 1



// ogolna postac filtru o dowolnym rozmiarze
struct filter
{
 // wymiary:
 int dim_x;		// rozmiar X-owy filtru
 int dim_y;		// rozmiar Y-owy filtru

 // pozycja srodka:
 int cent_x;		// X srodka filtru
 int cent_y;		// Y srodka filtru

 // maska wlasciwa:
 double *mask;		// maska filtru w postaci tablicy [x1;y1][x2;y1]...etc
 double  div;		// wspolczynnik podzialu
}; // struct filter






// makro zamienia pozycje X;Y w filtrze na offset w tablicy
#define filter_xy2off(f,x,y)	( (f)->dim_y*(x)+(y) )


// makra zwracajacy wymiary filtra
#define filter_dim_x(f)		( (f)->dim_x )
#define filter_dim_y(f)		( (f)->dim_y )


// makra zwracajace srodek filtra
#define filter_cent_x(f)	( (f)->cent_x )
#define filter_cent_y(f)	( (f)->cent_y )


// zwraca wspolczynnik podzialu
#define filter_div(f)		( (f)->div )


// zwraca wspolczynnik maski dla danego pixela filtra
#define filter_get_mask_p(f, x, y)	( (f)->mask[ filter_xy2off(f, x, y) ] )

// ustawia wspolczynnik maski dla danego pixela filtra
#define filter_set_mask_p(f, x, y, v)	\
			( (f)->mask[ filter_xy2off(f, x, y) ]=(v) )


//
// wewnetrze funkcje modulu:
//

// funkcja stosuje filtr na danym obrazie i zwraca NOWY obraz
//struct raster *filter_raster(struct filter *f, struct raster *r);

// funkcja zwalnia pamiec zajmowana przez filtr
//int filter_free(struct filter *f);





//
// dostepne filtry
//

// zwraca obraz po przepuszczeniu przez filtr medianowy
//   efekt podstawowy: usuniecie drobnego szumu
//   efekt uboczny:    rozmycie obrazu
struct raster *filter_median(struct raster *r);


// zwraca obraz po przepuszeniu przez filtr Laplace'a
//   efekt podstawowy: zwiekszenie ostrosci
//   efekt uboczny:    pojawienie sie szumu 'sol i pieprz'
struct raster *filter_laplace(struct raster *r);


// zwraca obraz po zmianie jasnosci o zadana wartosc [-255;+255]
//   efekt podstawowy: rozjasnienie/sciemnienie
//   efekt uboczny:    mozliwa utrata czesci informacji
struct raster *filter_ch_bri(struct raster *r, int ch_val);


// zwraca obraz po operacji odciecia na poziomie [0;255] (wszystkie
// wartosci ponizej zadanej sa ustawiane jako czern, pozotsle na biel)
// poziom odciecia mozna wymusic na automatyczny, podajac level=='-1'
//   efekt podstawowy: binaryzacja obrazu
//   efekt uboczny:    utrata/znieksztalcenie informacji
struct raster *filter_cut_off(struct raster *r, int level);


// zwraca obraz po operacji dylatacji
//   efekt podstawowy: pogrobia krawedzie
//   efekt uboczny:    gubi detale
struct raster *filter_dilatation(struct raster *r);


// zwraca obraz po operacji erozji
//   efekt podstawowy: usowa "niewyrazne" linie
//   efekt uboczny:    gubi szczegoly
struct raster *filter_erosion(struct raster *r);


// mozliwe odmiany filtra Frei-Chen
#define FILTER_FREI_CHEN_HORIZONTAL	-1
#define FILTER_FREI_CHEN_VERTICAL	 1

// zwraca obraz po przefiltrowaniu filtrem Frei-Chen
// mode={FILTER_FREI_CHEN_HORIZONTAL,FILTER_FREI_CHEN_VERTICAL}
//   efekt podstawowy: wykrywanie krawedzi
//   efekt uboczny:    znikaja detale
struct raster *filter_frei_chen(struct raster *r, int mode);


// zwraca obraz po przepuszczeniu przez filtr rozmywajacy
// (w pozycje bierzaca wpisuje wartosc srednia z pixeli otoczenia)
//   efekt podstawowy: usuniecie drobnego szumu
//   efekt uboczny:    rozmycie obrazu, sciagniecie histogramu
struct raster *filter_blur(struct raster *r);


// zwaraca obraz po otwarciu (obraz->erozja->dylatacja)
//   efekt podstawowy: zostawia tylko wieksze obiekty
//   efekt uboczny:    gubi szczegoly
struct raster *filter_opening(struct raster *r);


// zwaraca obraz po domknieciu (obraz->dylatacja->erozja)
//   efekt podstawowy: zostawia tylko wieksze obiekty
//   efekt uboczny:    gubi szczegoly
struct raster *filter_closing(struct raster *r);


// odchudza krawedzi an obrazie binarnym
//   efekt podstawowy: krawedzie pozostaja 1-pixelowe
//   efekt uboczny:    zmiana ksztaltow niektorych linii...
struct raster *filter_edge_thin(struct raster *r);


// wyostrzanie obrazu metoda Laplace'a
//   efekt podstawowy: poprawa ostrosci
//   efekt uboczny:    uwyraznienie szumu
//struct raster *filter_sharpen(struct raster *r);



#endif

