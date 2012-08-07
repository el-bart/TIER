#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <curses.h>

#include "tier_io/tier_io.h"

// COLOR-PAIRs
#define CP_NORM		1
#define CP_OK		2
#define CP_ERR		3

// pozycja na jakiej wypisujemy komendy
#define WR		22
#define WC		0


void print_stat(struct tier_io *tio)
{
 if( tier_io_handler_ok(&tio->h) )
 {
   color_set(CP_OK,   NULL);
   wprintw(stdscr, "ok\n");
 }
 else
 {
   color_set(CP_ERR,  NULL);
   wprintw(stdscr, "ERROR (no/incorrect response)\n");
 };
 color_set(CP_NORM, NULL);
}; // print_stat()




int main(void)
{
 // struktury wewnetrzne:
 struct tier_io  *tio;

 // nastawy gasiennic:
 int              t_left;
 int              t_right;

 // interakcja z izytkownikiem:
 char             quit;
 const char       help[]="\
****************************\n\
b,t,g  quiT (Break)\n\
space,\n\
 enter both tracks stop\n\
h,F1   this Help screen\n\
\n\
q      left track forward\n\
a      left track stop\n\
z      left track backward\n\
\n\
o      right track forward\n\
k      right track stop\n\
m      right track backward\n\
\n\
arrows:\n\
up     TIER forward\n\
down   TIER backward\n\
left   TIER left\n\
right  TIER right\n\
****************************\n\
";

 // podlaczamy sie do portu
 tio=tier_io_create(PORTIO_LPT0);
 assert(tio!=NULL);

 // ustalamy timeout... 0.5[s]
 tio->h.timeout.time   =0;	    // ustawiamy domyslny timeout
 tio->h.timeout.millitm=500;	// ...


 //
 // inicjalizacja ncurses
 //
 initscr();
 curs_set(0);			// kursor bedzie niewidoczny
 //sprawdzamy, czy mozemy uzywac kolorow:
 if( has_colors() )
 {
   if( start_color()==OK )
   {
     //tworzymy pary colorow CZCIONKA,TLO
     init_pair(CP_NORM,  COLOR_WHITE, COLOR_BLACK);
     init_pair(CP_OK,    COLOR_GREEN, COLOR_BLACK);
     init_pair(CP_ERR,   COLOR_RED,   COLOR_BLACK);
   }
 }
 // ustawiamy kolorystyke
 wattrset(stdscr, COLOR_PAIR(CP_NORM) );
 // parametry terminala
 noecho();		// piszemy bez echa
 keypad(stdscr, TRUE);	// wlaczamy rozszerzona obsluge klawiatury
 cbreak();		// piszemy bez buforowania (wazne!)

 wprintw(stdscr, "[ I ] -> init...\t");
 assert( tier_io_init(tio)==0 );
 print_stat(tio);

 // print help
 wprintw(stdscr, "\n%s\n", help);
 wrefresh(stdscr);



 //
 // interakcja z user'em
 //
 t_left =0;		// poczatkowo gasiennice stoja
 t_right=0;		// ...
 quit   =0;		// petla sie kreci. :)
 while(!quit)
 {
   int scr_width;	// do czyszczenia linii statusu
   int i;		// ...

   int key;		// przycisk wcisniety przez uzytkownika

   key=wgetch(stdscr);	// pobieramy znak (blokujaco)

   // czyscimy nasza linie statusowa:
   getmaxyx(stdscr, i,scr_width);	// pobieramy obecny wymiar okna
   for(i=0; i<scr_width; i++)
     mvwprintw(stdscr, WR,i, " ");

   // podejmujemy stosowna akcje w zaleznosci od tego co zostalo
   // wcisniete.
   switch(key)
   {

     case 'b':
     case 'B':
     case 't':
     case 'T':
     case 'g':
     case 'G':
          mvwprintw(stdscr, WR,WC, "[ Q ] -> quit (break)...\n");
          wrefresh(stdscr);
          // to jest dosc wazne, wiec wysylamy pare razy!
          // 1
          //assert( tier_io_stop(tio)==0 );
          tier_io_stop(tio);
          print_stat(tio);
          wrefresh(stdscr);
          // 2
          //assert( tier_io_stop(tio)==0 );
          tier_io_stop(tio);
          print_stat(tio);
          wrefresh(stdscr);
          // 3
          //assert( tier_io_stop(tio)==0 );
          tier_io_stop(tio);
          print_stat(tio);
          wrefresh(stdscr);
          // konczymy caly cykl!
          quit=1;
          break;

     case ' ':
     case 10:
     case 13:
     case KEY_ENTER:
          mvwprintw(stdscr, WR,WC, "[A S] -> all tracks stop...\t\t");
          wrefresh(stdscr);
          t_left =0;
          t_right=0;
          //assert( tier_io_stop(tio)==0 );
          assert( tier_io_spdst(tio, t_left, t_right)==0 );
          break;

     //
     // lewa gasiennica
     //

     case 'q':
     case 'Q':
          mvwprintw(stdscr, WR,WC, "[L F] -> left track forward...\t\t");
          wrefresh(stdscr);
          t_left=+250;
          assert( tier_io_spdst(tio, t_left, t_right)==0 );
          break;

     case 'a':
     case 'A':
          mvwprintw(stdscr, WR,WC, "[L S] -> left track stop...\t\t");
          wrefresh(stdscr);
          t_left=0;
          assert( tier_io_spdst(tio, t_left, t_right)==0 );
          break;

     case 'z':
     case 'Z':
          mvwprintw(stdscr, WR,WC, "[L B] -> left track backward...\t\t");
          wrefresh(stdscr);
          t_left=-250;
          assert( tier_io_spdst(tio, t_left, t_right)==0 );
          break;

     //
     // prawa gasiennica
     //

     case 'o':
     case 'O':
          mvwprintw(stdscr, WR,WC, "[R F] -> right track forward...\t\t");
          wrefresh(stdscr);
          t_right=+250;
          assert( tier_io_spdst(tio, t_left, t_right)==0 );
          break;

     case 'k':
     case 'K':
          mvwprintw(stdscr, WR,WC, "[R S] -> right track stop...\t\t");
          wrefresh(stdscr);
          t_right=0;
          assert( tier_io_spdst(tio, t_left, t_right)==0 );
          break;

     case 'm':
     case 'M':
          mvwprintw(stdscr, WR,WC, "[R B] -> right track backward...\t");
          wrefresh(stdscr);
          t_right=-250;
          assert( tier_io_spdst(tio, t_left, t_right)==0 );
          break;

     //
     // strzalki
     //

     case KEY_UP:
          mvwprintw(stdscr, WR,WC, "[a F] -> TIER forward...\t\t");
          wrefresh(stdscr);
          t_right=+250;
          t_left =+250;
          assert( tier_io_spdst(tio, t_left, t_right)==0 );
          break;

     case KEY_DOWN:
          mvwprintw(stdscr, WR,WC, "[a B] -> TIER backward...\t\t");
          wrefresh(stdscr);
          t_right=-250;
          t_left =-250;
          assert( tier_io_spdst(tio, t_left, t_right)==0 );
          break;

     case KEY_LEFT:
          mvwprintw(stdscr, WR,WC, "[a L] -> TIER left...\t\t\t");
          wrefresh(stdscr);
          t_right=+250;
          t_left =-250;
          assert( tier_io_spdst(tio, t_left, t_right)==0 );
          break;

     case KEY_RIGHT:
          mvwprintw(stdscr, WR,WC, "[a R] -> TIER right...\t\t\t");
          wrefresh(stdscr);
          t_right=-250;
          t_left =+250;
          assert( tier_io_spdst(tio, t_left, t_right)==0 );
          break;

     // domyslnie zaznaczamy ze przycisk nie jest znany
     default:
          color_set(CP_ERR,  NULL);
          mvwprintw(stdscr, WR,WC, "unknown key pressed");
          color_set(CP_NORM, NULL);
          key=-1;
          break;
/*
     //
     // domyslnie pokazujemy help'a
     // 

     case KEY_F(1):
     case 'h':
     case 'H':
     default:
          wprintw(stdscr, "\nsupported keys are:\n");
          wprintw(stdscr, "\n%s\n", help);
          break;
*/

   }; // switch(key_pressed)

   // wypisujemy status wykonania rozkazu
   if( key!=-1 && !quit )
     print_stat(tio);

   // odswierzamy ekran
   wrefresh(stdscr);

 }; // while(!quit)



 //
 // konczymy prace
 //

 // tier:
 tier_io_free(tio);

 // ncurses:
 wprintw(stdscr, "\n\n");
 use_default_colors();
 endwin();

 return 0;
}; // main()

