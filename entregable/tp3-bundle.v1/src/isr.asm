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
    call proximo_reloj
    call fin_intr_pic1

    iret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

global _isr33
_isr33:
    call fin_intr_pic1
    iret



;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
global _isr66
_isr66:
    mov eax, 0x42
    call fin_intr_pic1
    iret




%define IZQ 0xAAA
%define DER 0x441
%define ADE 0x83D
%define ATR 0x732


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
        
        
