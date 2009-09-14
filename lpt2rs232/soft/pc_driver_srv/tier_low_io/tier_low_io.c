#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "tier_low_io.h"


//
// bity portu LPT uzywane do komunikacji
//

// control:
#define TIER_PC_DATA_RDY	(1<<0)
#define TIER_PC_DATA_ACC	(1<<1)
// status:
#define TIER_uC_DATA_RDY	(1<<6)
#define TIER_uC_DATA_ACC	(1<<7)


//
// FUNKCJE LOKALNE
//

// PortSTATE
struct pstate
{
 char pc_rdy;
 char pc_acc;

 char uc_rdy;
 char uc_acc;

 char st;		// w/w pozycje w postaci 1 liczby (adres kom.)

 char rd_mode;		// czy tryb odczytu?
};


// zwraca stan w zwartej postaci
#define mk_st(pc_rdy,pc_acc, uc_rdy,uc_acc)	\
		( (pc_rdy)*(1<<3) + \
	          (pc_acc)*(1<<2) + \
	          (uc_rdy)*(1<<1) + \
	          (uc_acc)*(1<<0)     )

// czy stan jest stanem wymagajacym trybu zapisu?
#define needs_write(st)		( (st&(15-2))==(15-2)   )
// czy stan jest stanem w ktorym mozemy odczytac dane?
#define has_new_data(st)	( (st&(15-8))==(15-8-2) )


// zamienia st na pojedyncze flagi
static int decode_st(struct pstate *ps, char st)
{
 assert( ps!=NULL );

 if(st>=16)
   return -1;

 // pojedyncze flagi
 ps->pc_rdy=( (st&(1<<3))!=0 )?1:0;
 ps->pc_acc=( (st&(1<<2))!=0 )?1:0;
 ps->uc_rdy=( (st&(1<<1))!=0 )?1:0;
 ps->uc_acc=( (st&(1<<0))!=0 )?1:0;

 // stan sumaryczny
 ps->st=st;

 // jaki tryb bedzie odpowiedni?
 if( st!=mk_st(0,1,1,1) &&
     st!=mk_st(0,0,1,1)    )
   ps->rd_mode=1;
 else
   ps->rd_mode=0;

 return 0;
}; // decode_st()



// zwraca stan portu
static int pstate_get(int port, struct pstate *ps)
{
 char ctrl;
 char stat;

 // odczyt wlasciwy
 if( portio_read_control(port, &ctrl)!=0 )
   return -1;
 if( portio_read_status (port, &stat)!=0 )
   return -2;

 // tranzlacja na pola struktury
 ps->pc_rdy=( (ctrl&TIER_PC_DATA_RDY)!=0 )?1:0;
 ps->pc_acc=( (ctrl&TIER_PC_DATA_ACC)!=0 )?1:0;
 ps->uc_rdy=( (stat&TIER_uC_DATA_RDY)!=0 )?1:0;
 ps->uc_acc=( (stat&TIER_uC_DATA_ACC)!=0 )?1:0;

 // wyliczany sumaryczny stan
 ps->st=mk_st(ps->pc_rdy, ps->pc_acc,
              ps->uc_rdy, ps->uc_acc );

 // sprawdzamy tryb pracy
 ps->rd_mode=( (ctrl&PORTIO_CTRL_nWR)!=0 )?1:0;
 if( (stat&TIER_uC_DATA_RDY)==0 )	// zadanie zapisu od uC ma wyzszy pri.
   ps->rd_mode=1;

 return 0;
}; // pstate_get()




// ustawia stan portu
static int pstate_set(int port, struct pstate *ps)
{
 char ctrl;

 ctrl=ps->pc_rdy *TIER_PC_DATA_RDY +
      ps->pc_acc *TIER_PC_DATA_ACC +
      ps->uc_rdy *TIER_uC_DATA_RDY +
      ps->uc_acc *TIER_uC_DATA_ACC +
      ps->rd_mode*PORTIO_CTRL_nWR;	// jeszcze bit zapisu :)

 if( portio_write_control(port, ctrl)!=0 )
   return -1;

 return 0;
}; // pstate_set()




// I/O na buforach z punktu widzenia modulu

#define io_rx_enq(base, byte)	( wabuf_add ( (base)->rx, (byte) ) )
#define io_rx_deq(base, byte)	( wabuf_get ( (base)->rx, (byte) ) )
#define io_rx_cnt(base)		( wabuf_cnt ( (base)->rx )         )
#define io_rx_left(base)	( wabuf_left( (base)->rx )         )

#define io_tx_enq(base, byte)	( wabuf_add( (base)->tx, (byte) ) )
#define io_tx_deq(base, byte)	( wabuf_get( (base)->tx, (byte) ) )
#define io_tx_cnt(base)		( wabuf_cnt( (base)->tx )         )




//
// FUNKCJE WIDZIANE GLOBALNIE
//


struct tier_low_io *tier_low_io_create(int port)
{
 struct tier_low_io *base;

 assert( port>0     );

 base=(struct tier_low_io*)malloc( sizeof(struct tier_low_io) );
 if(base==NULL)
   return NULL;

 if( portio_open(port)!=0 )
 {
   free(base);
   return NULL;
 };

 // ustawiamy nasza strukture
 base->port=port;

 base->rx  =wabuf_create(1024, sizeof(unsigned char) );
 if(base->rx==NULL)
 {
   free(base);
   portio_close(port);
   return NULL;
 };

