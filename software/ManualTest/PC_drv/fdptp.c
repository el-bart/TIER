#include <stdlib.h>	// malloc();
#include <netinet/in.h> // htonl(); htons(); ntohl(); ntohs();
//#include <sys/types.h>  // open(); bind() - ale nie bezposrednio
#include <sys/socket.h> // socket();
#include <unistd.h>     // close();
#include <arpa/inet.h>  // inet_aton(); inet_ntoa();
#include <string.h>	// memcpy(); strcmp();
#include <assert.h>	// assert();
#include <netdb.h>	// gethostbyname();

#include "fdptp.h"




// ilu klientow moze czekac w kolejce?
#define  LISTEN_QUEUE_LEN	(2)

// wartosc oznaczajaca, ze jeszcze nie zaczelismy sciagac naglowka
#define FDPTP_SOCK_START_NONE	( sizeof(unsigned short)+1 )



//
// wewnetrzne funkcje pomocnicze
//

static int resolve_addr(char *addr, unsigned short port,
                        struct sockaddr_in *addr_out)
{
 assert(addr    !=NULL);
 assert(addr[0] !=0   );
 assert(addr_out!=NULL);

 // prbujemy ustalic IP na podstawie adresu
 if( strcmp(addr, "*")==0 )	// dowolny adres?
   addr_out->sin_addr.s_addr=INADDR_ANY;
 else
 {
   addr_out->sin_addr.s_addr=inet_addr(addr);
   if(addr_out->sin_addr.s_addr==INADDR_NONE)	// nie IP?
   {
     struct hostent *dns_info;
     // ostatnia szansa jest DNS:
     dns_info=gethostbyname(addr);
     if(dns_info==NULL || dns_info->h_addr_list[0]==NULL)	// nic z tego?
       return -1;
     addr_out->sin_addr.s_addr=
         *((in_addr_t*)dns_info->h_addr_list[0]);	// arbitalny...
   };
   // jezeli to adres IP, to jestesmy w domu! :)
 }; // else(addr=="*")

 // ustawiamy pozostale pola:
 addr_out->sin_family=AF_INET;
 addr_out->sin_port  =htons(port);

 return 0;
}; // resolve_addr()





//
// funckej widoczne na zewnatrz modulu
//


struct socket *fdptp_connect(char *addr, unsigned short port)
{
 int                 tmp;	// pomocnicza
 struct socket      *s;		// gniazdo do zwrocenia
 struct sockaddr_in  adres;	// adres do podlczaenia

 assert(addr   !=NULL);
 assert(addr[0]!=0   );

 if( resolve_addr(addr, port, &adres)!=0 )	// ustalamy adres
   return NULL;

 // tworzymy strukture do zwrocenia
 s=(struct socket*)malloc( sizeof(struct socket) );
 if(s==NULL)
   return NULL;

 // otwieramy gniazdo
 s->s=socket(AF_INET, SOCK_STREAM, 0);
 if(s->s==-1)
 {
   free(s);
   return NULL;
 };

 // podlaczamy sie!
 tmp=connect(s->s, (const struct sockaddr *)&adres, sizeof(adres));
 if(tmp==-1)
 {
   close(s->s);
   free(s);
   return NULL;
 };

 // na poaczatku niczego nie robilismy...
 s->start     =FDPTP_SOCK_START_NONE;
 s->data      =NULL;
 s->adres_rem =adres;
 s->was_closed=0;

 // zwracamy wynik pracy :)
 return s;
}; // fdptp_connect()









// zwraca !=0 jesli w gniezdzie sa jakies dane
// lub zero, w przeciwnym przypadku
int socket_has_data(struct socket *s)
{
 fd_set         rd_sock;	// zbior gniazd pod lupe :)
 int            out;		// wartosc zwrocona przez select()
 struct timeval t={0,0};	// czas oczekiwania na dane - nie czekamy wcale

 assert(s!=NULL);

 FD_ZERO(&rd_sock);		// nie mamy zadnego gniazda w zbiorze
 FD_SET(s->s, &rd_sock);	// dodajemy nasze gniazdo do zbioru

 out=select(s->s+1, &rd_sock, NULL, NULL, &t);	// sprawdzamy
 if(out==-1)
 {
   s->was_closed=1;
   return 0;
 };

 return out;			// zwracamy wynik
}; // socket_has_data()









