;
;
; sterownik przejsciowki LPT -> RS232
;
; program przewidziano na mikrokontroler at89c51 z kwarcem
; 11.059MHz.
; transmisja odbywa sie za posrednictwem wbudowanego portu
; rs232 z parametrami:
;   /*szybkosc:        57600 bps*/
;   /*szybkosc:        9600 bps*/
;   szybkosc:        4800 bps
;   bity stopu:      1
;   bity startu:     1
;   bity danych:     8
;   bit parzystosci: brak
;
;
; rozklad pin'ow:
; P0:
;   P0.0-P0.7 I/O 8-bit data
;
; P1:
;
; P2:
;   P2.7      pwr LED
;
; P3:
;   P3.0      rs232 - RX
;   P3.1      rs232 - TX
;      LPT ("PC" - steruje PC; "uC" - sterujemy my):
;   P3.2 ( 1) PCdataReady
;   P3.3 (14) PCdataAcc
;   P3.4 (10) uCdataReady
;   P3.5 (11) uCdataAcc
;
;
; transmisja LPT: uC <- PC:
;   1. PCdataReady=L
;   2. uCdataAcc  =L
;   3. PCdataReady=H
;   4. uCdataAcc  =H
;
; transmisja LPT: uC -> PC:
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
;rs232_bps EQU	9600			; predkosc transmisji (w bodach)
rs232_bps EQU	4800			; predkosc transmisji (w bodach)
t1_set    EQU	(256-57600/rs232_bps)	; nastaw licznika

; wyprowadzenia do transmisji po LPT
lpt_data        EQU	P0
lpt_PC_data_rdy EQU	P3.2
lpt_PC_data_acc EQU	P3.3
lpt_uC_data_rdy EQU	P3.4
lpt_uC_data_acc EQU	P3.5


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
    mov A,rs_rx_cnt		; pobieramy ilosc danych w kolejce
    clr C			; od odejmowania
    subb A,#0x10		; sprawdzamy czy mamy juz limit
  jz rs_ri_end			; jezeli mamy koniec bufora to nie
				; odbieramy dalszych bajtow danych
    mov A,SBUF			; pobieramy bajt
  lcall rs232_rx_enqueue	; wstawiamy do kolejki
  rs_ri_end:

    ; przywracamy ustawienia
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
    setb ES			; przerwanie z transmisji szeregowej
    setb EA			; globalne zezwolenie przerwania

    ; zaznaczamy, ze uklad pracuje poprawnie
    clr led_pwr


;*************************;
;*                       *;
;* GLOWNA PETLA PROGRAMU *;
;*                       *;
;*************************;
main:

    ; sprawdzanie czy trzeba cos NADAC przez LPT
    mov A,rs_rx_cnt
  jz lpt_tx_skip
  lcall rs232_rx_dequeue	; pobieramy bajt do ACC
  lcall lpt_send		; nadajemy bajt przez LPT
  lpt_tx_skip:

    ; sprawdzamy czy trzeba cos ODEBRAC przez LPT
  lcall lpt_recv		; probujemy cos odebrac
  jnb flg_lpt_has,lpt_recv_nothing		; niczego nie odebralismy
  lcall rs232_tx_enqueue	; dodajemy nasz bajt do kolejki nadawczej
  lpt_recv_nothing:

    ; sprawdzamy czy trzeba cos NADAC przez RS232
  jb flg_tx_now,rs_tx_skip_snd	; jak juz nadajemy, to niczego nie robimy
    ; mamy co nadawac?
    mov A,rs_tx_cnt
  jz rs_tx_skip_snd		; kolejka nadawcza jest pusta?
  lcall rs232_tx_dequeue	; pobieramy dane do nadania
    mov  SBUF,A			; rozpoczynamy nadawanie
    setb flg_tx_now		; zaznaczamy, ze juz nadajemy
  rs_tx_skip_snd:

    ; sprawdzamy czy trzeba cos ODEBRAC przez RS232
    ;   NOTA: to juz sie odbywa wewnatrz przerwania!


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




;****************;
;*              *;
;* LPT port I/O *;
;*              *;
;****************;

;
; nadawanie bajtu z ACC przez LPT
;
lpt_send:

    mov  lpt_data,A		; przenosimy dane do nadania
    clr  lpt_uC_data_rdy	; sygnalizujemy wole nadawania
  jb lpt_PC_data_acc,$		; czekamy az komputer potwierdzi odbior
    setb lpt_uC_data_rdy	; potwierdzamy odbior
  jnb lpt_PC_data_acc,$		; dla formalnosci czekamy na koniec transmisji

ret
; koniec nadawania bajtu przez LPT



;
; odbieranie bajtu przez LPT (zapis do ACC i ustawianie)
;
lpt_recv:

    mov lpt_data,#0xff		; przygotowujemy port do odbierania danych
    clr flg_lpt_has		; poki co nie mamy niczego do odebrania
  jb lpt_PC_data_rdy,no_write_req	; nie mamy sygnalu na zapis?
    ; otrzymalismy sygnal zadania odebrania danych
    mov A,lpt_data		; pobieramy dane
    clr lpt_uC_data_acc		; potwierdzamy odbior
  jnb lpt_PC_data_rdy,$		; czekamy na koniec transmisji
    setb lpt_uC_data_acc	; koniec zabawy

    setb flg_lpt_has		; odebralismy bajt!
  no_write_req:

ret
; koniec odbierania bajtu przez rs232



;*************************;
;*                       *;
;* RX (lpt<-rs232) QUEUE *;
;*                       *;
;*************************;


