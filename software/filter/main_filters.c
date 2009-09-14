/*
 * filter.c
 *
 */
#include <stdlib.h>
#include <assert.h>	// assert()
#include <math.h>	// sqrt()
#include <string.h>	// memset()
//#include <stdio.h>

#include "main_filters.h"
#include "raster/histogram.h"


// zwraca filtr o zadanych rozmiarach
struct filter *filter_create(int dim_x,  int dim_y,
                             int cent_x, int cent_y, double div)
{
 struct filter *f;

 // sprawdzamy sensownosc podanych parametrow
 if( dim_x<1  || dim_y<1       ||				// wymiary ok?
     cent_x<0 || dim_x<=cent_x || cent_y<0 || dim_y<=cent_y ||	// srodek ok?
     div==0.0 )							// div!=0.0?
   return NULL;
     

 f=(struct filter*)malloc( sizeof(struct filter) );
 if(f==NULL)
   return NULL;

 f->dim_x =dim_x;
 f->dim_y =dim_y;
 f->cent_x=cent_x;
 f->cent_y=cent_y;
 f->div   =div;

 // przydzielamy pamiec na maske
 f->mask=(double*)malloc( sizeof(double)*dim_x*dim_y );
 if(f->mask==NULL)
 {
   free(f);
   return NULL;
 };

 return f;
}; // filter_create()




#define norm_color(c)	( ((c)<0)?0.0:(((c)>255)?255.0:(c)) )


// funkcja zwraca nowa wartosc pixela po przfiltrowaniu
// zadana maska
struct rgb24 filter_pixel(struct filter *f, struct raster *r, int x_p, int y_p)
{
 struct rgb24 p_out;		// pixel wyjsciowy
 struct rgb24 p_null={0,0,0};	// kolor "poza" obrazem
 double       color_r;		// jakis kolor - pomocnicza zmienna
 double       color_g;		// ...
 double       color_b;		// ...
 int          x,y;		// for()
 int          x_off_min, x_off_max;	// zmiennosc x (wzgledem srodka filtra)
 int          y_off_min, y_off_max;	// zmiennosc y (j/w)
 int          x_inc, y_inc;		// x/y do maski filtra :)

 assert( raster_xy_rng(r, x_p, y_p) );	// musimy miec pewnosc, ze tego mamy :)

 // wartosci poczatkowe:
 color_r  =0;			// poki co brak informajci o kolorze
 color_g  =0;			// ...
 color_b  =0;			// ...
 x_off_min=-filter_cent_x(f);			// offset poczatq X
 x_off_max= filter_dim_x(f)-filter_cent_x(f)-1;	// offset konca X
 y_off_min=-filter_cent_y(f);			// offset poczatq Y
 y_off_max= filter_dim_y(f)-filter_cent_y(f)-1;	// offset konca Y

 // iterujemy po elementach
 for(y=y_off_min, y_inc=0; y<=y_off_max; y++, y_inc++)
   for(x=x_off_min, x_inc=0; x<=x_off_max; x++, x_inc++)
   {
     struct rgb24 p_tmp;	// tymczasowy kolor
     double       w;		// waga do wymnozenia

     // pobieramy nasz kolor do analizy:
     if( raster_xy_rng(r, x_p+x, y_p+y) )	// pixel nalezy do rastra?
       p_tmp=raster_pix_get(r, x_p+x, y_p+y);	// pobieramy go wiec!
     else			// kolor NIE nalezy do rastra?
       p_tmp=p_null;		// ustawiamy wiec na domyslny!

     // nanosimy skladowe na zmienne z wagami:
     w       =filter_get_mask_p(f, x_inc, y_inc);
     color_r+=p_tmp.r*w;
     color_g+=p_tmp.g*w;
     color_b+=p_tmp.b*w;
   }; // for([x;y])

