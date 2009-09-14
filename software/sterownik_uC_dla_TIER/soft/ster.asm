;
;
; sterownik mikrokontrolera robota TIER
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
;   P3.0      rs232 - RX
;   P3.1      rs232 - TX
;   P3.2-P0.7 univ. 6-bit I/O
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

; ustawienia dla silniqw:
eng_cycles EQU	(240+1)		; ilosc przerwan na cykl
t0_set     EQU	255		; nastaw licznika T0

; nastaw licznika T1 dla transmiscji rs232:
;rs232_bps EQU	57600			; predkosc transmisji (w bodach)
;rs232_bps EQU	 9600			; predkosc transmisji (w bodach)
rs232_bps EQU	 4800			; predkosc transmisji (w bodach)
t1_set    EQU	(256-57600/rs232_bps)	; nastaw licznika

; typy pakietow jakie mozemy otrzymac:
pck_init  EQU	00001111b	; inicjalizacja
pck_spdst EQU	00011110b	; ustawienie predkosci
pck_spdrd EQU	00101101b	; odczyt ustawien predkosci
pck_pp    EQU	00111100b	; ping-pong
pck_stop  EQU	11110000b	; soft-reset sterownika

; rozne
;comm_max  EQU	5


;*******************************;
;*                             *;
;* uzywane obszary pamieci RAM *;
;*                             *;
;*******************************;

; flagi bitowe:
flg_tx_now   EQU	0x20	; 1 oznacza ze uklad jest w trakcie nadawania
flg_comm_now EQU	0x21	; 1=odbyla sie jakas komunikacja

; statusowe:
p0_stat     EQU		0x30	; domyslny stan linii P0
p0_stat_now EQU		0x36	; bierzacy stan linii P0

; silniki:
; liczniki dla silniqw:
eng0_cnt      EQU	0x31	; licznik silnika 0
eng1_cnt      EQU	0x32	; licznik silnika 1
; wartosci poczatkowe liczniqw silniqw:
eng0_val      EQU	0x33	; wartosc dla licznika silnika 0
eng1_val      EQU	0x34	; wartosc dla licznika silnika 1
; licznik cykli przerwania silniqw:
eng_cycle_now EQU	0x35	; ...

; bufory I/O rs232 - po 16B kazdy:
rs_rx_buf  EQU		0x40	; 0x40<->0x4f wlacznie
rs_tx_buf  EQU		0x50	; 0x50<->0x5f wlacznie
; ilosc danych w RX/TX:
rs_rx_cnt  EQU		0x3f	; dlugosc bufora odbioru
rs_tx_cnt  EQU		0x3d	; dlugosc bufora nadawania

; timeout przy obieraniu danych:
rs_rx_tout		EQU	0x60	; adres licznika
rs_rx_tout_init EQU	240		; ilosc cykli przed kasowaniem

; rozne:
max_pck_len    EQU  5					; dlugosc pakietu

; skrocone adresy danych RX
rxpck_len      EQU	max_pck_len			; dlugosc dowolneog pakietu
rxpck_cmd      EQU	(rs_rx_buf+0)		; rozkaz
rxpck_d1       EQU	(rs_rx_buf+1)		; 1st bajt danych
rxpck_d2       EQU	(rs_rx_buf+2)		; 2nd bajt danych
rxpck_d3       EQU	(rs_rx_buf+3)		; 3rd bajt danych
rxpck_sum      EQU	(rs_rx_buf+4)		; ostatnia wyliczona suma kontrolna
last_pck_sum   EQU  rxpck_sum			; suma kontrolna otrzymanego pakietu

; skrocone adresy danych TX
txpck_len      EQU	max_pck_len			; dlugosc dowolnego pakietu
txpck_cmd      EQU	(rs_tx_buf+0)		; rozkaz
txpck_d1       EQU	(rs_tx_buf+1)		; 1st bajt danych
txpck_d2       EQU	(rs_tx_buf+2)		; 2nd bajt danych
txpck_d3       EQU	(rs_tx_buf+3)		; 3rd bajt danych
txpck_sum      EQU	(rs_tx_buf+4)		; ostatnia wyliczona suma kontrolna



;*********************;
;*                   *;
;* POCZATEK PROGRAMU *;
;*                   *;
;*********************;
ljmp start		; pomijamy poczatkowe obslugi przerwan

