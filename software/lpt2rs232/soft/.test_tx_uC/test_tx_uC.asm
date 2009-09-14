;
;
; sterownik przejsciowki LPT -> RS232
;
; program przewidziano na mikrokontroler at89c51 z kwarcem
; 11.059MHz.
; transmisja odbywa sie za posrednictwem wbudowanego portu
; rs232 z parametrami:
;   /*szybkosc:        57600 bps*/
;   szybkosc:        9600 bps
;   bity stopu:      1
;   bity startu:     1
;   bity danych:     8
;   bit parzystosci: brak
;
;
; rozklad pin'ow:
; P0:
;   P0.0      pwr LED
;
; P1:
;
; P2:
;   P2.0-P2.7 uniwersjalne 8-bit I/O
;
; P3:
;   P3.0      rs232 - RX
;   P3.1      rs232 - TX
;      LPT ("PC" - steruje PC; "uC" - sterujemy my):
;   P3.4 ( 1) PCdataReady
;   P3.5 (14) PCdataAcc
;   P3.3 (10) uCdataReady
;   P3.2 (11) uCdataAcc
;
;
; transmisja LPT: uC -> PC:
;   1. PCdataReady=L
;   2. uCdataAcc  =L
;   3. PCdataReady=H
;   4. uCdataAcc  =H
;
; transmisja LPT: uC <- PC:
;   1. uCdataReady=L
;   2. PCdataAcc  =L
;   3. uCdataReady=H
;   4. PCdataAcc  =H
;
; transmisjca uC -> PC ma wyzszy priorytet!
;




;*******************;
;*                 *;
;* uzywane symbole *;
;*                 *;
;*******************;

; diody LED:
led_pwr  EQU	P0.0

; nastaw licznika T1 dla transmiscji rs232:
;rs232_bps EQU	57600			; predkosc transmisji (w bodach)
rs232_bps EQU	9600			; predkosc transmisji (w bodach)
t1_set    EQU	(256-57600/rs232_bps)	; nastaw licznika

; wyprowadzenia do transmisji po LPT
lpt_data        EQU	P2
lpt_PC_data_rdy EQU	P3.4
lpt_PC_data_acc EQU	P3.5
lpt_uC_data_rdy EQU	P3.3
lpt_uC_data_acc EQU	P3.2


;*******************************;
;*                             *;
;* uzywane obszary pamieci RAM *;
;*                             *;
;*******************************;

; flagi bitowe:
flg_tx_now  EQU		0x20	; 1 oznacza ze uklad jest w trakcie nadawania
				; przez rs232
flg_lpt_has EQU		0x21	; informacja czy otrzymano dane z portu LPT

; bufory I/O rs232 - po 16B kazdy
rs_rx_buf  EQU		0x40	; 0x40<->0x4f wlacznie
rs_tx_buf  EQU		0x50	; 0x50<->0x5f wlacznie
; zakresy RX
rs_rx_beg  EQU		0x3e	; poczatek bufora odbioru
rs_rx_cnt  EQU		0x3f	; dlugosc bufora odbioru
; zakresy TX
rs_tx_beg  EQU		0x3c	; poczatek bufora nadawania
rs_tx_cnt  EQU		0x3d	; dlugosc bufora nadawania



;*********************;
;*                   *;
;* POCZATEK PROGRAMU *;
;*                   *;
;*********************;
ljmp start		; pomijamy poczatkowe obslugi przerwan

;
; OBSLUGA PRZERWANIA rs232
;
org 0023h
ljmp int_rs232



;
; FUNKCJA OBSLUGI PRZERWANIA rs232
;
int_rs232:

    ; na poczatek zachowujemy uzywane rejestry
    push PSW
    push ACC

    ;
    ; koniec nadawania danych?
    ;
  jnb TI,rs_ti_end		; nie koniec nadawania?
    clr TI			; usuwamy znacznik przerwania
    clr flg_tx_now		; zaznaczamy, ze nie nadajemy juz
  rs_ti_end:

    ;
    ; koniec odbierania danych?
    ;
  jnb RI,rs_ri_end		; nie koniec odbierania?
    clr RI			; dalej nie uzywamy tej flagi
    mov A,SBUF			; pobieramy bajt
