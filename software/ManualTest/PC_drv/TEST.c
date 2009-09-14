#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#include "tier_low_io.h"



int main(void)
{
 struct tier_low_io *base;
 unsigned char       tx[]={0x1e, 0xff,0x7f,0x05, 0x00};
 int                 i;

 // liczenie sumy kontrolnej
 //tab[sizeof(tx)-1]=0x00;
 for(i=0; i<sizeof(tx)-1; i++)
   tx[sizeof(tx)-1]+=tx[i]+1;

 assert( (base=tier_low_io_create(PORTIO_LPT0))!=NULL ); // init
 assert( tier_low_io_reset(base)==0 );	// reset port

 // kolejkowanie danych do wyslania
 fprintf(stderr, "write: ");
 for(i=0; i<sizeof(tx); i++)
 {
   fprintf(stderr, "0x%x  ", tx[i]);
   assert( tier_low_io_enqueue(base, &tx[i])==0 );
 };
 fprintf(stderr, "\n");


 // wysylamy dane i wyczekujemy odpowiedzi
 fprintf(stderr, "read:  ");
 i=0;
 while(i<3)
 {
   //struct timespec dt={0,2*1000*1000};
   //nanosleep(&dt, NULL);
   assert( tier_low_io_do_io(base)==0 );

   if( tier_low_io_rx_cnt(base) >0 )	// otrzymalismy cos? :)
   {
     unsigned char b;
     i++;		// zwiekaszamy licznik odebranych bajtow
     assert( tier_low_io_dequeue(base, &b)==0 );
     fprintf(stderr, "0x%x  ", b);
   };
 };
 fprintf(stderr, "\n");	// dla pozadq wyswietlania...

 assert( tier_low_io_free(base)==0 );	// free resources

 return 0;
}; // main()