struct fdptp_data *fdptp_read_nonblock(struct socket *s)
{
 int                tmp;		// pomocnicza
 int                off;		// przesuniecie wzgledem poczatku
 char              *data_out;		// do zwrocenia (czesc danych)
 struct fdptp_data *data_ret;		// do zwrocenia

 assert(s!=NULL);

 if( !socket_has_data(s) )		// nie czekaja na nas zadne dane?
   return NULL;

 if(s->start==FDPTP_SOCK_START_NONE)	// nie rozpoczelismy sciagania
					// dlugosc danych do pobrania?
 {
   s->start     =sizeof(s->total_len);	// tyle danych mamy do sciagniecia
   s->data      =(struct fdptp_data*)malloc( sizeof(struct fdptp_data) );
   s->data->data=NULL;			// na razie brak danych
   //s->data->data=(char*)malloc( sizeof(s->total_len) );	// przydzielamy pamiec
 }; // if(!start)

 if(s->start>0)				// jeszcze sciagamy naglowek?
 {
   // odczytujemy reszte naglowka
   off=sizeof(s->total_len) - s->start;	// ile juz mamy?
   tmp=read(s->s, &(s->data->len)+off, s->start );// odczytujemy ile sie da...
   if(tmp<=0)				// zamkniete gniazdo, lub blad?
   {
     fdptp_free_data(s->data);
     s->data      =NULL;
     s->start     =FDPTP_SOCK_START_NONE;
     s->was_closed=1;
     return NULL;
   };
   s->start-=tmp;			// odejmujemy co sciagniete
   if(s->start!=0)			// jeszcze nie wszystko...
     return NULL;
   
   // juz wszsytko sciagnelismy! (z naglowka)
   // sprawdzamy ile mamy docelowo odczytac
   //memcpy(&(s->total_len), s->data, sizeof(s->total_len) );
   s->total_len=ntohs(s->data->len);	// przechodzimy na notacje host'a
   data_out=(char*)malloc(s->total_len);
   if(data_out==NULL)
     return NULL;
   s->data->data=data_out;	// obszar pamieci, ktory pomiesci cale dane
   s->rem_len   =s->total_len;	// poki co nie mmay niczego sciagnietego
 }; // if(naglowek)

 if( !socket_has_data(s) )              // nie czekaja na nas zadne dane?
   return NULL;

 // sciagamy reszte (nie naglowek)
 off=s->total_len - s->rem_len;			// nasz offset
 tmp=read(s->s, s->data->data+off, s->rem_len);	// odczyt wlasciwy...
 if(tmp<=0)				// zamkniete gniazdo, lub blad?
 {
   fdptp_free_data(s->data);
   s->data      =NULL;
   s->start     =FDPTP_SOCK_START_NONE;
   s->was_closed=1;
   return NULL;
 };
 s->rem_len-=tmp;				// zaznaczamy ile mamy

 if(s->rem_len!=0)		// nadal mamy dane do sciagniecia?
   return NULL;

 // koniec sciagania - zwracamy sciagniete dane
 data_ret=s->data;			// kopia do zwrocenia
 s->data =NULL;				// na wszelki wypadek :)
 s->start=FDPTP_SOCK_START_NONE;	// nastepnym razem startuj od naglowka
 return data_ret;			// zwracamy cosmy sciagneli :)
}; // fdptp_read_nonblock()





struct fdptp_data *fdptp_read(struct socket *s)
{
 int                tmp;		// pomocnicza
 int                off;		// przesuniecie wzgledem poczatku
 struct fdptp_data *data_ret;		// do zwrocenia

 assert(s!=NULL);

 // instrukcja warunkowa jest tu potrzebna, poniewaz istnieje ryzyko, ze
 // odbieramy dane po owczesnych wywolaniach wersji nieblokujacej!
 if(s->start==FDPTP_SOCK_START_NONE)	// nie rozpoczelismy sciagania
					// dlugosc danych do pobrania?
 {
   s->start     =sizeof(s->total_len);	// tyle danych mamy do sciagniecia
   s->data      =(struct fdptp_data*)malloc( sizeof(struct fdptp_data) );
   s->data->data=NULL;			// na razie brak danych
 }; // if(!start)

