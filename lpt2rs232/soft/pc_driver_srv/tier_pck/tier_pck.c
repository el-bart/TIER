/*
 * tier_pck.c
 *
 */
#include <assert.h>	// assert()
#include <time.h>	// time()

#include "tier_pck.h"


//
// FUNKCJE WEWNETRZNE MODULU
//

// stiwerdza czy dany rodzaj pakietu jest znany
#define tier_pck_is_pck_type(pt) \
		( \
			(pt)==TIER_PCK_TYPE_INIT  || \
			(pt)==TIER_PCK_TYPE_SPDST || \
			(pt)==TIER_PCK_TYPE_SPDRD || \
			(pt)==TIER_PCK_TYPE_PP    || \
			(pt)==TIER_PCK_TYPE_ACK   || \
			(pt)==TIER_PCK_TYPE_STOP  || \
		)

// zwraca sume kontrolna danego pakietu (zadanego jako ciag bajtow)
static unsigned char cnt_chk_sum(int len, const char pck[])
{
 unsigned char sum;
 int           i;

 assert(len>0);

 sum=0;
 for(i=0; i<len; i++)
   sum+=pck[i]+1;

 return sum;
}; // cnt_chk_sum()



//
// FUNKCJE GLOBALNE
//


//
// funkcje tworzace dane typy pakietow
//

struct tier_pck_all tier_pck_mk_init(void)
{
 char                pck_char[1+3];
 struct tier_pck_all pck;

 pck.type   =TIER_PCK_TYPE_INIT;
 pck_char[0]=pck.type;

 pck.pck.init.b1=0x95;
 pck_char[1]    =pck.pck.init.b1;

 pck.pck.init.b2=0xe2;
 pck_char[2]    =pck.pck.init.b2;

 pck.pck.init.b3=0x12;
 pck_char[3]    =pck.pck.init.b3;

 pck.chk_sum=cnt_chk_sum(1+3, pck_char);

 return pck;
}; // tier_pck_mk_init()


struct tier_pck_all tier_pck_mk_spdst(unsigned char track1_r,
                                      unsigned char track2_l,
                                      unsigned char track_dir)
{
 char                pck_char[1+3];
 struct tier_pck_all pck;

 pck.type   =TIER_PCK_TYPE_SPDST;
 pck_char[0]=pck.type;

 pck.pck.spdst.track1   =track1_r;
 pck_char[1]            =pck.pck.spdst.track1;
 pck.pck.spdst.track2   =track2_l;
 pck_char[2]            =pck.pck.spdst.track2;
 pck.pck.spdst.track_dir=track_dir&0x0f;	// tylko 4 mlodsze bity!
 pck_char[3]            =pck.pck.spdst.track_dir;

 pck.chk_sum=cnt_chk_sum(1+3, pck_char);

 return pck;
}; // tier_pck_mk_spdst()


/*
struct tier_pck_all tier_pck_mk_spdrd(void)
{
 char                pck_char[1+3];
 struct tier_pck_all pck;

 assert( !"SPDTD packet can be recieved, but not send by PC!" );

 pck.chk_sum=cnt_chk_sum(1+3, pck_char);

 return pck;
}; // tier_pck_mk_spdrd()
*/


struct tier_pck_all tier_pck_mk_pp(void)
{
 char                pck_char[1+3];
 struct tier_pck_all pck;

 pck.type   =TIER_PCK_TYPE_PP;
 pck_char[0]=pck.type;

 pck.pck.pp.seq_no1=(unsigned char)time(NULL);	// jakas przypadkowa liczba
 pck_char[1]       =pck.pck.pp.seq_no1;

 pck.pck.pp.seq_no2=(unsigned char)time(NULL);	// jakas przypadkowa liczba
 pck_char[2]       =pck.pck.pp.seq_no2;

 pck.pck.pp.seq_no3=(unsigned char)time(NULL);	// jakas przypadkowa liczba
 pck_char[3]       =pck.pck.pp.seq_no3;

 pck.chk_sum=cnt_chk_sum(1+3, pck_char);

 return pck;
}; // tier_pck_mk_pp()


struct tier_pck_all tier_pck_mk_ack(const struct tier_pck_all *req)
{
 char                pck_char[1+1];
 struct tier_pck_all pck;

