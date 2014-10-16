; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"
%include "a20.asm"
extern GDT_DESC
global start


;; Saltear seccion de datos

jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg
;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;




;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0
    

    ; Habilitar A20
    
    call habilitar_A20

    ; Cargar la GDT

    lgdt [GDT_DESC] ; puede que sea fruta

    ; Setear el bit PE del registro CR0

    mov eax, CR0
    or eax, 1
    mov CR0, eax
    
    ; Saltar a modo protegido

    jmp 0x8:mp

    ; Establecer selectores de segmentos
BITS 32
mp: 
    xor eax, eax

    mov ax, 1000000b
    mov cs, ax  ; {Index: 8, gdt/ldt: 0, rpl = 0} code
    
    mov ax, 1010000b
    mov ds, ax  ; {Index: 10, gdt/ldt: 0, rpl = 0} data
    mov fs, ax
    
    mov ax, 1011000b
    mov ss, ax  ; {Index: 11, gdt/ldt: 0, rpl = 0} stack
    
    mov ax, 1001000b
    mov es, ax  ; {Index: 9, gdt/ldt: 0, rpl = 3} data video    

    ; Establecer la base de la pila
    
    mov ebp, 0x27000
    mov esp, 0x27000

    ; Imprimir mensaje de bienvenida

    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 4, 5

    ; Inicializar pantalla
    
    ; Inicializar el manejador de memoria
 
    ; Inicializar el directorio de paginas
    
    ; Cargar directorio de paginas

    ; Habilitar paginacion
    
    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    
    ; Cargar IDT
 
    ; Configurar controlador de interrupciones

    ; Cargar tarea inicial

    ; Habilitar interrupciones

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;