 if(s->start>0)				// jeszcze sciagamy naglowek?
 {
   char *data_out;			// do zwrocenia (czesc danych)

   // odczytujemy reszte naglowka az do skutku
   while(s->start!=0)
   {
     off=sizeof(s->total_len)-s->start;	// ile juz mamy?
     tmp=read(s->s, &(s->data->len)+off, s->start );// odczytujemy ile sie da...
     if(tmp<=0)				// zamkniete gniazdo, lub blad?
     {
       fdptp_free_data(s->data);
       s->data      =NULL;
       s->start     =FDPTP_SOCK_START_NONE;
       s->was_closed=1;
       return NULL;
     };
     s->start-=tmp;			// odejmujemy co sciagniete
   }; // while(naglowek)
   
   // juz wszsytko sciagnelismy! (z naglowka)
   // sprawdzamy ile mamy docelowo odczytac
   s->total_len=ntohs(s->data->len);	// przechodzimy na notacje host'a
   data_out=(char*)malloc(s->total_len);
   if(data_out==NULL)
     return NULL;
   s->data->data=data_out;	// obszar pamieci, ktory pomiesci cale dane
   s->rem_len   =s->total_len;	// poki co nie mmay niczego sciagnietego
 }; // if(naglowek)

 // sciagamy reszte (nie naglowek)
 while(s->rem_len!=0)
 {
   off=s->total_len - s->rem_len;			// nasz offset
   tmp=read(s->s, s->data->data+off, s->rem_len);	// odczyt wlasciwy...
   if(tmp<=0)				// zamkniete gniazdo, lub blad?
   {
     fdptp_free_data(s->data);
     s->data      =NULL;
     s->start     =FDPTP_SOCK_START_NONE;
     s->was_closed=1;
     return NULL;
   };
   s->rem_len-=tmp;		// zaznaczamy ile mamy
 }; // while(jeszcze_pobieramy_dane)

 // koniec sciagania - zwracamy sciagniete dane
 data_ret=s->data;			// kopia do zwrocenia
 s->data =NULL;				// na wszelki wypadek :)
 s->start=FDPTP_SOCK_START_NONE;	// nastepnym razem startuj od naglowka
 return data_ret;			// zwracamy cosmy sciagneli :)
}; // fdptp_read()





int fdptp_write(struct socket *s, struct fdptp_data *data)
{
 unsigned short  len;			// dlugosc
 int             tmp;			// pomocnicza
 char           *dptr;			// wskaznik na miejsce odczytu do zap.

 assert(s   !=NULL);
 assert(data!=NULL);

 // pobieramy dlugosc danych do przslania
 //memcpy(&len, data, sizeof(len));	// pobieramy
 //len =ntohs(len);			// przechodzimy na notacje host'a
 //len+=sizeof(len);			// uwzgledniamy dlugosc

 // zapisujemy dlugosc danych do gniazda
 len=sizeof(data->len);
 tmp=write(s->s, &(data->len), len);	// zapis wlasciwy
 if(tmp!=len)				// nie udalo sie zapisac?! :/
 {
   s->was_closed=1;
   return -1;
 };

 // zapisujemy dane do gniazda
 len =ntohs(data->len);
 dptr=data->data;
 do
 {
   tmp=write(s->s, dptr, len);		// zapis wlasciwy
   if(tmp<=0)				// nie udalo sie zapisac?! :/
   {
     s->was_closed=1;
     return -1;
   };
   len -=tmp;				// zaznaczamy ile juz przeczytalismy
   dptr+=tmp;				// ...
 }
 while(len!=0);				// zapis calosci (blad jest obsluzony)

 //return ntohs(data->len);
 return 0;
}; // fdptp_write()








int fdptp_close(struct socket *s)
{
 assert(s!=NULL);

 close(s->s);		// zamykamy gniazdo
 free(s->data);		// zwalniamy bufor pomocniczy (jesli takowy jest)
 free(s);

 return 0;
}; // fdptp_close()








struct socket *fdptp_listen(char *addr, unsigned short port)
{
 struct socket      *s;         // gniazdo do zwrocenia
 struct sockaddr_in  adres;     // adres do podlczaenia

 assert(addr   !=NULL);
 assert(addr[0]!=0   );

 if( resolve_addr(addr, port, &adres)!=0 )	// sprawdzamy adres
   return NULL;

 // tworzymy strukture do zwrocenia
 s=(struct socket*)malloc( sizeof(struct socket) );
 if(s==NULL)
   return NULL;

