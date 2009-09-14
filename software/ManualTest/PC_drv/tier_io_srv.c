/*
 * tier_io_srv.c
 *
 * server odbierajacy pakiety tier_pck poprzez fdptp,
 * przesylajacy je na wybrany port LPT oraz zwracajacy
 * odpowiedz poprzez to samo gniazdo.
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "fdptp/fdptp.h"
#include "tier_io/tier_io.h"
#include "tier_fdptp_comm.h"

#define VERSION "0.1.0"



void print_hello_message(char *ip, unsigned short port, int lpt)
{
 char lpt_no;

 // ustalamy jaki to port
 if(lpt==PORTIO_LPT0)
   lpt_no='0';
 else
   if(lpt==PORTIO_LPT1)
     lpt_no='1';
   else
     lpt_no='?';

 printf("\n");
 printf("TIER I/O server (version %s)\n", VERSION);
 printf("awaiting for incomming connections @ %s:%d\n", ip, port);
 printf("lpt is @ 0x%x (LPT%c)\n\n", lpt, lpt_no);
}; // print_hello_message()



void print_exit_help(char *pname, int ec)
{
 fprintf(stderr, "%s <lpt_addr> <listen_addr> <port>\n", pname);
 fprintf(stderr, "\tlp0_addr is %d (0x%x)\n", PORTIO_LPT0, PORTIO_LPT0);
 fprintf(stderr, "\tlp1_addr is %d (0x%x)\n", PORTIO_LPT1, PORTIO_LPT1);
 fprintf(stderr, "\tlisten_addr can be 1.2.3.4 or DNS name,"
                 " or '*' for 'any'\n");

 exit(ec);
}; // print_exit_help()



//
// MAIN
//
int main(int argc, char *argv[])
{
 struct tier_io *tier;		// komunikacja z robotem

 struct socket  *srv;		// nasz serwer - gniazdo nasluchujace
 char           *ip;		// ip nasluchiwania
 unsigned short  port;		// port nasluchiwania
 int             lpt_addr;	// adres portu LPT

 char            quit;		// konczymy program?


 //
 // przetwarzamy argumenty linii polecen:
 //

 if(argc!=4)			// nie podano argumentow jak trzeba?
   print_exit_help(argv[0], 1);

 // LPT:
 lpt_addr=atoi(argv[1]);
 if( lpt_addr!=PORTIO_LPT0 && lpt_addr!=PORTIO_LPT1 )
   print_exit_help(argv[0], 2);

 // IP & port:
 ip  =argv[2];
 port=atoi(argv[3]);

 //
 // inicjalizacja urzadzen i otwieranie portu
 //

 // otwieranie portu:
 tier=tier_io_create(lpt_addr);
 if(tier==NULL)
 {
   fprintf(stderr, "%s: unable to open LPT port @ 0x%x\n", argv[0], lpt_addr);
   return 3;
 };
 // ustawiamy timeout na 0.25[s]
 tier->h.timeout.time   =0;
 tier->h.timeout.millitm=250;

 // otwieranie gniazda do nasluchu: 
 srv=fdptp_listen(ip, port);
 if(srv==NULL)
 {
   fprintf(stderr, "%s: unable to open socket @ %s:%d\n", argv[0], ip, port);
   tier_io_free(tier);
   return 4;
 };


 print_hello_message(ip, port, lpt_addr);

 //
 // glowna petla przetwarzania danych
 // przyjmujemy polaczenia POJEDYNCZO i obslugujemy az do
 // konca transmisji, po czym czekamy dalej.
 //
 for(quit=0; !quit;)
 {

   struct socket *cln;		// gniazdo dla naszego klienta
   int            i;		// for()

   // czekamy az bedzie jakies polaczenie:
   cln=fdptp_accept(srv);
   if(cln==NULL)
   {
     fprintf(stderr, "%s: fdptp_accept(): failed\n", argv[0]);
     quit=1;
     continue;
   };
   printf("> got new connection from %s\n", 
           inet_ntoa( *(struct in_addr*)(&cln->adres_rem.sin_addr.s_addr) ) );

   // witamy sie z robotem
   printf("sending init... ");
   if( tier_io_init(tier)==0 )
     printf("ok\n");
   else
     printf("ERROR\n");


   //
   // przetwarzamy zadania klienta tak dlugo jak tylko
   // bedzie utrzymywal on polaczenie
   //
   while( !fdptp_was_closed(cln) )
   {
     struct fdptp_data          *data_in;
     struct tier_fdptp_comm_pck *pck_in;
     struct tier_fdptp_comm_pck  pck_out;
     struct fdptp_data           data_out;

     // pobieramy pakiet od klienta
     data_in=fdptp_read(cln);
     if(data_in==NULL)		// gniazdo zamkniete?
       continue;
     // typ sie nie zgadza?
     if( fdptp_data_len(data_in)!=sizeof(struct tier_fdptp_comm_pck) )
     {
       fdptp_free_data(data_in);
       continue;
     };

     // interpretujemy pakiet:
     pck_in      =(struct tier_fdptp_comm_pck*)data_in->data;
     pck_out.type=TIER_FDPTP_COMM_PCK_ERROR;	// domniemanie winy... ;)
     switch(pck_in->type)
     {
       case TIER_FDPTP_COMM_PCK_SPDST:
            if( tier_io_spdst(tier, pck_in->spd.left, pck_in->spd.right)==0 )
              pck_out.type=TIER_FDPTP_COMM_PCK_OK;
            break;

       case TIER_FDPTP_COMM_PCK_SPDRD:
            if( tier_io_spdrd(tier, &pck_out.spd.left, &pck_out.spd.right)==0 )
              pck_out.type=TIER_FDPTP_COMM_PCK_OK;
            break;

       case TIER_FDPTP_COMM_PCK_INIT:
            if( tier_io_init(tier)==0 )
              pck_out.type=TIER_FDPTP_COMM_PCK_OK;
            break;

       case TIER_FDPTP_COMM_PCK_STOP:
            if( tier_io_stop(tier)==0 )
              pck_out.type=TIER_FDPTP_COMM_PCK_OK;
            break;

       case TIER_FDPTP_COMM_PCK_PING:
            if( tier_io_ping(tier)==0 )
              pck_out.type=TIER_FDPTP_COMM_PCK_OK;
            break;

       default:
            break;
     }; // switch(pck_in_type)

     // zwracamy odpowiedz do nadawcy:
     fdptp_free_data(data_in);		// zwalniamy odebrany pakiet
     data_out=fdptp_mk_data_ptr( (unsigned char*)&pck_out,
                                 sizeof(struct tier_fdptp_comm_pck) );
     if( fdptp_write(cln, &data_out)<=0 )
     {
       continue;
     };
   }; // while(!closed)
   //
   // koniec przetwarzania pojedynczego kleinta
   //


   // zamykamy gniazdo
   fdptp_close(cln);
   printf("> connection has been closed\n");

   // wysylamy 3 razy sygnal stop dla pwnosci!
   printf("sending stop several times...\n");
   for(i=0; i<3; i++)
   {
     printf("\t%d: ", i);
     if( tier_io_stop(tier)==0 )
       printf("ok\n");
     else
       printf("ERROR (not yet implemented?)\n");
   };
   printf("\n");

 }; // for(!quit)
 //
 // koniec glownej petli przetwarzania
 //

 // zamykamy wszystko:
 fdptp_close(srv);
 tier_io_free(tier);

 return 0;
}; // main()

