/*
 * tier_low_io.h
 *
 * deklaracja niskopoziomowego dostepu do robota TIER za
 * posrednictwem odpowiedniego adaptera LPT<->RS232.
 *
 */
#ifndef INCLUDE_TIER_LOW_IO_H_FILE
#define INCLUDE_TIER_LOW_IO_H_FILE

#include "wabuf/wabuf.h"
#include "portio/portio.h"


struct tier_low_io
{
 int           port;	// adres portu do komunikacji

 struct wabuf *rx;	// RX bufor
 struct wabuf *tx;	// TX bufor
}; // struct tier_low_io


// daje zainicjalizowana strukture tier_low_io pracujaca
// na zadanym jako parametr porcie.
// przy poprawnym wykonaniu zwraca !=NULL.
struct tier_low_io *tier_low_io_create(int port);

// zwalnia zasoby zajmowane przez strukture bazowa
int tier_low_io_free(struct tier_low_io *base);


// dodaje bajt do kolejki nadawczej
#define tier_low_io_enqueue(base, byte)	\
			( wabuf_add( (base)->tx, byte) )

// pobiera bajt z kolejki odbiorczej
#define tier_low_io_dequeue(base, byte) \
			( wabuf_get( (base)->rx, byte) )
// pobiera wartosc z bufora odbiorczego z zadanej pozycji
// jednoczesnie NIE kasujac jej z bufora
#define tier_low_io_peek(base, byte, poz)	\
			( wabuf_peek( (base)->rx, byte, poz ) )
// zwraca ilosc elementow w kolejce odbiorczej
#define tier_low_io_rx_cnt(base)	\
			( wabuf_cnt( (base)->rx ) )


// wykonuje kolejny krok w komunikacji (zarowno odbior
// jak i nadawanie danych) i zwraca ilosc przeslanych
// bajtow lub <0 w przypadku wystapienia bledu.
int tier_low_io_do_io(struct tier_low_io *base);


// ustawia port w stan poczatkowy
int tier_low_io_reset(struct tier_low_io *base);


#endif