 // na koniec konwertujemy nasz kolor na skaladowe rgb
 //p_out.r=(unsigned char)( (color_r>0.0)?color_r:0.0 );
 //p_out.g=(unsigned char)( (color_g>0.0)?color_g:0.0 );
 //p_out.b=(unsigned char)( (color_b>0.0)?color_b:0.0 );
 color_r/=filter_div(f);
 color_g/=filter_div(f);
 color_b/=filter_div(f);
 p_out.r=(unsigned char)norm_color(color_r);
 p_out.g=(unsigned char)norm_color(color_g);
 p_out.b=(unsigned char)norm_color(color_b);

 return p_out;		// zwracamy utworzony pixel
}; // filter_pixel()




struct raster *filter_raster(struct filter *f, struct raster *r)
{
 struct raster *r_out;	// obraz wyjsciowy
 int            x,y;	// for([x;y])

 // tworzymy nowy obraz:
 r_out=raster_create( raster_dim_x(r), raster_dim_y(r) );
 if(r_out==NULL)
   return NULL;

 // nakladamy maske na poszczegolne pixele
 for(y=0; y<raster_dim_y(r); y++)
   for(x=0; x<raster_dim_x(r); x++)
   {
     struct rgb24 pix;			// pomocniczy pixel
     pix=filter_pixel(f, r, x, y);	// wyliczamy nowa wartosc
     raster_pix_set(r_out, x, y, &pix);	// i przypisujem na jego miejsce
   }; // for([x;y])

 return r_out;
}; // filter_raster()





int filter_free(struct filter *f)
{
 if(f==NULL)		// nie ma juz czego zwalniac
   return 0;

 free(f->mask);		// zwalniamy maske wlasciwa
 free(f);		// oraz strukture ja przechowujaca

 return 0;
}; // filter_free()







// pomocnicze makro dla filtru medianowego - zwraca liczbe, reprezentujaca
// kolor. jest to srednia wartosc kladowych RGB
#define to_grayscale(p)	( ( (p).r + (p).g + (p).b )/3 )


struct raster *filter_median(struct raster *r)
{
 struct raster *r_out;		// obraz wyjsciowy
 int            x,y;		// for()

 // tworzymy obraz wyjsciowy
 r_out=raster_create( raster_dim_x(r), raster_dim_y(r) );
 if(r_out==NULL)
   return NULL;

 // filtrowanie wlasciwe
 for(x=0; x<raster_dim_x(r); x++)
   for(y=0; y<raster_dim_y(r); y++)
   {
     struct rgb24 pix[9];	// pixele z otoczenia
     int          x_now,y_now;	// wartosci chwilowe, dla sasiedztwa
     int          i,j;		// for() sortowania

     // pobiermay parametry srodka:
     pix[4]=raster_pix_get(r, x, y);	// pobieramy nasz srodkowy pixel

     //
     // wpisujemy wartosci do tablicyc
     //
     for(y_now=-1; y_now<2; y_now++)
       for(x_now=-1; x_now<2; x_now++)
       {
         int x_r,y_r;			// pozycja w rastrze wejsciowym
         int poz;			// pozycja w tablicy
         poz=(y_now+1)*3 + (x_now+1);	// wyliczamy pozycje w tablicy
         x_r=x+x_now;			// oraz w rastrze
         y_r=y+y_now;			// ...

         if( !raster_xy_rng(r, x_r, y_r) )	// punkt poza obrazem?
           pix[poz]=pix[4];		// jak nie wiadomo co, to srodek! :)
         else
           pix[poz]=raster_pix_get(r, x_r, y_r);// ustawiamy na wartosc punktu

       }; // for([x_now;y_now])

     //
     // sorutujemy wyniki
     //
     for(i=0; i<9; i++)
     {
       int          swap   =0;	// dokonano jakiejs zamiany?
       int          max_poz=i;	// pozycja najmniejszego elementu
       struct rgb24 tmp_pix;	// pomocniczy pixel
       for(j=i+1; j<9; j++)	// nie trzeba przegladac wszystkiego!
         if( to_grayscale(pix[max_poz])<to_grayscale(pix[j]) )	// wiekszy?
         {
           swap   =1;		// cos mamy
           max_poz=j;		// nowa pozycja najmniejszego
         }; // if(jest_wiekszy)
       if(!swap)		// posortowano juz?
         break;

       // zmieniamy miejscami 2 elementy:
       tmp_pix     =pix[i];
       pix[i]      =pix[max_poz];
       pix[max_poz]=tmp_pix;
     }; // for(i)

     // ustawiamy wartosc srodkowa jako wyjscie
     raster_pix_set(r_out, x, y, &pix[4]);

   }; // for([x;y])