 // otwieramy gniazdo
 s->s=socket(AF_INET, SOCK_STREAM, 0);
 if(s->s==-1)
 {
   free(s);
   return NULL;
 };

 // bindujmy adres z gniazdem
 if( bind(s->s, (struct sockaddr*)&adres, sizeof(adres) )==-1 )
 {
   close(s->s);
   free(s);
   return NULL;
 };

 if( listen(s->s, LISTEN_QUEUE_LEN)==-1 )
 {
   close(s->s);
   free(s);
   return NULL;
 };

 // na poaczatku niczego nie robilismy...
 s->start     =FDPTP_SOCK_START_NONE;
 s->data      =NULL;
 s->adres_own =adres;
 s->was_closed=0;

 // zwracamy wynik pracy :)
 return s;
}; // fdptp_listen()








struct socket *fdptp_accept_nonblock(struct socket *s)
{
 struct socket      *s_out;	// gniazdo wyjsciowe
 struct sockaddr_in  adres_rem; // adres, z ktorego przyszlo polaczenie
 int                 ar_len;    // dlugosc w/w adresu

 assert(s!=NULL);

 if( !socket_has_data(s) )	// mamy jakies polaczenie?
   return NULL;

 ar_len=sizeof(adres_rem);

 s_out=(struct socket*)malloc( sizeof(struct socket) );	// tworzymy nowa struct
 if(s_out==NULL)
   return NULL;

 s_out->s=accept(s->s, (struct sockaddr*)&adres_rem,(socklen_t*)&ar_len );
 if(s_out->s==-1)
 {
   free(s_out);
   return NULL;
 };

 // ustawiamy parametry
 s_out->adres_own =s->adres_own;
 s_out->adres_rem =adres_rem;
 s_out->data      =NULL;
 s_out->start     =FDPTP_SOCK_START_NONE;
 s_out->was_closed=0;

 return s_out;
}; // fdptp_accept_nonblock()





struct socket *fdptp_accept(struct socket *s)
{
 struct socket      *s_out;	// gniazdo wyjsciowe
 struct sockaddr_in  adres_rem; // adres, z ktorego przyszlo polaczenie
 int                 ar_len;    // dlugosc w/w adresu

 assert(s!=NULL);

 ar_len=sizeof(adres_rem);

 s_out=(struct socket*)malloc( sizeof(struct socket) );	// tworzymy nowa struct
 if(s_out==NULL)
   return NULL;

 s_out->s=accept(s->s, (struct sockaddr*)&adres_rem,(socklen_t*)&ar_len );
 if(s_out->s==-1)
 {
   free(s_out);
   return NULL;
 };

 // ustawiamy parametry
 s_out->adres_own =s->adres_own;
 s_out->adres_rem =adres_rem;
 s_out->data      =NULL;
 s_out->start     =FDPTP_SOCK_START_NONE;
 s_out->was_closed=0;

 return s_out;
}; // fdptp_accept()





void fdptp_free_data(struct fdptp_data *data)
{
 if(data==NULL)
   return;

 if(data->data)		// sa jakies dane?
   free(data->data);
 free(data);		// na koniec pozbywamy sie samej struktury
}; // fdptp_free_data()






struct fdptp_data *fdptp_mk_data(unsigned char *data, unsigned short len)
{
 struct fdptp_data *d;

 assert(data!=NULL);
 assert(data[len-1]==data[len-1]);	// pamiec jest dostepna?

 d=(struct fdptp_data*)malloc( sizeof(struct fdptp_data) );
 if(d==NULL)
   return NULL;

 d->len =htons(len);
 d->data=(char*)malloc(len);	// przydzielamy pamiec na kopie
 if(d->data==NULL)
 {
   free(d);
   return NULL;
 };

 memcpy(d->data, data, len);	// kopiujemy dane
 //d->data=data;		// NIE robimy kopii!!!

 return d;
}; // fdptp_mk_data()







struct fdptp_data fdptp_mk_data_ptr(unsigned char *data, unsigned short len)
{
 struct fdptp_data d;

 assert(data!=NULL);
 assert(data[len-1]==data[len-1]);	// pamiec jest dostepna?

 d.len =htons(len);
 d.data=data;		// NIE robimy kopii!!!

 return d;
}; // fdptp_mk_data_ptr()





int fdptp_was_closed(struct socket *s)
{
 assert(s!=NULL);

 return s->was_closed!=0;
}; // fdptp_was_closed()




