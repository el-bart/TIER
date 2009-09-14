/*
 * tier_io.c
 *
 */
#include <assert.h>	// assert()
#include <time.h>	// time()
#include <stdlib.h>	// abs()

//#include <stdio.h>// TODO - usunac to!

#include "tier_io.h"


//
// FUNKCJE WEWNETRZNE MODULU
//

// stara sie zinterpretowac to co jest obecnie w kolejce i wstawic
// pod podany adres. w przypadku sukcesu zwraca >0, w przypadku bledu <0
// lub poprostu 0 jezeli nie udalo sie niczego sensownego odczytac.
static int try_interpret_queue(struct tier_low_io  *base,
                               struct tier_pck_all *pck,
                               struct tier_pck_all *pck_cmd)
{
 struct tier_pck_serial serial;	// bufor
 int  i;			// for()

 assert(pck !=NULL);
 assert(base!=NULL);

 if( tier_low_io_rx_cnt(base)<1 )	// nie ma sie czym zajmowac?
   return 0;

 // pobieramy pierwszy bajt
 tier_low_io_peek(base, &serial.byte[0], 0);

 // sprawdzamy:
 if( !tier_pck_header_ok(serial.byte[0]) )	// nie naglowek?
 {
   tier_low_io_dequeue(base, &serial.byte[0]);	// pozbywamy sie smiecia
   return 0;
 };
 serial.size=tier_pck_size(serial.byte[0]);	// ustalamy rozmiar
 if( tier_low_io_rx_cnt(base)<serial.size )	// za malo bajtow?
   return 0;

 // no to sprawdzamy reszte! :)
 for(i=1; i<serial.size; i++)
   tier_low_io_peek(base, &serial.byte[i], i);

 // sprawdzamy czy pakiet da sie zdeserializowac
 if( tier_pck_deserialize(pck, &serial)!=0 )
 {
   tier_low_io_dequeue(base, &serial.byte[0]);	// pozbywamy sie smiecia
   return 0;
 };

 // na koniec jeszcze sprawdzmay czy pakiet jest odpowiedzia na nasz rozkaz
 if(pck_cmd!=NULL)
   if( !tier_pck_is_response(pck_cmd, pck) )	// nie jest?
   {
     tier_low_io_dequeue(base, &serial.byte[0]);// no to na pohybel z nim!
     return 0;
   };

 // jezeli wszystko sie zgadza, to usuwamy calosc z kolejki!
 for(i=0; i<serial.size; i++)
 {
   char tmp;
   tier_low_io_dequeue(base, &tmp);		// dane juz przetworzone
 };

 return 1;
}; // try_interpret_queue()



//
// FUNKCJE GLOBALNE
// 


struct tier_io *tier_io_create(int port)
{
 struct tier_io *tio;

 tio=(struct tier_io*)malloc( sizeof(struct tier_io) );
 if(tio==NULL)
   return NULL;

 tio->comm=tier_low_io_create(port);
 if(tio->comm==NULL)
 {
   free(tio);
   return NULL;
 };

 return tio;
}; // tier_io_create()



int tier_io_free(struct tier_io *tio)
{
 assert(tio!=NULL);

 tier_low_io_free(tio->comm);
 free(tio);

 return 0;
}; // tier_io_free()



int tier_io_do_comm(struct tier_io *tio)
{
 struct timeb           tnow;
 struct tier_pck_serial serial;
 int                    i;
 long long              timeout_val;

 // serializujemy pakiet do nadawania
 if( tier_pck_serialize(&serial, &tio->h.pck_cmd)!=0 )
   return -2;
 // ustawiamy w kolejce
//fprintf(stderr, "sending:  ");
 for(i=0; i<serial.size; i++)
 {
//fprintf(stderr, "0x%x ", serial.byte[i]);
   if( tier_low_io_enqueue(tio->comm, &serial.byte[i])!=0 )
     return -1;
 };
//fprintf(stderr, "\n");

 // warunki poczatkowe:
 ftime(&tnow);
 timeout_val=(tio->h.timeout.time   +tnow.time   )*1000 +
             (tio->h.timeout.millitm+tnow.millitm);
 tio->h.ok  =0;

 while( timeout_val >= tnow.time*1000+tnow.millitm &&
        !tier_io_handler_ok( &(tio->h) )              )
 {
//fprintf(stderr, ".");
   tier_low_io_do_io(tio->comm);	// nadawanie i odbior!
   // probujemy cos sklecic z tego co odebralismy
   if( try_interpret_queue(tio->comm, &tio->h.pck_rep, &tio->h.pck_cmd)>0 )
     tio->h.ok=1;			// koniec!

   // naliczamy timeout...
   ftime(&tnow);
 }; // while()

 // udalo sie odebrac odpowiedz?
 if( tier_io_handler_ok( &tio->h ) )
 {
   tier_pck_serialize(&serial, &tio->h.pck_rep);
//fprintf(stderr, "recieved: ");
//   for(i=0; i<serial.size; i++)
//     fprintf(stderr, "0x%x ", serial.byte[i]);
//   fprintf(stderr, "\n\n");
 }
 else // nie ma odpowiedzi/odpowiedz nie poprawna?
 {
//   printf("no response/error in response!!! :((\n");
   return -1;
 };

 return 0;
}; // tier_io_do_comm()



int tier_io_queue_reset(struct tier_io *tio)
{
 return tier_low_io_reset(tio->comm);
}; // tier_io_queue_reset()




//
// fukncke wysylajace konkretne pakiety
//


int tier_io_init(struct tier_io *tio)
{
 tio->h.pck_cmd=tier_pck_mk_init();
 return tier_io_do_comm(tio);
}; // tier_io_init()


int tier_io_spdst(struct tier_io *tio, int left, int right)
{
 unsigned char t1,t2;
 unsigned char dir;

 assert( abs(left )<256 );
 assert( abs(right)<256 );

 t2 =abs(left );
 t1 =abs(right);
 dir=( (left >0)?0x04:0x08 ) | 
     ( (right>0)?0x01:0x02 );

 // 0==stop:
 if(left ==0)
   dir&=0xff-(0x04|0x08);
 if(right==0)
   dir&=0xff-(0x01|0x02);

 tio->h.pck_cmd=tier_pck_mk_spdst(t1, t2, dir);
 return tier_io_do_comm(tio);
}; // tier_io_spdst()


int tier_io_spdrd(struct tier_io *tio, int *left, int *right)
{
 //unsigned char t1,t2;
 //unsigned char dir;

 //tio->h.pck_

 //return tier_io_do_comm(tio);
 return -1;
}; // tier_io_spdrd()


int tier_io_ping(struct tier_io *tio)
{
 tio->h.pck_cmd=tier_pck_mk_pp();
 return tier_io_do_comm(tio);
}; // tier_io_ping()


int tier_io_stop(struct tier_io *tio)
{
 tio->h.pck_cmd=tier_pck_mk_stop();
 return tier_io_do_comm(tio);
}; // tier_io_stop()

