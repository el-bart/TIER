#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#include "tier_io/tier_io.h"



int main(void)
{
  struct tier_io  *tio;
  struct timeb     tout;
  int              s,i;     // for()

  tio=tier_io_create(PORTIO_LPT0);
  assert(tio!=NULL);

  // ustalamy timeout... 0.5[s]
  tout.time   =0;
  tout.millitm=500;



  // rozpoczecie
  fprintf(stderr, "init...");
  tio->h.timeout=tout;
  assert( tier_io_init(tio)==0 );
  fprintf(stderr, "\t\t%s\n", tier_io_handler_ok(&tio->h)?"ok":"ERROR" );

  for(s=0; s<255; s+=255/10)
  {
    struct timespec  dt={0,500*1000*1000};

    // ustalenie predkosci i poczekanie
    fprintf(stderr, "spdst [%d;%d]...", s, s);
    tio->h.timeout=tout;
    if( tier_io_spdst(tio, s, s)!=0 )
      break;
    fprintf(stderr, "\t%s\n", tier_io_handler_ok(&tio->h)?"ok":"ERROR" );

    // niech chwilke pochodzi...
    fprintf(stderr, "  sleep...");
    nanosleep(&dt, NULL);
    fprintf(stderr, "\tok\n");
  };

  // dla pewnosci rozkaz wysylamy kilka razy...
  for(i=0; i<3; ++i)
  {
    // reset robota
    fprintf(stderr, "stop...");
    tio->h.timeout=tout;
    assert( tier_io_stop(tio)==0 );
    fprintf(stderr, "\t\t%s\n", tier_io_handler_ok(&tio->h)?"ok":"ERROR" );
  };

  // koniec pracy :)
  tier_io_free(tio);

  return 0;
}; // main()

