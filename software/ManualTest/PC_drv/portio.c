/*
 * portio.c
 *
 */
#include <sys/io.h>	// outb(); intb(); ioperm();

#include "portio.h"


// bity
#define BIT0	(1<<0)
#define BIT1	(1<<1)
#define BIT2	(1<<2)
#define BIT3	(1<<3)
#define BIT4	(1<<4)
#define BIT5	(1<<5)
#define BIT6	(1<<6)
#define BIT7	(1<<7)




// ustawia bity tak, aby stan wyoski dla programu zawsze oznaczal 1 a niski 0
// btype moze byc 'd' dla bajtu danych, 'c' dla kontrolnego bajtu lub 's' dla bajtu statusu
static unsigned char correct_states(unsigned char byte, unsigned char btype)
{
 switch(btype)
 {
   case 'c': // zmieniamy bity: 3, 0, 1
             if( (byte&BIT3)==BIT3 )	// bit jest 1
               byte&=(~BIT3);
             else			// bit jest 0
               byte|=BIT3;

             if( (byte&BIT0)==BIT0 )    // bit jest 1
               byte&=(~BIT0);
             else                       // bit jest 0
               byte|=BIT0;

             if( (byte&BIT1)==BIT1 )    // bit jest 1
               byte&=(~BIT1);
             else                       // bit jest 0
               byte|=BIT1;

             return byte;

   case 's': // zmieniamy bit: 7
             if( (byte&BIT7)==BIT7 )    // bit jest 1
               return byte&(~BIT7);
             else                       // bit jest 0
               return byte|BIT7;

   default:  // ???
             return -1;

/*   case 'd': // nic nie ptrzeba poprawiac
             return byte;*/
 };
 return 0;	// zeby kompilator sie nie czepial
};



int portio_open(unsigned short port)
{
 return ioperm(port, 3, 1);
}; // portio_open()




int portio_write_data(unsigned short port, unsigned char byte)
{
// byte=correct_states(byte, 'd');
 outb(byte, port);
 return 0;
}; // portio_write_data()




int portio_read_data(unsigned short port, unsigned char *byte)
{
 *byte=inb(port);
// *byte=correct_states(*byte, 'd');
 return 0;
}; // portio_read_data()




int portio_write_control(unsigned short port, unsigned char byte)
{
 byte=correct_states(byte, 'c');
 outb(byte, port+2);
 return 0;
}; // portio_write_control()




int portio_read_control(unsigned short port, unsigned char *byte)
{
 *byte=inb(port+2);
 *byte=correct_states(*byte, 'c');
 return 0;
}; // port_read_control()




int portio_read_status(unsigned short port, unsigned char *byte)
{
 *byte=inb(port+1);
 *byte=correct_states(*byte, 's');
 return 0;
}; // portio_read_status()




int portio_close(unsigned short port)
{
 return ioperm(port, 3, 0);
}; // portio_close()