 return r_out;
}; // filter_median()





struct raster *filter_laplace(struct raster *r)
{
 struct raster *r_tmp;		// obrazm pomocniczy
 struct raster *r_out;		// obraz wyjsciowy
 struct filter *f;		// nasz filtr

 //
 // tworzymy filtr Laplace'a
 //
 //   -1 -1 -1
 //   -1  8 -1
 //   -1 -1 -1
 //

 f=filter_create(3,3, 1,1, -5.0);	// tworzymy nowy filtr

 //
 // wypelnianie filtra:
 //                   x,y
 filter_set_mask_p(f, 0,0, -1.0);
 filter_set_mask_p(f, 1,0, -1.0);
 filter_set_mask_p(f, 2,0, -1.0);

 filter_set_mask_p(f, 0,1, -1.0);
 filter_set_mask_p(f, 1,1,  8.0);
 filter_set_mask_p(f, 2,1, -1.0);

 filter_set_mask_p(f, 0,2, -1.0);
 filter_set_mask_p(f, 1,2, -1.0);
 filter_set_mask_p(f, 2,2, -1.0);

 //
 // obrobka wlasciwa:
 //
 r_tmp=filter_raster(f, r);		// filtracja wlasciwa
 r_out=raster_substract(r, r_tmp);	// odejmujemy obraz wejsciowy
					// od uzyskanego po przefitrowaniu

 raster_free(r_tmp);		// zwalniamy obrazek pomocniczy
 filter_free(f);		// zwalniamy pamiec zajmowana przez filtr
 return r_out;			// zwracamy wynikowy obraz
}; // filter_laplace()






// odpowiednio "odcina" kolor, aby sie miescil w zadanych granicach
#define cut_color(c)	( ((c)<0)?0:( ((c)>255)?255:(c) ) )

struct raster *filter_ch_bri(struct raster *r, int ch_val)
{
 struct raster *r_out;		// obraz wyjsciowy
 int            x,y;		// for()

 // tworzymy pusty obraz wyjsciwy:
 r_out=raster_create( raster_dim_x(r), raster_dim_y(r) );
 if(r_out==NULL)
   return NULL;

 // przetwazamy obrazu
 for(y=0; y<raster_dim_y(r); y++)
   for(x=0; x<raster_dim_x(r); x++)
   {
     struct rgb24 pix;			// pomocniczy pixel
     int          tmp;			// pomocnicza wartosc

     pix  =raster_pix_get(r, x, y);	// pobieramy pixel
     tmp  =pix.r+ch_val;		// R
     pix.r=cut_color(tmp);		// ...
     tmp  =pix.g+ch_val;		// G
     pix.g=cut_color(tmp);		// ...
     tmp  =pix.b+ch_val;		// B
     pix.b=cut_color(tmp);		// ...

     raster_pix_set(r_out, x, y, &pix);	// wstawiamy pixel do obrazu
   }; // for([x;y])

 return r_out;			// zwracamy wynik
}; // filter_ch_bri()





struct raster *filter_cut_off(struct raster *r, int level)
{
 struct raster *r_out;				// obraz wyjsciowy
 struct rgb24   p_black={0x00, 0x00, 0x00};	// czarny pixel
 struct rgb24   p_white={0xFF, 0xFF, 0xFF};	// bialy pixel
 int            x,y;				// for()