 assert(req!=NULL);

 pck.chk_sum=cnt_chk_sum(1+1, pck_char);

 pck.type   =TIER_PCK_TYPE_ACK;
 pck_char[0]=pck.type;

 pck.pck.ack.got_chk_sum=req->chk_sum;
 pck_char[1]            =pck.pck.pp.seq_no1;
 pck_char[2]            =pck.pck.pp.seq_no2;
 pck_char[3]            =pck.pck.pp.seq_no3;

 pck.chk_sum=cnt_chk_sum(1+3, pck_char);

 return pck;
}; // tier_pck_mk_ack()


struct tier_pck_all tier_pck_mk_stop(void)
{
 char                pck_char[1+3];
 struct tier_pck_all pck;

 pck.type   =TIER_PCK_TYPE_STOP;		// 0xf0
 pck_char[0]=pck.type;

 // predefiniowana sekwencja bajtow
 pck.pck.stop.byte1=0x15;
 pck_char[1]       =pck.pck.stop.byte1;

 pck.pck.stop.byte2=0x6e;
 pck_char[2]       =pck.pck.stop.byte2;

 pck.pck.stop.byte3=0x76;
 pck_char[3]       =pck.pck.stop.byte3;

 pck.chk_sum=cnt_chk_sum(1+3, pck_char);	// 0x76

 return pck;
}; // tier_pck_mk_stop()



//
// funkcje seriazliacji/deseriazliacji pakietow
//

int tier_pck_serialize(struct tier_pck_serial *serial,
                       const struct tier_pck_all *pck)
{
 assert(serial!=NULL);
 assert(pck   !=NULL);

 // najpierw typ pakietu
 serial->size=0;
 serial->byte[ serial->size++ ]=pck->type;

 // serializacja konkretnego typu pakietu
 switch(pck->type)
 {
   case TIER_PCK_TYPE_INIT:
        serial->byte[ serial->size++ ]=pck->pck.init.b1;
        serial->byte[ serial->size++ ]=pck->pck.init.b2;
        serial->byte[ serial->size++ ]=pck->pck.init.b3;
        break;

   case TIER_PCK_TYPE_SPDST:
        serial->byte[ serial->size++ ]=pck->pck.spdst.track1;
        serial->byte[ serial->size++ ]=pck->pck.spdst.track2;
        serial->byte[ serial->size++ ]=pck->pck.spdst.track_dir;
        break;

   case TIER_PCK_TYPE_SPDRD:
        serial->byte[ serial->size++ ]=pck->pck.spdrd.track1;
        serial->byte[ serial->size++ ]=pck->pck.spdrd.track2;
        serial->byte[ serial->size++ ]=pck->pck.spdrd.track_dir;
        break;

   case TIER_PCK_TYPE_PP:
        serial->byte[ serial->size++ ]=pck->pck.pp.seq_no1;
        serial->byte[ serial->size++ ]=pck->pck.pp.seq_no2;
        serial->byte[ serial->size++ ]=pck->pck.pp.seq_no3;
        break;

   case TIER_PCK_TYPE_ACK:
        serial->byte[ serial->size++ ]=pck->pck.ack.got_chk_sum;
        break;

   case TIER_PCK_TYPE_STOP:
        serial->byte[ serial->size++ ]=pck->pck.stop.byte1;
        serial->byte[ serial->size++ ]=pck->pck.stop.byte2;
        serial->byte[ serial->size++ ]=pck->pck.stop.byte3;
        break;

   default:	// nieznany rodzaj pakietu?
        return -1;
        break;
 }; // switch(packet_type)

 // na koniec jeszcze suma kontrolna
 serial->byte[ serial->size++ ]=pck->chk_sum;

 assert( serial->size==tier_pck_size(pck->type) );

 return 0;
}; // tier_pck_serialize()



