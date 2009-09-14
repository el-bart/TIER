/*
 * portio.h
 *
 * funkcje zapisujace dane do portu.
 * neguje wartosc wysylana/odbierana, jesli stan niski
 * oznacza 1 na styku, * tak wiec z punktu widzenia
 * uzytkownika stan wysoki ZAWSZE jest 1 a niski 0.
 *
 */
#ifndef INCLUDE_PORTIO_H_FILE
#define INCLUDE_PORTIO_H_FILE


// typowe porty
#define PORTIO_LPT0	0x378
#define PORTIO_LPT1	0x278


// niektore bity jako stale
#define PORTIO_CTRL_nWR	(1<<5)



// otwiera port
int portio_open(unsigned short port);
// zamyka port
int portio_close(unsigned short port);


// zapisuje dane do portu
int portio_write_data(unsigned short port, unsigned char byte);
// pobiera dane z portu
int portio_read_data(unsigned short port, unsigned char *byte);


// zapisuje dane do czesci kontrolnej portu
int portio_write_control(unsigned short port, unsigned char byte);
// pobiera dane z czesci kontrolnej portu
int portio_read_control(unsigned short port, unsigned char *byte);


// pobiera dane z czesci statusowej portu
int portio_read_status(unsigned short port, unsigned char *byte);



#endif