 // sprawdzamy, czy odciecie ma byc robione automatycznie
 // na podstawie wejsciowego histogramu?
 if(level==-1)
 {
   struct histogram h;			// nasz histogram do analizy
   h    =raster_histogram_get(r, HISTOGRAM_DEFAULT);
   h    =raster_histogram_smooth(h);	// wygladzamy histogram
   level=raster_histogram_threshold_get(h);
//fprintf(stderr, "THRESHOLD FOUND IS @ %d\n", level);
 }; // if(auto_histogram_threshold)

 // tworzymy pusty obraz wyjsciwy:
 r_out=raster_create( raster_dim_x(r), raster_dim_y(r) );
 if(r_out==NULL)
   return NULL;

 // przetwazamy obraz
 for(y=0; y<raster_dim_y(r); y++)
   for(x=0; x<raster_dim_x(r); x++)
   {
     struct rgb24 pix;			// pomocniczy pixel

     pix=raster_pix_get(r, x, y);	// pobieramy pixel
     if( (pix.r+pix.g+pix.b)/3<=level )	// nie miesci sie w przedziale?
       raster_pix_set(r_out, x, y, &p_black);	// czarny
     else
       raster_pix_set(r_out, x, y, &p_white);	// bialy
   }; // for([x;y])

 return r_out;			// zwracamy wynik
}; // filter_cut_off()





struct raster *filter_dilatation(struct raster *r)
{
 struct raster *r_out;		// obraz wyjsciowy
 struct filter *f;		// nasz filtr

 //
 // tworzymy filtr
 //
 //   0 1 0
 //   1 1 1
 //   0 1 0
 //

 f=filter_create(3,3, 1,1, 1.0);	// tworzymy nowy filtr

 //
 // wypelnianie filtra:
 //                   x,y
 filter_set_mask_p(f, 0,0, 0.0);
 filter_set_mask_p(f, 1,0, 1.0);
 filter_set_mask_p(f, 2,0, 0.0);

 filter_set_mask_p(f, 0,1, 1.0);
 filter_set_mask_p(f, 1,1, 1.0);
 filter_set_mask_p(f, 2,1, 1.0);

 filter_set_mask_p(f, 0,2, 0.0);
 filter_set_mask_p(f, 1,2, 1.0);
 filter_set_mask_p(f, 2,2, 0.0);

 //
 // obrobka wlasciwa:
 //
 r_out=filter_raster(f, r);		// filtracja wlasciwa

 filter_free(f);		// zwalniamy pamiec zajmowana przez filtr
 return r_out;			// zwracamy wynikowy obraz
}; // filter_dilatation()




struct raster *filter_erosion(struct raster *r)
{
 struct raster *r_tmp;		// obraz pomocniczy
 struct raster *r_out;		// obraz wyjsciowy
 struct filter *f;		// nasz filtr

 //
 // tworzymy filtr
 //
 //   0 1 0
 //   1 1 1
 //   0 1 0
 //

 f=filter_create(3,3, 1,1, 5.0);	// tworzymy nowy filtr

 //
 // wypelnianie filtra:
 //                   x,y
 filter_set_mask_p(f, 0,0, 0.0);
 filter_set_mask_p(f, 1,0, 1.0);
 filter_set_mask_p(f, 2,0, 0.0);

 filter_set_mask_p(f, 0,1, 1.0);
 filter_set_mask_p(f, 1,1, 1.0);
 filter_set_mask_p(f, 2,1, 1.0);

 filter_set_mask_p(f, 0,2, 0.0);
 filter_set_mask_p(f, 1,2, 1.0);
 filter_set_mask_p(f, 2,2, 0.0);

 //
 // obrobka wlasciwa:
 //
 r_tmp=filter_raster(f, r);		// filtracja wlasciwa
 r_out=filter_cut_off(r_tmp, 254);	// zachowujemy tylko "mieszczace sie"

 raster_free(r_tmp);
 filter_free(f);		// zwalniamy pamiec zajmowana przez filtr

