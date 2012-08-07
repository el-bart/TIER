/*
 * tier_pck.h
 *
 * modul przetwarzajacy poszczegolne pakiety 
 * robota TIER (zarowno nadawane jak i odbierane).
 *
 */
#ifndef INCLUDE_TIER_PCK_H_FILE
#define INCLUDE_TIER_PCK_H_FILE


// rozmiar bufora serializacji (minimum to 
// sorzmiar_pakietu+2!)
#define TIER_IO_SERIAL_MAX	(5+2)


//
// rozpoznawane typy pakietow
//
#define TIER_PCK_TYPE_INIT	0x0f
#define TIER_PCK_TYPE_SPDST	0x1e
#define TIER_PCK_TYPE_SPDRD	0x2d
#define TIER_PCK_TYPE_PP	0x3c
#define TIER_PCK_TYPE_ACK	0x4b
#define TIER_PCK_TYPE_STOP	0xf0





//
// struktury reprezentujaca poszczegolne rodzaje pakietow
// obslugiwanych przez protoql
//

struct tier_pck_init
{
}; // struct tier_pck_init


struct tier_pck_spdst
{
 unsigned char track1;
 unsigned char track2;
 unsigned char track_dir;
}; // struct tier_pck_spdst


struct tier_pck_spdrd
{
 unsigned char track1;
 unsigned char track2;
 unsigned char track_dir;
}; // struct tier_pck_spdrd


struct tier_pck_pp
{
 unsigned char seq_no;
}; // struct tier_pck_pp


struct tier_pck_ack
{
 unsigned char got_chk_sum;
}; // struct tier_pck_ack


struct tier_pck_stop
{
 unsigned char byte1;
 unsigned char byte2;
}; // struct tier_pck_stop


//
// unia laczaca wszystkie rodzaje pakietow w 1
//
struct tier_pck_all
{
 unsigned char type;
 union
 {
   struct tier_pck_init  init;
   struct tier_pck_spdst spdst;
   struct tier_pck_spdrd spdrd;
   struct tier_pck_pp    pp;
   struct tier_pck_ack   ack;
   struct tier_pck_stop  stop;
 } pck;
 unsigned char chk_sum;
}; // struct tier_pck_all


//
// struktura reprezentujaca pakiet w postaci
// zserializowanej wraz z podanym rozmiarem
//
struct tier_pck_serial
{
 unsigned char byte[TIER_IO_SERIAL_MAX];	// dane
 int           size;				// ich ilosc
}; // struct tier_pck_serial



//
// funkcje tworzace dane typy pakietow
//

// pakiet init
struct tier_pck_all tier_pck_mk_init(void);
// pakiet ustaenienia szybkosci
struct tier_pck_all tier_pck_mk_spdst(unsigned char track1_r,
                                      unsigned char track2_l,
                                      unsigned char track_dir);
// pakiet odczytu ustawien predkosci
//struct tier_pck_all tier_pck_mk_spdrd(void);
// ping-pong (request)
struct tier_pck_all tier_pck_mk_pp(void);
// potwierdzenie pakietu
struct tier_pck_all tier_pck_mk_ack(const struct tier_pck_all *req);
// stop (resetuje uC)
struct tier_pck_all tier_pck_mk_stop(void);


//
// funkcje serializacji/deserializacji pakietow
//

// serializacja
int tier_pck_serialize(struct tier_pck_serial *serial,
                       const struct tier_pck_all *pck);
// deserializacja
int tier_pck_deserialize(struct tier_pck_all *pck,
                         const struct tier_pck_serial *serial);


//
// funkcje/makra weryfikujace sam naglowek (typ) pakietu
//

// sprawdza czy podany naglowek jest popranwy skladniowo
#define tier_pck_header_syntax(header)	\
		(  ( ((header)&0xf0)>>4 )==( (~(header))&0x0f )  )
// sprawdza czy naglowek jest znany
#define tier_pck_header_type_ok(header)	\
		(\
			(header)==TIER_PCK_TYPE_INIT  || \
			(header)==TIER_PCK_TYPE_SPDST || \
			(header)==TIER_PCK_TYPE_SPDRD || \
			(header)==TIER_PCK_TYPE_PP    || \
			(header)==TIER_PCK_TYPE_ACK   || \
			(header)==TIER_PCK_TYPE_STOP     \
                )
// sprawdza czy podany naglowek jest naglowkiem istniejacego pakietu
#define tier_pck_header_ok(header)	\
		( tier_pck_header_syntax(header) && \
                  tier_pck_header_type_ok(header)     )

// zwraca rozmiar danego pamietu, wliczajac w to naglowek
// i sume kontrolna.
int tier_pck_size(unsigned char header);

// sprawdza czy dany pakiet jest odpowidzia na pakiet wejsciowy
int tier_pck_is_response(struct tier_pck_all *cmd, struct tier_pck_all *rep);


#endif

