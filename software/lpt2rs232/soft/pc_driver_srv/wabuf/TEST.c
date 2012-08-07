/*
 * TEST.c
 *
 * prosty program przeprowadzajacy pare podstawowych
 * testow na bibliotece.
 *
 * przetesotwanie dla buf_size==3 i zmiennych typu int (4B)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "wabuf.h"


int main(void)
{
 struct wabuf *buf;
 int           val;
 int           elem;

 // tworzenie
 buf=wabuf_create(3, sizeof(val) );
 assert( wabuf_size_el(buf)==3 && wabuf_left(buf)==3 );

 // przesuniecie bufora
 val=10;
 assert( wabuf_add(buf, (char*)&val )==0 );
 assert( wabuf_get(buf, &elem)==0 && elem==val );

 // dodawanie
 val=15;	assert( wabuf_add(buf, &val)==0 );
 val=13;	assert( wabuf_add(buf, &val)==0 );
 val=11;	assert( wabuf_add(buf, &val)==0 );
 val=19;	assert( wabuf_add(buf, &val)!=0 );

 // podglad
 assert( wabuf_peek(buf, &elem, 0)==0 && elem==15 );
 assert( wabuf_peek(buf, &elem, 1)==0 && elem==13 );
 assert( wabuf_peek(buf, &elem, 2)==0 && elem==11 );
 assert( wabuf_peek(buf, &elem, 3)!=0             );

 // pobranie
 assert( wabuf_get(buf, &elem)==0 && elem==15 );
 assert( wabuf_get(buf, &elem)==0 && elem==13 );
 assert( wabuf_get(buf, &elem)==0 && elem==11 );
 assert( wabuf_get(buf, &elem)!=0             );

 return 0;
};


