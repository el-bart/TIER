#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "tier_io_client.h"



int main(int argc, char *argv[])
{
 struct tier_io_client *tio_cln;
 struct timespec        dt={2,0};

 if(argc!=3)
 {
   fprintf(stderr, "%s <srv_addr> <port>\n", argv[0]);
   return 1;
 };

 tio_cln=tier_io_client_create(argv[1], atoi(argv[2]) );
 assert(tio_cln!=NULL);


 // rozpoczecie
 fprintf(stderr, "init...");
 assert( tier_io_client_init(tio_cln)==0 );
 fprintf(stderr, "\t\tok\n");

 // ustalenie predkosci i poczekanie
 fprintf(stderr, "spdst...");
 assert( tier_io_client_set_speed(tio_cln, +200, +200)==0 );
 fprintf(stderr, "\tok\n");

 // niech chwilke pochodzi...
 fprintf(stderr, "  sleep...");
 nanosleep(&dt, NULL);
 fprintf(stderr, "\tok\n");

 // reset robota
 fprintf(stderr, "stop...");
 assert( tier_io_client_stop(tio_cln)==0 );
 fprintf(stderr, "\t\t??\n");
 

 tier_io_client_free(tio_cln);

 return 0;
}; // main()

