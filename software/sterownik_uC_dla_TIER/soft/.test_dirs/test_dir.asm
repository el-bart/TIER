;
;
; testowy sterownik mikrokontrolera robota TIER
;
; program przewidziano na mikrokontroler at89c51 z kwarcem
; 11.059MHz.
; program co okolo 1 sekunde zmienia biegunowosc 1 z silniqw.
;
; rozklad pin'ow:
; P0:
;   P0.0      sterowanie silnikiem 0
;   P0.1      ...
;   P0.2      sterowanie silnikiem 1
;   P0.3      ...
;   P0.4      led - power on
;   P0.5      led - data send/recv
;   P0.6      led - unused0
;   P0.7      led - unused1
;
; P1:
;   P1.0-P1.7 uniwersjalne 8-bit I/O
;
; P2:
;   P2.0-P2.7 uniwersjalne 8-bit I/O
;
; P3:
;   P0.0      rs232 - RX
;   P0.1      rs232 - TX
;   P0.2-P0.7 univ. 6-bit I/O
;
;




;*******************;
;*                 *;
;* uzywane symbole *;
;*                 *;
;*******************;

; sterowanie silnikami:
eng0_f   EQU	00000001b	; silnik pierwszy - naprzod
eng0_b   EQU	00000010b	; silnik pierwszy - wstecz
eng1_f   EQU	00000100b	; silnik drugi    - naprzod
eng1_b   EQU	00001000b	; silnik drugi    - wstecz

; diody LED:
led_pwr  EQU	00010000b	; dioda od zasilania
led_rxtx EQU	00100000b	; dioda transmisji RX/TX
led_un0  EQU	01000000b	; nieuzywana dioda 0
led_un1  EQU	10000000b	; nieuzywana dioda 1

; ilosc przebiegow potrzebnych do pelnego cyklu silniqw
eng_cycles EQU	240		; ...

; nastaw licznika T1 dla transmiscji rs232:
rs232_bps EQU	57600			; predkosc transmisji (w bodach)
t1_set    EQU	(256-57600/rs232_bps)	; nastaw licznika


;*******************************;
;*                             *;
;* uzywane obszary pamieci RAM *;
;*                             *;
;*******************************;

; licznik cykli przerwania silniqw:
eng_cycle_now EQU	0x31	; ...

; kolejne stany silniqw:
eng_poz     EQU	0x32		; 0x32<->0x39



;*********************;
;*                   *;
;* POCZATEK PROGRAMU *;
;*                   *;
;*********************;
nop
nop
ljmp start		; pomijamy poczatkowe obslugi przerwan

;
; OBSLUGA PRZERWANIA TIMERa T0
;
org 000bh
ljmp int_t0


;
; FUNKCJA OBSLUGI PRZERWANIA TIMERa T0
; (sterowanie silnikami)
;
int_t0:

    ; zapisujemy stan rejestrow
    push PSW
    push ACC

    ; sprawdzamy czy silnik 0 juz zakonczyl?
    mov  A,#0xff		; pobieramy wartosc do osiagniecia
    clr  C			; zerujemy bit przeniesienia
    subb A,eng_cycle_now	; odejmujemy zeby porownac
  jnz eng_skip			; juz licznik osiagniety?
    inc  R5
    mov  A,#(4*14)		; pobieramy wartosc max.
    clr  C
    subb A,R5			; sprawdzamy czy przepelnienie?
  jnz eng_skip			; nie ma przepelnienia?
    mov R5,#0x00		; resetujemy index 2

    ; zmiana pozycji - do dziela
    mov A,B			; pobieramy index
    add A,#eng_poz		; dodajemy poczatek
    mov R0,A
    mov P0,@R0			; wstawiamy nowa wartosc
    inc B			; zwiekszamy index
    mov A,B
    clr C
    subb A,#8			; index poza tablica?
  jnz eng_skip
    mov B,#0			; jak tak to lecimy od poczatq
  jmp eng_skip

  eng_skip:
    inc  eng_cycle_now		; odliczamy kolejny cykl

    ; przywracamy rejestry
    pop ACC
    pop PSW

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


    ; inicjalizacja ustawien timer'a T0
    mov TL0, #0x00		; wartosc poczatkowa
    mov TH0, #0x00		; wartosc do przeladowania
    anl TMOD,#0xfc		; ...
    orl TMOD,#0x02		; 2 tryb pracy 

    ; ustawienie sterowania silnikami:
    mov eng_cycle_now,#0x00	; jeszcze nie odbyto zadnego cyklu
    mov R0,#eng_poz
    mov @R0,#00010001b
    inc R0
    mov @R0,#00100010b
    inc R0
    mov @R0,#01000100b
    inc R0
    mov @R0,#10001000b
    inc R0
    mov @R0,#01010101b
    inc R0
    mov @R0,#10101010b
    inc R0
    mov @R0,#01100110b
    inc R0
    mov @R0,#10011001b

    mov B, #0x00		; index 1
    mov R5,#0x00		; index 2

    ; wlaczamy uzywane przez nas przerwania:
    setb ET0			; przerwanie od timera T0
    setb EA			; globalne zezwolenie przerwania

    ; wlaczamy liczniki
    setb TR0

    ; zaznaczamy, ze uklad pracuje poprawnie
    mov P0,#led_pwr		; wystawiamy nasze ustawnienia


;*************************;
;*                       *;
;* GLOWNA PETLA PROGRAMU *;
;*                       *;
;*************************;
main:

    mov R1,TL0
    mov R2,TH0





ljmp main
;******************************;
;*                            *;
;* KONIEC PROGRAMU WLASCIWEGO *;
;*                            *;
;******************************;



