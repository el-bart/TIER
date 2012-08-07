/*
 * tier_io.h
 *
 * modul kolejkujacy i zbierajacy pakiety do/z
 * robota TIER za posrednictwa warswy tier_low_io.
 * pakiety sa kreowane za pomoca tier_pck.
 *
 */
#ifndef INCLUDE_TIER_IO_H_FILE
#define INCLUDE_TIER_IO_H_FILE

#include <sys/timeb.h>

#include "tier_low_io/tier_low_io.h"
#include "tier_pck/tier_pck.h"




// struktura pojedynczego pakietu do wyslania
struct tier_io_handler
{
 struct tier_pck_all pck_cmd;	// nadawany pakiet
 struct tier_pck_all pck_rep;	// otrzymana odpowiedz

 char                ok;	// wymiana przebiegla pomyslnie?

 struct timeb        timeout;	// po uplywie tego czasu pakiet jest
				// zwracany z bledem (!ok)
}; // struct tier_io_handler


// informuje czy odpowiedz jest poprawna
#define tier_io_handler_ok(h)		( (h)->ok )







// glowna struktura I/O
struct tier_io
{
 struct tier_low_io     *comm;	// komunikacja z robotem

 struct tier_io_handler  h;	// uchwyt do komunikacji (konkretny pakiet)
}; // struct tier_io




//
// funkcje operujace na samej strukturze
//

// storzy strukture
struct tier_io *tier_io_create(int port);
// zwalnia strukture
int tier_io_free(struct tier_io *tio);

// wykonuje kolejny etap pracy na kolejce
int tier_io_do_comm(struct tier_io *tio);

// resetuje kolejke
int tier_io_queue_reset(struct tier_io *tio);


//
// fukncke wysylajace konkretne pakiety
//

// inicjalizuje komunikajce z robotem
int tier_io_init(struct tier_io *tio);
// ustawia predkosci gasiennic
//   left/right <-255;+255> (0==stop)
int tier_io_spdst(struct tier_io *tio, int left, int right);
// pobiera predkosci gasiennic
//   left/right <-255;+255> (0==stop)
int tier_io_spdrd(struct tier_io *tio, int *left, int *right);
// ping-pong
int tier_io_ping(struct tier_io *tio);
// reset robota
int tier_io_stop(struct tier_io *tio);



#endif

