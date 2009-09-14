/*
 * TEST.cpp
 *
 */
#include <iostream>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

#include "TIERproto.hpp"
#include "ProtoLocal.hpp"

using namespace std;
using namespace CommProto;


// COLOR-PAIRs
#define CP_NORM		1
#define CP_OK		2
#define CP_ERR		3


void print_stat(bool status)
{
 if(status)
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




int main(const int argc, const char * const * const argv )
{
  try
  {
    ProtoLocal pl(ProtoLocal::LPT0);

    // nastawy gasiennic:
    int              t_left;
    int              t_right;

    // interakcja z izytkownikiem:
    char             quit;
    const char       help[]="****************************\n"
                            "b,t,g  quiT (Break)\n"
                            "space,\n"
                            "enter both tracks stop\n"
                            "h,F1   this Help screen\n"
                            "\n"
                            "q      left track forward\n"
                            "a      left track stop\n"
                            "z      left track backward\n"
                            "\n"
                            "o      right track forward\n"
                            "k      right track stop\n"
                            "m      right track backward\n"
                            "\n"
                            "arrows:\n"
                            "up     TIER forward\n"
                            "down   TIER backward\n"
                            "left   TIER left\n"
                            "right  TIER right\n"
                            "\n"
                            "r      refresh screen\n"
                            "p      ping TIER\n"
                            "****************************\n";

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
    print_stat( pl.init() );

    // print help
    wprintw(stdscr, "\n%s\n", help);
    wrefresh(stdscr);


    //
    // interakcja z uzyszkodnikiem
    //
    t_left =0;		// poczatkowo gasiennice stoja
    t_right=0;		// ...
    quit   =0;		// petla sie kreci. :)
    while(!quit)
    {
//      int scr_width;	// do czyszczenia linii statusu
//      int i;		// ...

      int key;		// przycisk wcisniety przez uzytkownika

      key=wgetch(stdscr);	// pobieramy znak (blokujaco)

      // czyscimy nasza linie statusowa:
//      getmaxyx(stdscr, i,scr_width);	// pobieramy obecny wymiar okna
//      for(i=0; i<scr_width; i++)
//        mvwprintw(stdscr, WR,i, " ");

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
          wprintw(stdscr, "[ Q ] -> quit (break)...\n");
          wrefresh(stdscr);
          // to jest dosc wazne, wiec wysylamy pare razy!
          for(int i=0; i<3; ++i)
          {
            pl.stop();
            print_stat(false);  // w zasadzie zawsze taki kod zwracany bedzie
            wrefresh(stdscr);
          };
          // konczymy caly cykl!
          quit=1;
          break;

        case ' ':
        case 10:
        case 13:
        case KEY_ENTER:
          wprintw(stdscr, "[A S] -> all tracks stop...\t\t");
          wrefresh(stdscr);
          t_left =0;
          t_right=0;
          print_stat( pl.spdst( TIERproto::Speed(t_left, t_right) ) );
          break;

          //
          // lewa gasiennica
          //

        case 'q':
        case 'Q':
          wprintw(stdscr, "[L F] -> left track forward...\t\t");
          wrefresh(stdscr);
          t_left=+250;
          print_stat( pl.spdst( TIERproto::Speed(t_left, t_right) ) );
          break;

        case 'a':
        case 'A':
          wprintw(stdscr, "[L S] -> left track stop...\t\t");
          wrefresh(stdscr);
          t_left=0;
          print_stat( pl.spdst( TIERproto::Speed(t_left, t_right) ) );
          break;

        case 'z':
        case 'Z':
          wprintw(stdscr, "[L B] -> left track backward...\t\t");
          wrefresh(stdscr);
          t_left=-250;
          print_stat( pl.spdst( TIERproto::Speed(t_left, t_right) ) );
          break;

          //
          // prawa gasiennica
          //

        case 'o':
        case 'O':
          wprintw(stdscr, "[R F] -> right track forward...\t\t");
          wrefresh(stdscr);
          t_right=+250;
          print_stat( pl.spdst( TIERproto::Speed(t_left, t_right) ) );
          break;

        case 'k':
        case 'K':
          wprintw(stdscr, "[R S] -> right track stop...\t\t");
          wrefresh(stdscr);
          t_right=0;
          print_stat( pl.spdst( TIERproto::Speed(t_left, t_right) ) );
          break;

        case 'm':
        case 'M':
          wprintw(stdscr, "[R B] -> right track backward...\t");
          wrefresh(stdscr);
          t_right=-250;
          print_stat( pl.spdst( TIERproto::Speed(t_left, t_right) ) );
          break;

          //
          // strzalki
          //

        case KEY_UP:
          wprintw(stdscr, "[a F] -> TIER forward...\t\t");
          wrefresh(stdscr);
          t_right=+250;
          t_left =+250;
          print_stat( pl.spdst( TIERproto::Speed(t_left, t_right) ) );
          break;

        case KEY_DOWN:
          wprintw(stdscr, "[a B] -> TIER backward...\t\t");
          wrefresh(stdscr);
          t_right=-250;
          t_left =-250;
          print_stat( pl.spdst( TIERproto::Speed(t_left, t_right) ) );
          break;

        case KEY_LEFT:
          wprintw(stdscr, "[a L] -> TIER left...\t\t\t");
          wrefresh(stdscr);
          t_right=+250;
          t_left =-250;
          print_stat( pl.spdst( TIERproto::Speed(t_left, t_right) ) );
          break;

        case KEY_RIGHT:
          wprintw(stdscr, "[a R] -> TIER right...\t\t\t");
          wrefresh(stdscr);
          t_right=-250;
          t_left =+250;
          print_stat( pl.spdst( TIERproto::Speed(t_left, t_right) ) );
          break;

          //
          // wysylamy pakiet init
          //
        case 'i':
        case 'I':
          wprintw(stdscr, "[t I] -> TIER init...\t\t\t");
          wrefresh(stdscr);
          print_stat( pl.init() );
          break;

          //
          // rozne
          //

          // czyscimy ekran
        case 'r':
        case 'R':
          wclear(stdscr);
          wrefresh(stdscr);
          break;

          // ping
        case 'p':
        case 'P':
          wprintw(stdscr, "[t P] -> ping TIER...\t\t\t");
          wrefresh(stdscr);
          print_stat( pl.ping() );
          break;

          //
          // domyslnie pokazujemy help'a
          //
        case KEY_F(1):
        case 'h':
        case 'H':
        default:
          color_set(CP_ERR,  NULL);
          wprintw(stdscr, "unknown key pressed\n");
          wprintw(stdscr, "\nsupported keys are:\n");
          wprintw(stdscr, "\n%s\n", help);
          color_set(CP_NORM, NULL);
          key=-1;
          break;

      }; // switch(key_pressed)

      // odswierzamy ekran
      wrefresh(stdscr);

    }; // while(!quit)



    //
    // konczymy prace
    //

    // ncurses:
    wprintw(stdscr, "\n\n");
    use_default_colors();
    endwin();

  }
  catch(const CommProto::Exception &ex)
  {
    cerr<<argv[0]<<": CommProto exception: "<<ex.what()<<endl;
  }
  catch(...)
  {
    cerr<<argv[0]<<": unknown exception cought"<<endl;
  };

  return 0;
};