 return r_out;			// zwracamy wynikowy obraz
}; // filter_erosion()





struct raster *filter_frei_chen(struct raster *r, int mode)
{
 struct raster *r_out;		// obraz wyjsciowy
 struct filter *f;		// nasz filtr
 double         s;		// sqrt(2)

 if( mode!=FILTER_FREI_CHEN_HORIZONTAL &&
     mode!=FILTER_FREI_CHEN_VERTICAL      )
   return NULL;

 //
 // tworzymy filtry Frei-Chen'a
 //
 // s1 (horizontal):
 //   -1 -s -1
 //    0  0  0
 //    1  s  1
 //
 // s2 (vertical):
 //   -1  0  1
 //   -s  0  s
 //   -1  0  1
 //
 // gdzie s=sqrt(2)
 //

 f=filter_create(3,3, 1,1, 1.0);	// tworzymy nowy filtr
 s=sqrt(2.0);

 if(mode==FILTER_FREI_CHEN_HORIZONTAL)
 {
   //
   // wypelnianie filtra s1:
   //                   x,y
   filter_set_mask_p(f, 0,0, -1.0);
   filter_set_mask_p(f, 1,0, -s  );
   filter_set_mask_p(f, 2,0, -1.0);

   filter_set_mask_p(f, 0,1,  0.0);
   filter_set_mask_p(f, 1,1,  0.0);
   filter_set_mask_p(f, 2,1,  0.0);

   filter_set_mask_p(f, 0,2,  1.0);
   filter_set_mask_p(f, 1,2,  s  );
   filter_set_mask_p(f, 2,2,  1.0);
 }
 else	// FILTER_FREI_CHEN_VERTICAL
 {
   //
   // wypelnianie filtra s2:
   //                   x,y
   filter_set_mask_p(f, 0,0, -1.0);
   filter_set_mask_p(f, 1,0,  0.0);
   filter_set_mask_p(f, 2,0,  1.0);

   filter_set_mask_p(f, 0,1, -s  );
   filter_set_mask_p(f, 1,1,  0.0);
   filter_set_mask_p(f, 2,1,  s  );

   filter_set_mask_p(f, 0,2, -1.0);
   filter_set_mask_p(f, 1,2,  0.0);
   filter_set_mask_p(f, 2,2,  1.0);
 }; // if(mode)

 //
 // obrobka wlasciwa:
 //
 r_out=filter_raster(f, r);

 filter_free(f);		// zwalniamy pamiec zajmowana przez filtr
 return r_out;			// zwracamy wynikowy obraz
}; // filter_frei_chen()






struct raster *filter_blur(struct raster *r)
{
 struct raster *r_out;		// obraz wyjsciowy
 int            x,y;		// for()

 // tworzymy obraz wyjsciowy
 r_out=raster_create( raster_dim_x(r), raster_dim_y(r) );
 if(r_out==NULL)
   return NULL;

