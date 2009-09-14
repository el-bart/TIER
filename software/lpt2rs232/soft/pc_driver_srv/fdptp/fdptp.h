/*
 * fdptp.h
 *
 * FullDataPartsTransferProtocol
 * protokol dba o to, aby "paczka" doszla w calosci, nie zaleznie od
 * jej rozmiarow (zeby uniknac odbierania "na czesci").
 *
 */
#ifndef INCLUDE_FDPTP_H
#define INCLUDE_FDPTP_H
#include <netinet/in.h> // struct sockaddr_in;




// struktura opisujaca gniazdo protokolu fdptp
struct socket
{
 int                s;		// gniazdo TCP/IP
 struct fdptp_data *data;	// czesc danych juz odebrana (nic==NULL)
 unsigned short     total_len;	// dlugosc do pobrania
 unsigned short     rem_len;	// dlugosc pozostala do pobrania

 unsigned short     start;	// dlugosc poczatkowa - przy rozpoczeciu nowego
				// pobierania jest tu wpisane X, poniewaz
				// oczekujemy na X bajtow (X==sizeof(total_len)
				// zeby wiedziec ile danych mamy do odebrania.

 struct sockaddr_in adres_own;	// wlasny adres sieciowy
 struct sockaddr_in adres_rem;	// adres drugiej strony lacza :)

 char               was_closed;	// informacja czy gniazdo zostalo zamkniete
				// po drugiej stronie lacza
};



// struktura danych "przesylana" przez gniazdo
struct fdptp_data
{
 unsigned short  len;		// ilosc bajtow danych (zapis w notacji sieci)
 unsigned char  *data;		// dane w postaci zserializowanej
};





// funkcja laczy sie pod adres addr:port
struct socket *fdptp_connect(char *addr, unsigned short port);

// odczytuje dane z gniaza - odczyt nie blokujacy. w przypadku nie
// odczytania calosci zwraca NULL, lecz zapamietuje dane juz przeczytane.
// zwraca dane w postaci serializowanej
struct fdptp_data *fdptp_read_nonblock(struct socket *s);

// wersja blokujaca w/w. zawsze zwraca dane, lub blad.
struct fdptp_data *fdptp_read(struct socket *s);

// zapisuje dane z postaci zserializowanej do gniazda
int fdptp_write(struct socket *s, struct fdptp_data *data);

// zamyka gniazdo protokolu fdptp
int fdptp_close(struct socket *s);

// inicjuje przujmowanie polaczen pod damym adresem & portem
struct socket *fdptp_listen(char *addr, unsigned short port);

// zwraca kolejne polaczenie, jesli takowe wystapilo (funkcja nie blokujaca)
// NULL oznacza, ze nie otrzymalismy zadnego polaczenia
struct socket *fdptp_accept_nonblock(struct socket *s);

// zwraca kolejne polaczenie. funkcja blokujace.
struct socket *fdptp_accept(struct socket *s);

// zwalnia pamiec zajmowana przez strukture danych
void fdptp_free_data(struct fdptp_data *data);

// tworzy strukture do wyslania - dane przekazane jako parametr
// SA KOPIOWANE! dlugosc len jest podawana w notacji host'a!
struct fdptp_data *fdptp_mk_data(unsigned char *data, unsigned short len);

// tworzy strukture do wyslania - dane przekazane jako parametr
// NIE SA KOPIOWANE - jedynie wskaznik jest przypisany! dlugosc len jest
// podawana w notacji host'a!
struct fdptp_data fdptp_mk_data_ptr(unsigned char *data, unsigned short len);

// zwraca ile bajtow posiadaja dane w strukturze
#define fdptp_data_len(data)	( ntohs( (data)->len ) )

// zwraca informacje czy gniazdo zostalo zamkniete po drugiej stronie
// komunikacji - jezeli zwroci prawde, nalezy zamknac gniazdo.
int fdptp_was_closed(struct socket *s);



#endif