;
; OBSLUGA PRZERWANIA TIMERa T0
;
org 000bh
ljmp int_t0


;
; OBSLUGA PRZERWANIA rs232
;
org 0023h
ljmp int_rs232



;
; FUNKCJA OBSLUGI PRZERWANIA TIMERa T0
; (sterowanie silnikami)
;
int_t0:

    ; zapisujemy stan rejestrow
    push PSW
    push ACC

  int_t0_exec_start:
    ;
    ; sprawdzamy czy silnik 0 juz zakonczyl?
    ;
    mov  A,eng0_val		; pobieramy wartosc do osiagniecia
  cjne A,eng0_cnt,eng0_skip	; jeszcze nie osiagnieto wartosci?
    ; juz osiagnieto zalozona wartosc - wylaczamy silnik
    anl p0_stat_now,#(0xff-(eng0_f|eng0_b)); zerujemy bity sterujace silnika
  jmp eng0_end			; konczymy ten przebieg
  eng0_skip:
    inc  eng0_cnt		; odliczamy kolejny cykl
  eng0_end:

    ;
    ; sprawdzamy czy silnik 1 juz zakonczyl?
    ;
    mov  A,eng1_val		; pobieramy wartosc do osiagniecia
  cjne A,eng1_cnt,eng1_skip	; jeszcze nie osiagnieto wartosci?
    ; juz osiagnieto zalozona wartosc - wylaczamy silnik
    anl p0_stat_now,#(0xff-(eng1_f|eng1_b)); zerujemy bity sterujace silnika
  jmp eng1_end			; konczymy ten przebieg
  eng1_skip:
    inc  eng1_cnt		; odliczamy kolejny cykl
  eng1_end:

    ;
    ; sprawdzamy czy nie skonczylismy przypadkiem juz limitu cykli?
    ;
    mov  A,eng_cycle_now	; pobieramy wartosc do osiagniecia
  cjne A,#eng_cycles,cycles_skip; jeszcze nie osiagnieto wartosci?
	;
	; oprozniamy kolejke, jezeli minal timeout
	;
	clr EA				; sekcja krytyczna :)
  djnz rs_rx_tout,no_timeout
    mov rs_rx_tout,#1	; timeout reached
	mov rs_rx_cnt,#0	; czyszczenie bufora
  no_timeout:
    setb EA				; koniec sekcji krytycznej

    ; juz osiagnieto zalozona wartosc - wylaczamy silnik
    mov P0,p0_stat_now		; najpierw zapisujemy zadana wartosc
    mov p0_stat_now,p0_stat	; ustawiamy wartosc na domyslna
    ; zerujemy liczniki przebiegow silniqw i naszego cyklu
    mov eng0_cnt,     #0x00	; silnik 0
    mov eng1_cnt,     #0x00	; silnik 1
    mov eng_cycle_now,#0x00	; cykl

    ; mrugamy dioda od RX/TX, jezeli cos sie dzieje
  jnb flg_comm_now,comm_flg_skip	; niczego nie nadano/odebrano?
    mov A,p0_stat		; pobieramy stan bierzacy
    cpl A			; negujemy
    anl A,#led_rxtx		; i wyluskujemy interesujacy nas bit
    anl p0_stat,#(0xff-led_rxtx); zerujemy ten jeden bit
    orl p0_stat,A		; i ustawiamy jego nowa wartosc
    clr flg_comm_now		; zerujemy flgae do nastepnego przebiegu
  jmp comm_flag_was_set		; dopiero co zmienilismy stan!
  comm_flg_skip:
    anl p0_stat,#(0xff-led_rxtx); gasimy diode
  comm_flag_was_set:

  ;jmp t0_int_end		; konczymy ten przebieg
  jmp int_t0_exec_start		; moze jakies flagi sa do usuniecia?
  cycles_skip:
    inc eng_cycle_now		; zwiekszamy ilosc odbytych cykli
  cycles_end:

    ;
    ; na koniec zapisujemy nowe ustawienia jako wyjsciowe
    ; oraz wykonujemy pozostale "czynnosci administracyjne" ;)
    ;
    mov P0,p0_stat_now		; zapisujemy bierzacy stan

  t0_int_end:
    ; przywracamy rejestry
    pop ACC
    pop PSW

