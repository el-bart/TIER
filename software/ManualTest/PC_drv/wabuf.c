/*
 * wabuf.c
 *
 */
#include <stdlib.h>	// NULL
#include <assert.h>	// assert()
#include <string.h>	// memcpy()

#include "wabuf.h"



struct wabuf *wabuf_create(int elems, int elem_size)
{
 struct wabuf *buf;

 assert(elems    >0);
 assert(elem_size>0);

 buf=(struct wabuf*)malloc( sizeof(struct wabuf) );
 if(buf==NULL)
   return NULL;

 // zaznaczamy parametry bufora
 buf->size_bs  =elems*elem_size;
 buf->size_es  =elems;
 buf->elem_size=elem_size;
 // przydzielamy pamiec na bufor wewnetrzny:
 buf->buf  =(char*)malloc(buf->size_bs);
 if(buf->buf==NULL)
 {
   free(buf);
   return NULL;
 };
 // ustalamy ze bufor jest pusty
 buf->begin=0;
 buf->cnt  =0;

 return buf;
}; // wabuf_create()



int wabuf_free(struct wabuf *buf)
{
 assert(buf!=NULL);

 free(buf->buf);
 free(buf);

 return 0;
}; // wabuf_free()



int wabuf_add(struct wabuf *buf, const void *elem)
{
 int poz;		// element position
 int poz_byte;		// pozycja w bajtach
 //int size_byte;		// rozmiar w bajtach

 assert(buf !=NULL);
 assert(elem!=NULL);

 if( wabuf_left(buf)==0 )		// bufor is full?
   return -1;

 poz_byte=(buf->begin+buf->cnt)*buf->elem_size;
 //size_byte=buf->size_es*buf->elem_size;
 poz=poz_byte%buf->size_bs;		// pozycja wstawienia
 memcpy(buf->buf+poz, elem, buf->elem_size);	// kopiujemy
 buf->cnt++;				// zwiekszamy ilosc elementow

 return 0;
}; // cbuf_add()




int wabuf_get(struct wabuf *buf, void *elem)
{
 int poz;

 assert( buf !=NULL );
 assert( elem!=NULL );

 if( wabuf_cnt(buf)==0 )	// nie ma wiecej elementow?
   return -1;

 poz       =buf->begin*buf->elem_size;
 memcpy(elem, buf->buf+poz, buf->elem_size);	// pobieranie
 buf->begin=(buf->begin+1)%buf->size_es;	// przenosimy poczatek
 buf->cnt--;			// zmniejszamy ilosc elementow

 return 0;
}; // cbuf_get()



int wabuf_peek(const struct wabuf *buf, void *elem, int poz)
{
 assert( buf !=NULL );
 assert( elem!=NULL );
 assert( poz >=0    );

 if( buf->cnt<=poz )	// nie ma tyle elementow?
   return -1;

 // wyznaczamy pozycje wzgledem poczatku:
 poz  =( (buf->begin+poz)*buf->elem_size )%buf->size_bs;	
 memcpy(elem, buf->buf+poz, buf->elem_size);	// i pobieramy!

 return 0;
}; // cbuf_peek()