int tier_pck_deserialize(struct tier_pck_all *pck,
                         const struct tier_pck_serial *serial)
{
 int cnt;

 assert( pck!=NULL );
 assert( serial!=NULL );

 // mamy jakies dane do deserializacji?
 if( serial->size<1 )
   return -1;

 // najpierw typ pakietu
 cnt      =0;
 pck->type=serial->byte[ cnt++ ];

 // sprawdzamy czy rozmiar danego pakietu zgadza sie ze spodziewanym
 if( serial->size!=tier_pck_size(pck->type) )
   return -2;


 // deserializacja danego typu pakietu
 switch(pck->type)
 {
   case TIER_PCK_TYPE_INIT:
        break;

   case TIER_PCK_TYPE_SPDST:
        pck->pck.spdst.track1   =serial->byte[ cnt++ ];
        pck->pck.spdst.track2   =serial->byte[ cnt++ ];
        pck->pck.spdst.track_dir=serial->byte[ cnt++ ];
        break;

   case TIER_PCK_TYPE_SPDRD:
        pck->pck.spdrd.track1   =serial->byte[ cnt++ ];
        pck->pck.spdrd.track2   =serial->byte[ cnt++ ];
        pck->pck.spdrd.track_dir=serial->byte[ cnt++ ];
        break;

   case TIER_PCK_TYPE_PP:
        pck->pck.pp.seq_no1=serial->byte[ cnt++ ];
        pck->pck.pp.seq_no2=serial->byte[ cnt++ ];
        pck->pck.pp.seq_no3=serial->byte[ cnt++ ];
        break;

   case TIER_PCK_TYPE_ACK:
        pck->pck.ack.got_chk_sum=serial->byte[ cnt++ ];
        break;

   case TIER_PCK_TYPE_STOP:
        pck->pck.stop.byte1=serial->byte[ cnt++ ];
        pck->pck.stop.byte2=serial->byte[ cnt++ ];
        pck->pck.stop.byte3=serial->byte[ cnt++ ];
        break;

   default:	// nieznany rodzaj pakietu?
        return -3;
        break;
 }; // switch(packet_type)

 // na koniec jeszcze suma kontrolna
 pck->chk_sum=serial->byte[ cnt++ ];

 // suma kontrolna sie zgadza?
 if( pck->chk_sum!=cnt_chk_sum(serial->size-1, serial->byte) )
   return -4;

 return 0;
}; // tier_pck_deserialize()



int tier_pck_size(unsigned char header)
{
 switch(header)
 {
   case TIER_PCK_TYPE_INIT:  return 1+3+1;
   case TIER_PCK_TYPE_SPDST: return 1+3+1;
   case TIER_PCK_TYPE_SPDRD: return 1+3+1;
   case TIER_PCK_TYPE_PP:    return 1+3+1;
   case TIER_PCK_TYPE_ACK:   return 1+1+1;
   case TIER_PCK_TYPE_STOP:  return 1+3+1;
   default:                  return -1;		// nieznany rodzaj pakietu?
 }; // switch(packet_type)

 return 0;	// tutaj nigdy nie dochodzimy!
}; // tier_pck_size()



int tier_pck_is_response(struct tier_pck_all *cmd, struct tier_pck_all *rep)
{
 switch(cmd->type)
 {
   case TIER_PCK_TYPE_INIT:
        return rep->type   ==TIER_PCK_TYPE_ACK &&
               cmd->chk_sum==rep->pck.ack.got_chk_sum;

   case TIER_PCK_TYPE_SPDST:
        return rep->type   ==TIER_PCK_TYPE_ACK &&
               cmd->chk_sum==rep->pck.ack.got_chk_sum;

   case TIER_PCK_TYPE_SPDRD:
        return rep->type   ==TIER_PCK_TYPE_ACK &&
               cmd->chk_sum==rep->pck.ack.got_chk_sum;

   case TIER_PCK_TYPE_PP:
        return rep->type          ==TIER_PCK_TYPE_PP &&
               cmd->pck.pp.seq_no1==(unsigned char)(rep->pck.pp.seq_no1-1) &&
               cmd->pck.pp.seq_no2==(unsigned char)(rep->pck.pp.seq_no2-1) &&
               cmd->pck.pp.seq_no3==(unsigned char)(rep->pck.pp.seq_no3-1);

   case TIER_PCK_TYPE_ACK:
        return 0;

   case TIER_PCK_TYPE_STOP:
        return 0; //rep->type==TIER_PCK_TYPE_ACK;

   default:
        return 0;		// nieznany rodzaj pakietu?
 }; // switch(packet_type)

 return 0;      // tutaj nigdy nie dochodzimy!
}; // tier_pck_is_response()


