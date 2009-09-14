/*
 * tier_io_client.h
 *
 * interface do uzywania przez klienta serwera
 * tier_io_server. implementuje wszystkie rozkazy, jakie
 * mozna wyda robotowi.
 * rozkazy sa przesylane poprzez fdptp.
 *
 */
#ifndef INCLUDE_TIER_IO_CLIENT_H_FILE
#define INCLUDE_TIER_IO_CLIENT_H_FILE

#include <sys/timeb.h>

#include "fdptp/fdptp.h"
#include "tier_fdptp_comm.h"


// struktura uzywana przez klienta
struct tier_io_client
{
 struct socket *s;		// gniazdo fdptp
 struct timeb   timeout;	// domyslny timeout
}; // struct tier_io_client




// tworzy nowa strukture do komunikacji
struct tier_io_client *tier_io_client_create(char *addr, unsigned short port);
// zwalnia strukture komunikacyjna
int tier_io_client_free(struct tier_io_client *tio_cln);


// wysyla zadanie ustawienia szybkosci
int tier_io_client_set_speed(struct tier_io_client *tio_cln,
                             int left, int right);
// wysyla zadanie pobrania nastawow predkosci
int tier_io_client_get_speed(struct tier_io_client *tio_cln,
                             int *left, int *right);
// wysyla zadanie inicjalizacji (powitanie)
int tier_io_client_init(struct tier_io_client *tio_cln);
// zatrzymanie robota + reset
int tier_io_client_stop(struct tier_io_client *tio_cln);
// ping-pong
int tier_io_client_ping(struct tier_io_client *tio_cln);



#endif