;  lcall rs232_rx_enqueue	; wstawiamy do kolejki
  rs_ri_end:

    ; przywracamy ustawienia
    push ACC
    push PSW

reti








;***********************************;
;*                                 *;
;* ROZPOCZECIE PROGRAMU WLASCIWEGO *;
;* (inicjalizacja)                 *;
;*                                 *;
;***********************************;
start:

    ; na poczatek zerujemy wszystkie porty:
    mov P0,#0x00
    mov P1,#0x00
    mov P2,#0x00
    mov P3,#0x00

    ; ustawiamy wartosci poczatkowe flag
    clr flg_tx_now		; nie nadajemy niczego jak narazie :)
    clr flg_lpt_has		; niczego nie otrzymalismy przez LPT

    ; ustawianie buforow I/O dla rs232
    mov rs_rx_beg,#0x00
    mov rs_rx_cnt,#0x00
    mov rs_tx_beg,#0x00
    mov rs_tx_cnt,#0x00

    ; inicjalizacja portu LPT:
    setb lpt_PC_data_rdy	; odczyt
    setb lpt_PC_data_acc	; ...
    setb lpt_uC_data_rdy	; ...
    setb lpt_uC_data_acc	; ...
    mov  lpt_data,#0xff		; przygotowujemy do odbioru

    ; inicjalizacja portu rs232:
    setb P3.0			; musimy umowliziwc nadawanie i odbior
    setb P3.1			; ...
    mov SCON,#01010000b		; rs232 w tryb 1 + zezwolenie na odbior
    orl PCON,#10000000b		; SMOD=1
    mov TMOD,#00100000b		; timer T1 w trybie 2
    mov TH1, #t1_set		; nastaw timer'a T1
    mov TL1, #t1_set		; oraz wartosc startowa
    ;mov TL1, #0x00		; oraz wartosc startowa
    ; flagi przerwania rs232
    clr  RI			; nie ma przerwania po odbiorze! :)
    clr  TI			; ani po nadawaniu!
    ; koniec ustawien - czas rozpoczac prace wlasciwa
    setb TR1			; uruchamiamy T1

    ; ustawiamy priorytety przerwan - przerwanie rs232 najwazniejsze!
    mov  IP,#0x00		; zerujemy priorytety
    setb PS			; ustawamy bit rs232

    ; wlaczamy uzywane przez nas przerwania:
;    setb ES			; przerwanie z transmisji szeregowej
;    setb EA			; globalne zezwolenie przerwania

    ; zaznaczamy, ze uklad pracuje poprawnie
    clr led_pwr


;*************************;
;*                       *;
;* GLOWNA PETLA PROGRAMU *;
;*                       *;
;*************************;
main:

    lcall delay_1000ms		; czekamy 1s
      clp led_pwr		; mrugniecie dioda
    lcall tx_ff			; wysylamy rozkaz

ljmp main
;******************************;
;*                            *;
;* KONIEC PROGRAMU WLASCIWEGO *;
;*                            *;
;******************************;



;***********************************;
;*                                 *;
;* FUNKCJE POMOCNICZE DLA PROGRAMU *;
;*                                 *;
;***********************************;



delay_1000ms:

    mov R7,#14
  loop_d1:
    mov R6,#255
  loop_d2:
    mov R5,#255
  djnz R5,$
  djnz R6,loop_d2
  djnz R7,loop_d1
    
ret


; obie gasiennice naprzod
; 0x1e  0xff 0x7f 0x05  0xa5
tx_ff:

    ; blad
    clr TI
    mov SBUF,#0x42
  jnb TI,$

    ; naglowek (ok, ale za wczesnie!)
    clr TI
    mov SBUF,#0x1e
  jnb TI,$

    ; blad 
    clr TI
    mov SBUF,#0x29
  jnb TI,$

    ; dalej jest juz ok

    ; naglowek
    clr TI
    mov SBUF,#0x1e
  jnb TI,$

    ; silnik 1
    clr TI
    mov SBUF,#0xff
  jnb TI,$

    ; silnik 2
    clr TI
    mov SBUF,#0x7f
  jnb TI,$

    ; kierunki
    clr TI
    mov SBUF,#0x05
  jnb TI,$

    ; suma kontrolna
    clr TI
    mov SBUF,#0xa5
  jnb TI,$

ret



