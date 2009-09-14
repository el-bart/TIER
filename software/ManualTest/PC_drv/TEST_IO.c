#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#include "tier_io/tier_io.h"



int main(void)
{
 struct tier_io  *tio;
 struct timespec  dt={2,0};
 struct timeb     tout;

 tio=tier_io_create(PORTIO_LPT0);
 assert(tio!=NULL);

 // ustalamy timeout... 0.25[s]
 tout.time   =0;
 tout.millitm=250;



 // rozpoczecie
 fprintf(stderr, "init...");
 tio->h.timeout=tout;
 assert( tier_io_init(tio)==0 );
 fprintf(stderr, "\t\t%s\n", tier_io_handler_ok(&tio->h)?"ok":"ERROR" );

 // ustalenie predkosci i poczekanie
 fprintf(stderr, "spdst...");
 tio->h.timeout=tout;
 assert( tier_io_spdst(tio, +200, +200)==0 );
 fprintf(stderr, "\t%s\n", tier_io_handler_ok(&tio->h)?"ok":"ERROR" );

 // niech chwilke pochodzi...
 fprintf(stderr, "  sleep...");
 nanosleep(&dt, NULL);
 fprintf(stderr, "\tok\n");

 // reset robota
 fprintf(stderr, "stop...");
 tio->h.timeout=tout;
 assert( tier_io_stop(tio)==0 );
 fprintf(stderr, "\t\t%s\n", tier_io_handler_ok(&tio->h)?"ok":"ERROR" );
 


 tier_io_free(tio);

 return 0;
}; // main()