reti


;
; FUNKCJA OBSLUGI PRZERWANIA rs232
;
int_rs232:

    ; na poczatek zachowujemy uzywane rejestry
    push PSW
    push ACC
	mov A,R0
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
    mov R0,SBUF			; pobieramy bajt
    ; dodajmey dane do kolejki przychodzacych
	mov A,#rs_rx_buf	; adres poczatq
	add A,rs_rx_cnt		; przesuniecie
	xch A,R0			; zamiana A<->R0
    mov @R0,A			; dostawamy dane do kolejki
	inc rs_rx_cnt		; cnt++
	mov rs_rx_tout,#rs_rx_tout_init	; resetujemy licznik timeoutowy
  rs_ri_end:

    setb flg_comm_now		; zaznaczamy, ze odbyla si ejakas komunikacja

    ; przywracamy ustawienia
	pop ACC
	mov R0,A
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

    ; ustawianie buforow I/O dla rs232
    mov rs_rx_cnt,#0x00
    mov rs_tx_cnt,#0x00
	mov rs_rx_tout,#1		; niczego jeszcze nie dostalismy

    ; inicjalizacja portu rs232:
    setb P3.0			; musimy umozliwic odbior i nadawanie
    setb P3.1			; ...
    mov SCON,#01010000b		; rs232 w tryb 1 + zezwolenie na odbior
    orl PCON,#10000000b		; SMOD=1
    mov TMOD,#00100000b		; timer T1 w trybie 2
    mov TH1, #t1_set		; nastaw timer'a T1
    mov TL1, #t1_set		; oraz wartosc startowa
    ; flagi przerwania rs232
    clr  RI			; nie ma przerwania po odbiorze! :)
    clr  TI			; ani po nadawaniu!
    ; koniec ustawien - czas rozpoczac prace wlasciwa
    setb TR1			; uruchamiamy T1

    ; ustawienie sterowania silnikami:
    mov eng_cycle_now,#0x00	; jeszcze nie odbyto zadnego cyklu
    mov eng0_val,#0x00		; silniki nie pracuja
    mov eng1_val,#0x00		; ...
    mov eng0_cnt,#0x00		; ...
    mov eng1_cnt,#0x00		; ...

    ; ustawienie przerwania-timer'a T0
    orl TMOD,#00000010b		; timer T0 w trybie 2
    mov TH0, #t0_set		; nastaw licznika
    mov TL0, #0x00		; wartosc poczatkowa
    setb TR0			; uruchamiamy licznik T0

    ; rozne inne ustawienia
    mov p0_stat,#0x00		; wartosc do wystawienia na port P0
    orl p0_stat,#led_pwr	; zapalamy diode zasilania

    ; ustawiamy priorytety przerwan - przerwanie rs232 najwazniejsze!
    mov  IP,#0x00		; zerujemy priorytety
    setb PS			; ustawamy bit rs232

    ; wlaczamy uzywane przez nas przerwania:
    setb EA			; globalne zezwolenie przerwania
    setb ES			; przerwanie z transmisji szeregowej
    setb ET0			; przerwanie od timera T0

    ; zaznaczamy, ze uklad pracuje poprawnie
    mov p0_stat_now,p0_stat	; zakladamy ze obecnie mamy wartosc poczatkowa
    mov P0,p0_stat_now		; wystawiamy nasze ustawnienia


;*************************;
;*                       *;
;* GLOWNA PETLA PROGRAMU *;
;*                       *;
;*************************;
main:


    ;
    ; sprawdzamy czy mamy jakies dane do wyslania
    ;
    mov A,rs_tx_cnt
  jz rs_tx_no_data		; nie mamy danych do wyslania?
  jb flg_tx_now,rs_tx_busy	; juz cos nadajemy?

    ; rozpoczynamy wiec nadawanie bajtu!
    ; pobieramy bajt z kolejki:
	mov A,rs_tx_buf		; pobieramy bajt
    mov SBUF,A			; wysylamy!
    setb flg_tx_now		; i zaznaczamy ten fakt flaga
	; "cofamy" bufor:
	mov rs_tx_buf+0,rs_tx_buf+1	; ...
	mov rs_tx_buf+1,rs_tx_buf+2	; ...
	mov rs_tx_buf+2,rs_tx_buf+3	; ...
	mov rs_tx_buf+3,rs_tx_buf+4	; ...
	dec rs_tx_cnt		; cnt--

  rs_tx_busy:
  rs_tx_no_data:


    ;
    ; czekamy na dane z rs232 (2B)
    ;
    mov A,rs_rx_cnt
  jz rs_rx_no_data		; nie mamy danych do analizy?
  lcall rx_try_interpret_queue	; sprawdzamy czy otrzymalismy juz jakas
				; kompletna paczke danych i (w razie potrzeby)
				; podejmujemy odpowiednie czynnosci
  rs_rx_no_data:


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



