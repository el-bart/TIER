#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <netinet/in.h>
#include <string.h>

#include "fdptp.h"
#include "test_config.h"


//
// kod servera
//
int main(void)
{
 struct socket *s;
 int            ret;
 int            i;		// for(;;)
 int            cnt;		// for(zew)
 char           buf[SIZE];
 //unsigned short len;
 struct fdptp_data *data;

 // ustalanie dlugosci
 //len=SIZE-sizeof(unsigned short);
 //len=htons(len);
 //memcpy(buf, &len, sizeof(unsigned short) );

 s=fdptp_listen("localhost", PORT);
 assert(s!=NULL);

 strcpy(buf, MSG);
 data=fdptp_mk_data(buf, (unsigned short)SIZE);
 //data.data=buf;
 //data.len =htons(SIZE);

 for(cnt=0 ; cnt<STOP_CNT-1; cnt++)
 {
   struct socket *c;
   for(;;)
   {
     c=fdptp_accept_nonblock(s);
     if(c==NULL)	// nic nie ma
     {
       struct timespec dt={0,10100};	// ~1/100[s]
       nanosleep(&dt, NULL);
       continue;
     }
     else
       break;
   }; // for(accept)

   // wysylamy kilka razy cos:
   for(i=0; i<10; i++)
   {
     ret=fdptp_write(c, data);
     assert( !fdptp_was_closed(c) );
     if( ret!=SIZE )//-sizeof(unsigned short) )
     {
       fprintf(stderr, "ret code  - %d\n", ret);
       perror("???");
       return 1;
     };
   };

   ret=fdptp_close(c);
   assert(ret==0);

   // co jakis czas wypisujemy
   if(cnt%WYP==0)
     fprintf(stderr, "%d clients...\n", cnt);

 }; // for(;;)

 assert( !fdptp_was_closed(s) );
 ret=fdptp_close(s);
 assert(ret==0);

 fprintf(stderr, "any key to exit...\n");
 scanf("%d", &i);

 return 0;
};

