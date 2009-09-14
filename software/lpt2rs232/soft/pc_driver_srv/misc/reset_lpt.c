//
// reset_lpt.c
//
// ustawia wyprowadzenia portu w stani niski (aby nie 
// zasilac uC przez to).
//

#include <stdio.h>
#include <assert.h>

#include "portio/portio.h"



int main(void)
{
 int           port;

 //port=PORTIO_LPT1;
 port=PORTIO_LPT0;
 assert( portio_open(port)==0 );

 assert( portio_write_data   (port, 0)==0 );
 assert( portio_write_control(port, 0)==0 );

 assert( portio_close(port)==0 );

 return 0;
}; // main()

