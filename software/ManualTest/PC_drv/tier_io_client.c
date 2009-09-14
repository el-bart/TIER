/*
 * tier_io_client.c
 *
 */
#include <stdlib.h>	// NULL;
#include <assert.h>	// assert();
#include <string.h>	// memcpy();

#include "tier_io_client.h"




struct tier_io_client *tier_io_client_create(char *addr, unsigned short port)
{
 struct socket         *s;		// fdptp
 struct tier_io_client *tio_cln;	// klient

 assert(addr!=NULL);

 s=fdptp_connect(addr, port);		// laczymy sie z serverem
 if(s==NULL)
   return NULL;

 // przydzielamy pamiec
 tio_cln=(struct tier_io_client*)malloc( sizeof(struct tier_io_client) );
 if(tio_cln==NULL)
 {
   fdptp_close(s);
   return NULL;
 };

 tio_cln->s              =s;		// gniazdo
 tio_cln->timeout.time   =0;		// timeout 0.25[s]
 tio_cln->timeout.millitm=250;		// ...

 return tio_cln;
}; // tier_io_cleint_create()



int tier_io_client_free(struct tier_io_client *tio_cln)
{
 assert(tio_cln!=NULL);

 fdptp_close(tio_cln->s);
 free(tio_cln);

 return 0;
}; // tier_io_client_free()



// wewnetrzna f-cja pomocnicza: wysyla i odbiera dane, zwracajac
// przy tym odpowiedni kod powrotu i dane.
static int do_io(struct socket *s, struct tier_fdptp_comm_pck *in,
                                   struct tier_fdptp_comm_pck *out)
{
 struct fdptp_data  data;
 struct fdptp_data *resp;
 const int          size=sizeof(struct tier_fdptp_comm_pck);

 data=fdptp_mk_data_ptr( (char*)in, size);
 if( fdptp_write(s, &data)!=0 )			// wysylamy zadanie
   return -1;

 resp=fdptp_read(s);				// pobieramy odpowiedz
 if(resp==NULL)
   return -2;
 if( fdptp_data_len(resp)!=size )		// dlugosc sie zgadza?
 {
   fdptp_free_data(resp);
   return -3;
 };
 memcpy(out, resp->data, size);			// kopiujemy dla user'a

 fdptp_free_data(resp);				// zwalniamy pamiec

 return 0;
}; // do_io()




int tier_io_client_set_speed(struct tier_io_client *tio_cln,
                             int left, int right)
{
 struct tier_fdptp_comm_pck pck;
 struct tier_fdptp_comm_pck resp_pck;

 pck.type     =TIER_FDPTP_COMM_PCK_SPDST;
 pck.spd.left =left;
 pck.spd.right=right;

 // wysylamy i odbieramy dane
 if( do_io(tio_cln->s, &pck, &resp_pck)!=0 )
   return -1;

 // sprawdzamy czy odpowiedz jest poprawna
 if( resp_pck.type!=TIER_FDPTP_COMM_PCK_OK )
   return -2;

 return 0;		// wszystko ok! :)
}; // tier_io_client_set_speed()



int tier_io_client_get_speed(struct tier_io_client *tio_cln,
                             int *left, int *right)
{
 struct tier_fdptp_comm_pck pck;
 struct tier_fdptp_comm_pck resp_pck;

 pck.type=TIER_FDPTP_COMM_PCK_SPDRD;

 // wysylamy i odbieramy dane
 if( do_io(tio_cln->s, &pck, &resp_pck)!=0 )
   return -1;

 // sprawdzamy czy odpowiedz jest poprawna
 if( resp_pck.type!=TIER_FDPTP_COMM_PCK_OK )
   return -2;

 // zwracamy odpowiedz
 *left =resp_pck.spd.left;
 *right=resp_pck.spd.right;

 return 0;		// wszystko ok! :)
}; // tier_io_client_get_speed()



int tier_io_client_init(struct tier_io_client *tio_cln)
{
 struct tier_fdptp_comm_pck pck;
 struct tier_fdptp_comm_pck resp_pck;

 pck.type=TIER_FDPTP_COMM_PCK_INIT;

 // wysylamy i odbieramy dane
 if( do_io(tio_cln->s, &pck, &resp_pck)!=0 )
   return -1;

 // sprawdzamy czy odpowiedz jest poprawna
 if( resp_pck.type!=TIER_FDPTP_COMM_PCK_OK )
   return -2;

 return 0;		// wszystko ok! :)
}; // tier_io_client_init()



int tier_io_client_stop(struct tier_io_client *tio_cln)
{
 struct tier_fdptp_comm_pck pck;
 struct tier_fdptp_comm_pck resp_pck;

 pck.type=TIER_FDPTP_COMM_PCK_STOP;

 // wysylamy i odbieramy dane
 do_io(tio_cln->s, &pck, &resp_pck);	// NOTA: obecna implementacja NIE
					// wspiera wysylania potwierdzenia
					// komendy stop.
/*
 if( do_io(tio_cln->s, &pck, &resp_pck)!=0 )
   return -1;

 // sprawdzamy czy odpowiedz jest poprawna
 if( resp_pck.type!=TIER_FDPTP_COMM_PCK_OK )
   return -2;*/

 return 0;		// wszystko ok! :)
}; // tier_io_client_stop()



int tier_io_client_ping(struct tier_io_client *tio_cln)
{
 struct tier_fdptp_comm_pck pck;
 struct tier_fdptp_comm_pck resp_pck;

 pck.type=TIER_FDPTP_COMM_PCK_PING;

 // wysylamy i odbieramy dane
 if( do_io(tio_cln->s, &pck, &resp_pck)!=0 )
   return -1;

 // sprawdzamy czy odpowiedz jest poprawna
 if( resp_pck.type!=TIER_FDPTP_COMM_PCK_OK )
   return -2;

 return 0;		// wszystko ok! :)
}; // tier_io_client_ping()


