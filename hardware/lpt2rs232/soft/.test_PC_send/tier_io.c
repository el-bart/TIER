#include <stdio.h>
#include <assert.h>

#include "portio/portio.h"


// bity uzywane do komunikacji
// control:
#define TIER_PC_DATA_RDY	(1<<0)
#define TIER_PC_DATA_ACC	(1<<1)
// status:
#define TIER_uC_DATA_RDY	(1<<6)
#define TIER_uC_DATA_ACC	(1<<7)



int tier_write(int port, unsigned char byte)
{
 unsigned char tmp;
 unsigned char ctrl;

 ctrl=0xff & (~PORTIO_CTRL_nWR);
 portio_write_control(port, ctrl);	// WRITE mode

 portio_write_data(port, byte);		// zapisujemy dane
 ctrl&=~TIER_PC_DATA_RDY;
 portio_write_control(port, ctrl);	// zaznaczamy, ze wysylamy

 // czekamy az uC potwierdzi
 do
 {
   portio_read_status(port, &tmp);	// juz odebrano?
 }
 while( (tmp&TIER_uC_DATA_ACC)!=0 );

 portio_write_control(port, 0xff);	// EOT

 return 0;
}; // tier_write()


unsigned char tier_read(int port)
{
 unsigned char byte;
 unsigned char ctrl;
 unsigned char tmp;

 ctrl=0xff;				// READ mode
 //portio_write_data(port, 0xff);		// ...
 portio_write_control(port, ctrl);	// ...

 // wait for read-request flag
 do
 {
   portio_read_status(port, &tmp);	// czekamy na sygnal odczytu
 }
 while( (tmp&TIER_uC_DATA_RDY)!=0 );

 portio_read_data(port, &byte);		// pobieramy bajt

 ctrl&=~TIER_PC_DATA_ACC;		// sygnalizujemy odbior
 portio_write_control(port, ctrl);	// ...

 // wait for read-acc-acc flag :)
 do
 {
   portio_read_status(port, &tmp);	// czekamy na sygnal odczytu
 }
 while( (tmp&TIER_uC_DATA_RDY)==0 );

 portio_write_control(port, 0xff);	// koniec transmisji
 
 return byte;
}; // tier_read()



int main(void)
{
 int           port;
 int           i;
 unsigned char tx[]={0x1e, 0xff,0x7f,0x05, 0x00};
 unsigned char rx[]={0x00, 0x00,0x00,0x00, 0x00};

 //port=PORTIO_LPT1;
 port=PORTIO_LPT0;
 assert( portio_open(port)==0 );

 // liczenie sumy kontrolnej
 //tab[sizeof(tx)-1]=0x00;
 for(i=0; i<sizeof(tx)-1; i++)
   tx[sizeof(tx)-1]+=tx[i]+1;

/*
 // nadawanie
 for(i=0; i<sizeof(tx); i++)
 {
   fprintf(stderr, "writing %x... ", tx[i]);
   tier_write(port, tx[i]);
   fprintf(stderr, "DONE! :)\n");
 };
 */

 // odbieranie
 for(i=0; i<40; i++)
 {
   fprintf(stderr, "reading... ");
   rx[0]=tier_read(port);
   fprintf(stderr, "0x%x\n", rx[0]);
 };
 

 assert( portio_close(port)==0 );

 return 0;
}; // main()