 // filtrowanie wlasciwe
 for(x=0; x<raster_dim_x(r); x++)
   for(y=0; y<raster_dim_y(r); y++)
   {
     struct rgb24 pix[9];	// pixele z otoczenia
     struct rgb24 pix_out;	// pixel wyjsciowy
     int          x_now,y_now;	// wartosci chwilowe, dla sasiedztwa

     // pobiermay parametry srodka:
     pix[4]=raster_pix_get(r, x, y);	// pobieramy nasz srodkowy pixel

     //
     // wpisujemy wartosci do tablicyc
     //
     for(y_now=-1; y_now<2; y_now++)
       for(x_now=-1; x_now<2; x_now++)
       {
         int x_r,y_r;			// pozycja w rastrze wejsciowym
         int poz;			// pozycja w tablicy
         poz=(y_now+1)*3 + (x_now+1);	// wyliczamy pozycje w tablicy
         x_r=x+x_now;			// oraz w rastrze
         y_r=y+y_now;			// ...

         if( !raster_xy_rng(r, x_r, y_r) )	// punkt poza obrazem?
           pix[poz]=pix[4];		// jak nie wiadomo co, to srodek! :)
         else
           pix[poz]=raster_pix_get(r, x_r, y_r);// ustawiamy na wartosc punktu

       }; // for([x_now;y_now])

     // wyliczamy wartosc srednia
     pix_out.r=( pix[0].r + pix[1].r + pix[2].r+
                 pix[3].r + pix[4].r + pix[5].r+
                 pix[6].r + pix[7].r + pix[8].r )/9;	// Red
     pix_out.g=( pix[0].g + pix[1].g + pix[2].g+
                 pix[3].g + pix[4].g + pix[5].g+
                 pix[6].g + pix[7].g + pix[8].g )/9;	// Green
     pix_out.b=( pix[0].b + pix[1].b + pix[2].b+
                 pix[3].b + pix[4].b + pix[5].b+
                 pix[6].b + pix[7].b + pix[8].b )/9;	// Blue
     

     // ustawiamy wartosc srodkowa jako wyjscie
     raster_pix_set(r_out, x,y, &pix_out);

   }; // for([x;y])

 return r_out;
}; // filter_blur()





struct raster *filter_opening(struct raster *r)
{
 struct raster *r_tmp;		// obraz pomoczniczy
 struct raster *r_out;		// obraz wyjsciowy

 r_tmp=filter_erosion(r);	// erozja
 r_out=filter_dilatation(r_tmp);// dylatacja

 raster_free(r_tmp);		// zwalniamy pomoczniczy obraz

 return r_out;
}; // filter_opening()




struct raster *filter_closing(struct raster *r)
{
 struct raster *r_tmp;		// obraz pomoczniczy
 struct raster *r_out;		// obraz wyjsciowy

 r_tmp=filter_dilatation(r);	// dylatacja
 r_out=filter_erosion(r_tmp);	// erozja

 raster_free(r_tmp);		// zwalniamy pomoczniczy obraz

 return r_out;
}; // filter_closing()




// makra sprawdzajace kolory
#define pix_is_white(p)	( (p).r==255 && (p).g==255 && (p).b==255 )
#define pix_is_black(p)	( (p).r==  0 && (p).g==  0 && (p).b==  0 )
// oraz ich skrocone wersje :)
#define piw(p)		( pix_is_white(p) )
#define pib(p)		( pix_is_black(p) )

// sprawdza czy 2 kolory sa identyczne
#define peq(p1,p2)	( (p1).r==(p2).r && (p1).g==(p2).g && (p1).b==(p2).b )


// zwraca prawde jezeli pixel kwalifikuje sie do usuniecia
static int check_rm_pix(struct raster *r, int x, int y, struct rgb24 pix)
{
 struct rgb24 p[9];		// otoczenie pixela pix:
				//
				//   0 3 6
				//   1 4 7
				//   2 5 8

 int          i,j;		// for()
 int          xc,yc;		// bierzace x,y
 int          poz;		// pozycja w tablicy
 struct rgb24 white={255,255,255};	// domyslnie bialy

 // najpierw wypelniamy tablice
 poz=0;
 for(i=-1; i<=1; i++)
   for(j=-1; j<=1; j++)
   {
     xc=x+i;			// wyliczamy bierzaca pozycje
     yc=y+j;			// ...
     if( raster_xy_rng(r, xc,yc) )	// pixel nalezy do obrazu?
       p[poz]=raster_pix_get(r, xc,yc);
     else
       p[poz]=white;
     poz++;			// przechodzimy na kolejna pozycje
   };

 //
 // teraz sprawdzamy czy zachodzi ktorys z warunkow
 //

 // poziome linie
 if( peq(p[0],p[3]) && peq(p[3],p[6]) &&
     peq(p[2],p[5]) && peq(p[5],p[8]) &&
     !peq(p[0],p[2]) )
   return 1;
 // pionowe linie
 if( peq(p[0],p[1]) && peq(p[1],p[2]) &&
     peq(p[6],p[7]) && peq(p[7],p[8]) &&
     !peq(p[0],p[6]) )
   return 1;
 // kostka L-G
 if( piw(p[0]) && piw(p[1]) &&         1 &&
     piw(p[3]) &&         1 && pib(p[5]) &&
             1 && pib(p[7]) && pib(p[8]) )
   return 1;
 // kostka L-D
 if(         1 && pib(p[1]) && pib(p[2]) &&
     piw(p[3]) &&         1 && pib(p[5]) &&
     piw(p[6]) && piw(p[7]) &&         1 )
   return 1;
 // kostka P-D
 if( pib(p[0]) && pib(p[1]) &&         1 &&
     pib(p[3]) &&         1 && piw(p[5]) &&
             1 && piw(p[7]) && piw(p[8]) )
   return 1;
 // kostka P-G
 if(         1 && piw(p[1]) && piw(p[2]) &&
     pib(p[3]) &&         1 && piw(p[5]) &&
     pib(p[6]) && pib(p[7]) &&         1 )
   return 1;

 // jezeli zaden z przypadqw nie zaszedl, to pixel zostaje!
 return 0;
}; // check_rm_pix()