;
; dodanie bajtu z ACC do kolejki odbiorczej
;
rs232_rx_enqueue:
    clr  ES		; obecnie nie zezwalamy na przerwania rs232
    push B		; kopia zapasowa
    mov  B,R0		; ...
    push B		; ...

    mov B,A		; kopia :) ACC bedzie potrzebny
    ; liczymy offset nowego elementu:
    mov A,rs_rx_beg	; zaczynamy od miejsca gdzie jest poczatek
    add A,rs_rx_cnt	; dodajemy ilosc elementow juz w buforze
  cjne A,#0x10,rx_buf_next	; A<0x10 => C=1
  rx_buf_next:
  jc rx_addr_ok		; A<0x10 ?
    ;clr C
    subb A,#0x10	; bufor sie zawinal!
  rx_addr_ok:
    add A,#rs_rx_buf	; dodajemy do offsetu adres poczatq
    mov R0,A		; dzieki R0 mozna adresowac

    mov @R0,B		; wstawiamy nasza wartosc
    inc rs_rx_cnt	; zaznaczamy, ze mamy o 1 element wiecej

    pop B		; przywracamy oryginal
    mov R0,B		; ...
    pop B		; ...
    setb ES		; przywracamy mozliwosc przerwan od rs232
ret
; powrot z kolejkowania odebranego elementu




;
; zwraca element z kolejki odbiorczej w ACC
;
rs232_rx_dequeue:
    clr  ES		; obecnie nie zezwalamy na przerwania rs232
    push B
    mov  B,R0		; kopia zapasowa
    push B		; ...

    ; wyznaczamy pozycje w pamieci:
    mov A,rs_rx_beg	; offset poczatku
    add A,#rs_rx_buf	; i dodajemy adres tablicy
    mov R0,A		; wstawiamy do rejestru adresujacego

    mov  A,@R0		; pobieramy spod zadanego adresu 1 bajt
    ;push A		; ACC bedzie jeszcze potrzebny

    inc rs_rx_beg	; zaznaczamy, ze usunelismy 1 element
    dec rs_rx_cnt	; ...
    mov R0,rs_rx_beg	; pomocniczo pobieramy wartosc
  cjne R0,#0x10,rx_deq_beg_ok	; poczatek miesci sie w zakresie?
    mov rs_rx_beg,#0x00	; ustawiamy sie na poczatek buforu
  rx_deq_beg_ok:

    ;pop A		; pobieramy nasza odpowiedz do ACC

    pop B		; przywracamy oryginal
    mov R0,B		; ...
    pop B		; ...
    setb ES		; przywracamy mozliwosc przerwan od rs232
ret
; powrot z pobierania elementu z kolejki




;*************************;
;*                       *;
;* TX (lpt->rs232) QUEUE *;
;*                       *;
;*************************;


;
; dodanie bajtu z ACC do kolejki nadawczej
;
rs232_tx_enqueue:
    clr  ES		; obecnie nie zezwalamy na przerwania rs232
    push B		; kopia zapasowa
    mov  B,R0		; ...
    push B		; ...

    mov B,A		; kopia :) ACC bedzie potrzebny
    ; liczymy offset nowego elementu:
    mov A,rs_tx_beg	; zaczynamy od miejsca gdzie jest poczatek
    add A,rs_tx_cnt	; dodajemy ilosc elementow juz w buforze
  cjne A,#0x10,tx_buf_next	; A<0x10 => C=1
  tx_buf_next:
  jc tx_addr_ok		; A<0x10 ?
    ;clr C
    subb A,#0x10	; bufor sie zawinal!
  tx_addr_ok:
    add A,#rs_tx_buf	; dodajemy do offsetu adres poczatq
    mov R0,A		; dzieki R0 mozna adresowac

    mov @R0,B		; wstawiamy nasza wartosc
    inc rs_tx_cnt	; zaznaczamy, ze mamy o 1 element wiecej

    pop B		; przywracamy oryginal
    mov R0,B		; ...
    pop B		; ...
    setb ES		; przywracamy mozliwosc przerwan od rs232
ret
; powrot z kolejkowania elementu do nadania




;
; zwraca element z kolejki nadawczej w ACC
;
rs232_tx_dequeue:
    clr  ES		; obecnie nie zezwalamy na przerwania rs232
    push B
    mov  B,R0		; kopia zapasowa
    push B		; ...

    ; wyznaczamy pozycje w pamieci:
    mov A,rs_tx_beg	; offset poczatku
    add A,#rs_tx_buf	; i dodajemy adres tablicy
    mov R0,A		; wstawiamy do rejestru adresujacego

    mov  A,@R0		; pobieramy spod zadanego adresu 1 bajt
    ;push A		; ACC bedzie jeszcze potrzebny

    inc rs_tx_beg	; zaznaczamy, ze usunelismy 1 element
    dec rs_tx_cnt	; ...
    mov R0,rs_tx_beg	; pomocniczo pobieramy wartosc
  cjne R0,#0x10,tx_deq_beg_ok	; poczatek miesci sie w zakresie?
    mov rs_tx_beg,#0x00	; ustawiamy sie na poczatek buforu
  tx_deq_beg_ok:

    ;pop A		; pobieramy nasza odpowiedz do ACC

    pop B		; przywracamy oryginal
    mov R0,B		; ...
    pop B		; ...
    setb ES		; przywracamy mozliwosc przerwan od rs232
ret
; powrot z pobierania elementu z kolejki nadawczej



