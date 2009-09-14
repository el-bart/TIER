#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#include "fdptp.h"
#include "test_config.h"


//
// kod klienta
//
int main(void)
{
 struct socket *s;
 int            ret;
 int            i;		// for(;;)
 int            cnt;		// for(zew)
 struct fdptp_data *data;

 //s=fdptp_connect("127.0.0.1", PORT);
 //assert(s!=NULL);

 for(cnt=0 ; cnt<STOP_CNT-1; cnt++)
 {
   s=fdptp_connect("localhost", PORT);
   assert(s!=NULL);
 
   // odbeiramy kilka razy cos:
   for(i=0; i<10; i++)
   {
     for(;;)
     {
       data=fdptp_read_nonblock(s);
       //data=fdptp_read(s);
       //assert(data!=NULL);
       if(data!=NULL)
         break;
       else
       {
         struct timespec dt={0,10100};	//~1/100[s]
         nanosleep(&dt, NULL);
         assert( !fdptp_was_closed(s) );
       };
     };
     assert( strcmp(data->data, MSG)==0 );
     fdptp_free_data(data);		// nie potrzeba nam to do szczescia
   };

   ret=fdptp_close(s);
   assert(ret==0);

   // co jakis czas wypisujemy
   if(cnt%WYP==0)
     fprintf(stderr, "%d tries...\n", cnt);

 }; // for(;;)

 fprintf(stderr, "any key to exit...\n");
 scanf("%d", &i);

 return 0;
};