struct raster *filter_edge_thin(struct raster *r)
{
 int            dim_x,dim_y;	// wymiary obrazka
 int            x,y;		// for()
 int            changed;	// czy cos sie zmienilo?
 struct raster *r_out;		// obraz wyjsciowy
 struct rgb24   pix;		// bierzacy pixel
 struct rgb24   black={0,0,0};	// czarny pixel

 dim_x=raster_dim_x(r);
 dim_y=raster_dim_y(r);

 r_out=filter_cut_off(r, 127);	// i tak potrzebujemy kopie a przynajmniej
				// bedziemy mieli pewnosc, ze obraz binarny!

 do
 {
   changed=0;			// domyslnie nic nie uleglo zmianie

   // glowna petla przegladania obrazu
   for(x=0; x<dim_x; x++)
     for(y=0; y<dim_y; y++)
     {
       pix=raster_pix_get(r_out, x,y);		// pobieramy bierzacy punkt

       if( pix_is_black(pix) )			// czarne punkty pomijamy
         continue;
       if( !check_rm_pix(r_out, x,y, pix) )	// sprawdzamy czy NIE usuwac?
         continue;

       raster_pix_set(r_out, x,y, &black);	// wiec jednak usuwamy...
       changed=1;				// zaznaczamy ten fakt
     }; // for([x;y])

 }
 while(changed);		// odchudzamy dopoki cos sie zmienia

 return r_out;
}; // filter_edge_thin()



/*
struct raster *filter_sharpen(struct raster *r)
{
 struct raster *r_out;		// obraz wyjsciowy
 struct filter *f;		// nasz filtr

 //
 // tworzymy filtr
 //
 //   -1 -1 -1
 //   -1  8 -1
 //   -1 -1 -1
 //

 f=filter_create(3,3, 1,1, 1.0);	// tworzymy nowy filtr

 //
 // wypelnianie filtra:
 //                   x,y
 filter_set_mask_p(f, 0,0, -1.0);
 filter_set_mask_p(f, 1,0, -1.0);
 filter_set_mask_p(f, 2,0, -1.0);

 filter_set_mask_p(f, 0,1, -1.0);
 filter_set_mask_p(f, 1,1,  8.0);
 filter_set_mask_p(f, 2,1, -1.0);

 filter_set_mask_p(f, 0,2, -1.0);
 filter_set_mask_p(f, 1,2, -1.0);
 filter_set_mask_p(f, 2,2, -1.0);

 //
 // obrobka wlasciwa:
 //
 r_out=filter_raster(f, r);		// filtracja wlasciwa

 filter_free(f);		// zwalniamy pamiec zajmowana przez filtr

 return r_out;			// zwracamy wynikowy obraz
}; // filter_sharpen()
*/