 base->tx=wabuf_create(1024, sizeof(unsigned char) );
 if(base->tx==NULL)
 {
   wabuf_free(base->rx);
   free(base);
   portio_close(port);
   return NULL;
 };

 // inicjalizujemy rejestr kontrolny odpowiednia wartoscia
 if( portio_write_control(base->port, 0xff)!=0 )
 {
   tier_low_io_free(base);
   return NULL;
 };

 return base;
}; // tier_low_io_create()



int tier_low_io_free(struct tier_low_io *base)
{
 assert( base!=NULL );

 // zamykamy port, jesli trzeba
 if(base->port>0) 		// jeszcze nie zwolniono?
   portio_close(base->port);

 // zwalniamy byfor TX
 if(base->tx!=NULL)
   wabuf_free(base->tx);

 // zwalniamy bufor RX
 if(base->rx!=NULL)
   wabuf_free(base->rx);

 // zwalniamy strukture wlaciwa
 free(base);

 return 0;
}; // tier_low_io_free()






int tier_low_io_do_io(struct tier_low_io *base)
{
 static char stat_map[16]={	// mapa stan0->stan1
				// dec  pr pa ur ua
			15,	//  0    0  0  0  0 - niepoprawny
			 3,	//  1    0  0  0  1 +
			15,	//  2    0  0  1  0 - niepoprawny
			 7,	//  3    0  0  1  1 +
			15,	//  4    0  1  0  0 - niepoprawny
			 1,	//  5    0  1  0  1 +
			14,	//  6    0  1  1  0 +
			 6,	//  7    0  1  1  1 +
			15,	//  8    1  0  0  0 - niepoprawny
			11,	//  9    1  0  0  1 +
			15,	// 10    1  0  1  0 - niepoprawny
			15,	// 11    1  0  1  1 +
			15,	// 12    1  1  0  0 - niepoprawny
			 9,	// 13    1  1  0  1 +
			15,	// 14    1  1  1  0 +
			15,	// 15    1  1  1  1 x reset (domyslny stan)

                          };

 struct pstate ps;		// odczytany stan portow
 struct pstate ps_new;		// nowy stan portow (do ustawienia)
 char          new_state;	// new state in machine
 char          sleep_some;	// flaga - czy trzeba zaczekac troche...

 assert( base!=NULL );

 sleep_some=0;			// domyslnie nie czekamy

 if( pstate_get(base->port, &ps)!=0 )	// pobieramy stan obecny
   return -1;
 ps_new=ps;		// robimy kopie do ustalenia nowego stanu

//printf("%d %d %d %d -> ", ps.pc_rdy, ps.pc_acc, ps.uc_rdy, ps.uc_acc);

 // czy przypadkiem nie mamy wolnego lacza, zeby cos wyslac? :)
 //if( ( ps.st==mk_st(1,1,1,1) || ps.st==mk_st(1,1,0,1) ) &&
 //    io_tx_cnt(base)>0 )
 if( needs_write(ps.st) && io_tx_cnt(base)>0 )
 {
   unsigned char   byte;
   //struct timespec delay={0, 350*1000};	// 350[us]
   //struct timespec delay={0, 10*1000*1000};	// 4[ms]
   //nanosleep(&delay, NULL);		// dajemy troche czasu dla uC

   if( io_tx_deq(base, &byte)!=0 )	// pobieramy bajt do nadania
     return -2;
   if( portio_write_data(base->port, byte)!=0 )	// zapis bajtu
     return -3;
//printf(" [w:0x%02X(%02d)]\n", byte, byte);

   sleep_some=1;			// damy troche czasu dla uC...
   new_state =mk_st(0,1,1,1);		// nasz kolej stan - zapis!
 } // if(zaczynamy_nadawac?)
 else	// nie ma czego nadawac?
   new_state=stat_map[(int)ps.st];	// kolejny stan w/g maszyny

 // moze mamy juz cos gotowego do odczytania (i miejsce na to)?
 if( has_new_data(ps.st) && io_rx_left(base)>0 )
 {
   unsigned char byte;

   if( portio_read_data(base->port, &byte)!=0 )
     return -4;
//printf(" [r:0x%02X(%02d)]\n", byte, byte);
   if( io_rx_enq(base, &byte)!=0 )
     return -5;
 };

 // ustalamy nowy stan, w jakim sie znajdziemy
 if( decode_st(&ps_new, new_state)!=0 )
   return -6;

 // zapisujemy nowy stan na wyprowadzenia portu
 if( pstate_set(base->port, &ps_new)!=0 )
   return -7;

//ps=ps_new;
//printf("%d %d %d %d\n", ps.pc_rdy, ps.pc_acc, ps.uc_rdy, ps.uc_acc);

 // dajemy troche czasu dla uC w przypadku kiedy jego stan ulegl zmianie
 //if(ps.st!=new_state)
 if(sleep_some)
 {
   //struct timespec delay={0, 350*1000};	// 350[us]
   struct timespec delay={0, 30*1000*1000};	// 30[ms] (1/(4800/10)*3*3 + zapas)
   nanosleep(&delay, NULL);		// dajemy troche czasu dla uC
 };

 return 0;
}; // tier_low_io_do_io()





int tier_low_io_reset(struct tier_low_io *base)
{
 // w ramach resetu dajemy wszystkie wyprowadzenia w stan wysoki,
 // ale port pozostawiamy w trybie odczytu!
 if( portio_write_control(base->port, 0xff&~PORTIO_CTRL_nWR)!=0 )
   return -1;

 return 0;
}; // tier_low_io_reset()


