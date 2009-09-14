/*
 * wabuf.h
 *
 * WrapAroundBUFor - bufor o stalem (zalozonym w chwili
 * tworzenia) rozmiarze.
 * dostep jest podobny do kolejki FIFO, ale umozliwia
 * tez podglad (peek) do dowolnego elementu (nie
 * niszczacy).
 *
 * przed uzyciem bufor nalezy stworzyc za pomoca
 * wabuf_create(). po zakocznonym uzyciu nalezy go zwolnic
 * funkcja wabuf_free().
 *
 * elementy sa wsyawiane poprzez wartosc, tzn: kopiowanych
 * jestch odpowiednia ilosc bajtow spod podanego adresu 
 * do bufora.
 *
 */
#ifndef INCLUDE_WABUF_H_FILE
#define INCLUDE_WABUF_H_FILE


// struktura reprezentujaca bufor
struct wabuf
{
 char *buf;		// bufor wlasciwy
 int   size_bs;		// rozmiar bufora w BAJTACH
 int   size_es;		// rozmiar bufora w ilosc elementow
 int   elem_size;	// rozmiar pojedynczego elementu

 // zmienne liczone w ilosciach elementow (a nie w bajtach!!)
 int   begin;		// pozycja poczatku bufora
 int   cnt;		// ilosc elementow w buforze
}; // struct wabuf


// zwraca poprawny, pusty bufor
//   elem_size - rozmiar pojedynczego elementu
//   elems     - ilosc elementow
struct wabuf *wabuf_create(int elems, int elem_size);
// zwalnia bufor.
int wabuf_free(struct wabuf *buf);


// dodaje element do kolejki
int wabuf_add(struct wabuf *buf, const void *elem);


// usuwa element z kolejki i wstawia pod zadany adres
int wabuf_get(struct wabuf *buf, void *elem);
// pobiera element z zadanej pozycji wzgledem poczatq
// nie usuwajac go z kolejki
int wabuf_peek(const struct wabuf *buf, void *elem, int poz);


// zwraca rozmiar bufora
#define wabuf_size_el(buf)	( (buf)->size_es )
// zwraca ilosc elementow w buforze
#define wabuf_cnt(buf)	( (buf)->cnt )
// zwraca ilosc wolnych pozycji
#define wabuf_left(buf)	( wabuf_size_el(buf)-wabuf_cnt(buf) )


#endif

