; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

int0_capturada: db "La int 0 fue capturada con exito!"
int0_capturada_len equ $ - int0_capturada

int13_capturada: db "General Protection"
int13_capturada_len equ $ - int13_capturada

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;

global _isr0
global _isr13

_isr0:
    imprimir_texto_mp int0_capturada,int0_capturada_len,0x20, 4,1
    jmp $

_isr13:
    imprimir_texto_mp int13_capturada,int13_capturada_len,0x20, 5,1
    jmp $

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
_isr32:
    pushad
    call proximo_reloj
    call fin_intr_pic1
    popad
    iret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
%define IZQ 0xAAA
%define DER 0x441
%define ADE 0x83D
%define ATR 0x732


%define Tab 0x0f
%define Q 0x10
%define W 0x11
%define E 0x12
%define R 0x13
%define T 0x14
%define Y 0x15
%define U 0x16
%define I 0x17
%define O 0x18
%define P 0x19
%define BrktL{ 0x1a
%define BrktR} 0x1b
%define nter 0x1c
%define LCtrl 0x1d
%define A 0x1e
%define S 0x1f
%define D 0x20
%define F 0x21
%define G 0x22
%define H 0x23
%define J 0x24
%define K 0x25
%define L 0x26
%define ptoYcoma 0x27
%define comas 0x28
;%define `~ 0x29
%define LShift 0x2a
;%define \| 0x2b
%define Z 0x2c
%define X 0x2d
%define C 0x2e
%define V 0x2f
%define B 0x30
%define N 0x31
%define M 0x32
%define coma< 0x33
%define punto> 0x34
%define Barra/Pregunta 0x35
%define RShift 0x36
;%define Keypad-* 0x37
%define LAlt 0x38
%define Space bar 0x39
%define CapsLock 0x3a
%define F1 0x3b
%define F2 0x3c
%define F3 0x3d
%define F4 0x3e
%define F5 0x3f
%define F6 0x40
%define F7 0x41
%define F8 0x42
%define F9 0x43
%define F10 0x44
%define NumLock 0x45 
%define ScrollLock 0x46 
%define Keypad/Home 0x47 
%define Keypad/Up 0x48 
%define Keypad/PgUp 0x49 
;%define Keypad 0x4a 
%define Keypad/Left 0x4b 
;%define Keypad 0x4c 
%define Keypad/Right 0x4d
;%define Keypad 0x4e
%define Keypad/End 0x4f
%define Keypad/Down 0x50
%define Keypad/PgDn 0x51
;%define Keypad 0x52
%define Keypad/Del 0x53

coordenadaX dd 40
coordenadaY dd 25


global _isr33
_isr33:
    pushad
    in al, 0x60
    cmp al, A
    jz .A
    cmp al, B
    jz .fin
    cmp al, C
    jz .fin
    cmp al, D
    jz .fin
    cmp al, E
    jz .fin
    cmp al, F
    jz .fin
    cmp al, G
    jz .fin
    cmp al, H
    jz .fin
    cmp al, I
    jz .fin
    cmp al, J
    jz .fin
    cmp al, K
    jz .fin
    cmp al, L
    jz .fin
    cmp al, M
    jz .fin
    cmp al, N
    jz .fin
    cmp al, O
    jz .fin
    cmp al, P
    jz .fin
    cmp al, Q
    jz .fin
    cmp al, R
    jz .fin
    cmp al, S
    jz .fin
    cmp al, T
    jz .fin
    cmp al, Y
    jz .fin
    cmp al, Z
    jz .fin
    cmp al, V
    jz .fin
    jmp .fin
.A:
    call moverDerecha
    jmp .fin
.fin:
    call fin_intr_pic1
    popad
    iret

moverDerecha:
    pushad
    mov eax, 79
    imul dword [coordenadaY]
    mov ecx, [coordenadaX]
    add ecx,2
    mov [coordenadaX], ecx
    sub ecx, 2
    add ecx, eax
    mov di, 0x3000
    mov [gs:ecx], di
    popad
    ret


;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr66
_isr66:
    pushad
    call fin_intr_pic1
    popad
    mov eax, 0x42
    iret





;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
        pushad
        inc DWORD [isrnumero]
        mov ebx, [isrnumero]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrnumero], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                imprimir_texto_mp ebx, 1, 0x0f, 49, 79
                popad
        ret
        
        
