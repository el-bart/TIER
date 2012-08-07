/*
 * tier_fdptp_comm.h
 *
 * abstrakcyjny protoql komunikacji pomiedzy klientem
 * chcacym sterowac TIER'em a serverem realizujacym
 * te usluge.
 *
 */
#ifndef INCLUDE_TIER_FDPTP_COMM_H_FILE
#define INCLUDE_TIER_FDPTP_COMM_H_FILE


//
// definicja typow pakietow:
//

// ustawienie predkosci
#define TIER_FDPTP_COMM_PCK_SPDST	1
// odczyt predkosci
#define TIER_FDPTP_COMM_PCK_SPDRD	2
// inicjalizacja
#define TIER_FDPTP_COMM_PCK_INIT	3
// zatrzymanie + reset
#define TIER_FDPTP_COMM_PCK_STOP	4
// ping
#define TIER_FDPTP_COMM_PCK_PING	5
// potwierdzenie przyjecia rozkazu
#define TIER_FDPTP_COMM_PCK_OK		6
// blad podczas odbierania odpowiedzi
#define TIER_FDPTP_COMM_PCK_ERROR	7



// pakiet uzywany podczas komunikacji
struct tier_fdptp_comm_pck
{
 int type;		// typ pakietu


 struct
 {
   int left;
   int right;
 } spd;			// SPDST/SPDRD

 struct
 {
   int val;
 } ping;		// PING

}; // struct tier_fdptp_comm_pck





#endif