;***********************************;
;*                                 *;
;* INTERPRETERY KOMEND STERUJACYCH *;
;*                                 *;
;***********************************;

; interpreter sprawdzajacy czy otrzymano poprawny pakiet
; i (jesli tak) stara sie go zinterpretowac, lub (jesli nie)
; usuwa pierwszy bajt z kolejki.
rx_try_interpret_queue:

	  mov A,rs_rx_cnt	; pobieramy ilosc bajtow danych
	cjne A,#max_pck_len, rx_try_interpret_queue_len_cmp	; sprawdzamy ilosc B.
	rx_try_interpret_queue_len_cmp:
    jc pck_end_far		; jezeli A<max_pck_len to jeszcze czekamy!

      ; pobieramy pierwszy z bajtow i probujemy go zinterpretowac
      mov A,rxpck_cmd	; z poczatq kolejki

      ; sprawdzamy czy to wogole ma szanse byc paczatkiem JAKIEGOKOLWIEK
      ; z posrod pakietow
      mov  B,A		; kopia robocza
      swap A		; zamieniamy starsza z mlodsza czescia bajtu
      cpl  A		; negujemy
    cjne A,B,pck_err_near1	; porownanie
    jmp pck_control_sum		; sprawdzamy jakiego typu jest pakiet
    pck_err_near1:
    jmp pck_err_far	; pakiet na odstrzal!

	  ; liczymy sume kontrolna dla pakietu
    pck_control_sum:
	  mov A,#max_pck_len-1	; inkrementujemy dla kazdego pakietu,
	  						; oprocz sumy wlasciwej oczywiscie
	  add A,rxpck_cmd
	  add A,rxpck_d1
	  add A,rxpck_d2
	  add A,rxpck_d3
	cjne A,rxpck_sum,pck_sum_err_near	; suma sie nie zgadza?
	jmp pck_check_type		; kontynuujemy przetwarzanie!
	pck_sum_err_near:
	jmp pck_err_far			; konczymy i usuwamy bledny bajt

      ; sprawdzamy rodzaj pakietu i wyliczamy sume kontrolna:
    pck_check_type:
	  mov A,rxpck_cmd			; ponownie pobieramy nasz rozkaz

    cjne A,#pck_init, not_pck_init
    lcall interpret_pck_init
    jmp pck_ok_far
    not_pck_init:

    cjne A,#pck_spdst,not_pck_spdst
    lcall interpret_pck_spdst
    jmp pck_ok_far
    not_pck_spdst:

;    cjne A,#pck_spdrd,not_pck_spdrd
;    lcall interpret_pck_spdrd
;    jmp pck_ok_far
;    not_pck_spdrd:

    cjne A,#pck_pp,   not_pck_pp
    lcall interpret_pck_pp
    jmp pck_ok_far
    not_pck_pp:

    cjne A,#pck_stop, not_pck_stop
    lcall interpret_pck_stop
    jmp pck_ok_far
    not_pck_stop:

    ; odpowiednie powroty z wywolan
  pck_err_far:
  pck_ok_far:
    mov rs_rx_cnt,#0x00			; queue is empty again
  pck_end_far:
ret
; powrot z testera interpretacji komend



;**************************************************;
;*                                                *;
;* FUNKCJE OBSLUGUJACE POSZCZEGOLNE TYPY PAKIETOW *;
;*                                                *;
;**************************************************;


