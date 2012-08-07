#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <curses.h>
#include <stdlib.h>

#include "tier_io_client.h"


// COLOR-PAIRs
#define CP_NORM		1
#define CP_OK		2
#define CP_ERR		3

// pozycja na jakiej wypisujemy komendy
#define WR		22
#define WC		0


void print_stat(int ret_code)
{
 if(ret_code==0)
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




int main(int argc, char *argv[])
{
 // struktury wewnetrzne:
 struct tier_io_client *tio_cln;

 // nastawy gasiennic:
 int        t_left;
 int        t_right;

 // interakcja z izytkownikiem:
 char       quit;
 const char help[]="\
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

 if(argc!=3)
 {
   fprintf(stderr, "%s <srv_addr> <port>\n", argv[0]);
   return 1;
 };

 // podlaczamy sie do portu
 tio_cln=tier_io_client_create(argv[1], atoi(argv[2]) );
 assert(tio_cln!=NULL);


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
 print_stat( tier_io_client_init(tio_cln) );

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
          print_stat( tier_io_client_stop(tio_cln) );
          wrefresh(stdscr);
          // 2
          print_stat( tier_io_client_stop(tio_cln) );
          wrefresh(stdscr);
          // 3
          print_stat( tier_io_client_stop(tio_cln) );
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
          print_stat( tier_io_client_set_speed(tio_cln, t_left, t_right) );
          break;

     //
     // lewa gasiennica
     //

     case 'q':
     case 'Q':
          mvwprintw(stdscr, WR,WC, "[L F] -> left track forward...\t\t");
          wrefresh(stdscr);
          t_left=+250;
          print_stat( tier_io_client_set_speed(tio_cln, t_left, t_right) );
          break;

     case 'a':
     case 'A':
          mvwprintw(stdscr, WR,WC, "[L S] -> left track stop...\t\t");
          wrefresh(stdscr);
          t_left=0;
          print_stat( tier_io_client_set_speed(tio_cln, t_left, t_right) );
          break;

     case 'z':
     case 'Z':
          mvwprintw(stdscr, WR,WC, "[L B] -> left track backward...\t\t");
          wrefresh(stdscr);
          t_left=-250;
          print_stat( tier_io_client_set_speed(tio_cln, t_left, t_right) );
          break;

     //
     // prawa gasiennica
     //

     case 'o':
     case 'O':
          mvwprintw(stdscr, WR,WC, "[R F] -> right track forward...\t\t");
          wrefresh(stdscr);
          t_right=+250;
          print_stat( tier_io_client_set_speed(tio_cln, t_left, t_right) );
          break;

     case 'k':
     case 'K':
          mvwprintw(stdscr, WR,WC, "[R S] -> right track stop...\t\t");
          wrefresh(stdscr);
          t_right=0;
          print_stat( tier_io_client_set_speed(tio_cln, t_left, t_right) );
          break;

     case 'm':
     case 'M':
          mvwprintw(stdscr, WR,WC, "[R B] -> right track backward...\t");
          wrefresh(stdscr);
          t_right=-250;
          print_stat( tier_io_client_set_speed(tio_cln, t_left, t_right) );
          break;

     //
     // strzalki
     //

     case KEY_UP:
          mvwprintw(stdscr, WR,WC, "[a F] -> TIER forward...\t\t");
          wrefresh(stdscr);
          t_right=+250;
          t_left =+250;
          print_stat( tier_io_client_set_speed(tio_cln, t_left, t_right) );
          break;

     case KEY_DOWN:
          mvwprintw(stdscr, WR,WC, "[a B] -> TIER backward...\t\t");
          wrefresh(stdscr);
          t_right=-250;
          t_left =-250;
          print_stat( tier_io_client_set_speed(tio_cln, t_left, t_right) );
          break;

     case KEY_LEFT:
          mvwprintw(stdscr, WR,WC, "[a L] -> TIER left...\t\t\t");
          wrefresh(stdscr);
          t_right=+250;
          t_left =-250;
          print_stat( tier_io_client_set_speed(tio_cln, t_left, t_right) );
          break;

     case KEY_RIGHT:
          mvwprintw(stdscr, WR,WC, "[a R] -> TIER right...\t\t\t");
          wrefresh(stdscr);
          t_right=-250;
          t_left =+250;
          print_stat( tier_io_client_set_speed(tio_cln, t_left, t_right) );
          break;

     // domyslnie zaznaczamy ze przycisk nie jest znany
     default:
          color_set(CP_ERR,  NULL);
          mvwprintw(stdscr, WR,WC, "unknown key pressed");
          color_set(CP_NORM, NULL);
          print_stat(-1);
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

   // odswierzamy ekran
   wrefresh(stdscr);

 }; // while(!quit)



 //
 // konczymy prace
 //

 // tier:
 tier_io_client_free(tio_cln);

 // ncurses:
 wprintw(stdscr, "\n\n");
 use_default_colors();
 endwin();

 return 0;
}; // main()

