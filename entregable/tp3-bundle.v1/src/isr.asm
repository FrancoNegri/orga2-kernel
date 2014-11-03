; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

int0_capturada: db "Divide Error Exception",0
int1_capturada: db "Debug Exception",0
int2_capturada: db "NMI Interrupt",0
int3_capturada: db "Breakpoint Exception",0
int4_capturada: db "Overflow Exception",0
int5_capturada: db "BOUND Range Exceeded Exception!",0
int6_capturada: db "Invalid Opcode Exception",0
int7_capturada: db "Device Not Available Exception!",0
int8_capturada: db "Double Fault Exception",0
int9_capturada: db "Coprocessor Segment Overrun",0
int10_capturada: db "invalid TSS",0
int11_capturada: db "Segment Not Present",0
int12_capturada: db "Stack Fault Exception!",0
int14_capturada: db "Page-Fault Exception",0
int13_capturada: db "General Protection",0
int17_capturada: db "Alignment Check Exception",0




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
global _isr5
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr17
global _isr1
global _isr2
global _isr3
global _isr4
global _isr6
global _isr7


;en caso de que el error sea en la pila, esto podria traer porblemas
extern print
_isr0:
    push int0_capturada
    call print
    add esp, 4
    jmp $

_isr1:
    push int1_capturada
    call print
    add esp, 4
    jmp $

_isr2:
    push int2_capturada
    call print
    add esp, 4
    jmp $

_isr3:
    push int3_capturada
    call print
    add esp, 4
    jmp $

_isr4:
    push int4_capturada
    call print
    add esp, 4
    jmp $

_isr5:;BOUND Range Exceeded Exception
    push int5_capturada
    call print
    add esp, 4
    jmp $

_isr6:
    push int6_capturada
    call print
    add esp, 4
    jmp $

_isr7:
    push int7_capturada
    call print
    add esp, 4
    jmp $

_isr8:;Double Fault Exception
    push int8_capturada
    call print
    add esp, 4
    jmp $

_isr9:;Coprocessor Segment Overrun
    push int9_capturada
    call print
    add esp, 4
    jmp $

_isr10:;invalid TSS
    push int10_capturada
    call print
    add esp, 4
    jmp $

_isr11:;Segment Not Present
    push int11_capturada
    call print
    add esp, 4
    jmp $

_isr12:;Stack Fault Exception
    push int12_capturada
    call print
    add esp, 4
    jmp $

_isr13:
    push int13_capturada
    call print
    add esp, 4
    jmp $
_isr14:
    push int14_capturada
    call print
    add esp, 4
    jmp $

_isr17:
    push int17_capturada
    call print
    add esp, 4
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
        
        