interpret_pck_init:

    ; sekwencja powinna byc postaci:
	; 0x00 0x95 0xe2 0x12 0x8D

	mov A,rxpck_d1		; 0x95?
  cjne A,#0x95,pck_init_err	; zgadza sie?

	mov A,rxpck_d2		; 0xe2?
  cjne A,#0xe2,pck_init_err	; zgadza sie?

	mov A,rxpck_d3		; 0x12?
  cjne A,#0x12,pck_init_err	; zgadza sie?

    ; wysylamy odpowiedz (ACK)
  lcall rs232_send_ack		; ...

  pck_init_err:
ret
; /interpret_pck_init



interpret_pck_spdst:

    ; pakiet poprawny - pobieramy dane z kolejki przychodzacej
    ; oraz interpretujemy w locie:

    ; gasiennica 1:
    mov A,rxpck_d1		; nastaw gasiennicy 1
    mov eng0_val,A		; ustawiamy

    ; gasiennica 2:
    mov A,rxpck_d2		; nastaw gasiennicy 2
    mov eng1_val,A		; ustawiamy

    ; kierunki:
    mov A,rxpck_d3		; kierunki ruchu
    anl A,#0x0f			; tylko mlodsze 4 bity!
    anl p0_stat,#0xf0		; zerujemy mlodsze bity
    orl p0_stat,A		; i ustawiamy nowe

    ; wysylamy odpowiedz (ACK)
  lcall rs232_send_ack		; ...

ret
; /interpret_pck_spdst



interpret_pck_spdrd:
	; nie zaimplementowane...
ret
; /interpret_pck_spdrd



interpret_pck_pp:

    ; wysylamy pakiet identyczny, ale z liczba o 1 wieksza
	mov txpck_cmd,rxpck_cmd

	mov txpck_d1,rxpck_d1
	inc txpck_d1

	mov txpck_d2,rxpck_d2
	inc txpck_d2

	mov txpck_d3,rxpck_d3
	inc txpck_d3

	; wyliczamy sume kontrolna
	mov A,#4				; 4 inkrementacje :)
	add A,txpck_cmd			; bajty danych
	add A,txpck_d1			; ...
	add A,txpck_d2			; ...
	add A,txpck_d3			; ...
	mov txpck_sum,A			; wstawiamy sume kontrolna

	mov rs_tx_cnt,#0x05		; 5 bajtow do nadania

ret
; /interpret_pck_pp



interpret_pck_stop:

    ; caly stop powinien byc postaci:
    ; 0xF0  0x15 0x6E 0x76  0xED

    mov A,rxpck_d1			; 0x15?
  cjne A,#0x15,pck_stop_err	; nie zgadza sie?

    mov A,rxpck_d2			; 0x6E?
  cjne A,#0x6e,pck_stop_err	; nie zgadza sie?

    mov A,rxpck_d3			; 0x76?
  cjne A,#0x76,pck_stop_err	; nie zgadza sie?

    ;
    ; wykonanie rozkazu - miekki reset uC!! 8-O
    ;

    ; przerwania
    clr EA			; zabraniamy wszelakich przerwan
    clr ES			; ...
    clr ET0			; ...

    ; timery
    clr TR0			; wylaczamy timery
    clr TR1			; ...

    ; porty
    mov P0,#0xff		; porty w stan wysoki
    mov P1,#0xff		; ...
    mov P2,#0xff		; ...
    mov P3,#0xff		; ...

    ; inne
    mov SP,#0x07		; czyscimy stos do dna!

    ;zaczynamy program od poczatq
    ljmp start			; i przekazujemy sterowanie na poczatek!

    ; tutaj NIGDY nie docieramy, jezeli reset sie uda!
  pck_stop_err:

ret
; /interpret_pck_stop



;
; ustawia pakiet ACK do wyslania na podstawie wyliczonej
; sumy kontrolnej w zmiennej last_pck_ack
;
rs232_send_ack:

    ; tworzymy pakiet
    mov txpck_cmd,#0x4b			; naglowek ACK
	mov txpck_d1,last_pck_sum	; ostatnio wyliczona suma kontrolna

    ; wyliczamy sume kontrolna
	mov A,#0x02				; 2 bajty
	add A,txpck_cmd			; rozkaz
	add A,txpck_d1			; suma kontrolna rozkazu

	mov txpck_d2,A			; wstawaimy sume kontrolna
	mov rs_tx_cnt,#0x03		; 3 bajty do nadania

ret
; / rs232_send_ack


