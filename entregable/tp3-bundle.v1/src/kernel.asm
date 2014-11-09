; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
;xchg bx,bx

%include "imprimir.mac"

global start


;; Saltear seccion de datos
jmp start
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
 
        lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0

        mov eax, CR0
        or eax, 1
        mov CR0, eax

    ; Saltar a modo protegido

        jmp 0x40:mp
        ;01000000

    ; Establecer selectores de segmentos
BITS 32
mp: 
        xor eax, eax
        ;mov ax, 0x40
        ;mov cs, ax  ; {Index: 8, gdt/ldt: 0, rpl = 0} kernel code, se setea con jmp no es necesario

        ;todos los segmenos apuntan al mismo descriptor (necesario para convención C)
        mov ax, 0x50
        mov es, ax
        mov ds, ax  
        mov ss, ax  
        mov fs, ax 
        mov gs, ax 


    ; Establecer la base de la pila
    
        mov ebp, 0x27000
        mov esp, 0x27000

    ; Imprimir mensaje de bienvenida

        push iniciando_mp_msg
        call print
        add esp, 4

    ; Inicializar pantalla


        call game_inicializarMapa

        push iniciando_vide_msg
        call print
        add esp, 4

    ; Inicializar el manejador de memoria
   
        call mmu_inicializar

    ; Inicializar el directorio de paginas

        push iniciando_paginacion
        call print
        add esp, 4

        call page_init

    ; Cargar directorio de paginas

        mov eax, 0x27000         
        mov cr3, eax
    
    ; Habilitar paginacion

        mov eax, cr0                
        or  eax, 0x80000000     
        mov cr0, eax
    
        push mensaje_bienvenida
        call print
        add esp, 4

    ; Inicializar tss

        call tss_inicializar

        call cargarTSS_zombie

    ; Inicializar tss de la tarea Idle

        call inicializarTareaIdle

    ; Inicializar el scheduler

        ;el scheduler no necesita inicializar, le pregunta a game.c que hacer

    ; Inicializar la IDT
    
        push iniciando_idt_msg
        call print
        add esp, 4

        call idt_inicializar

    ; Cargar IDT

        push cargando_idt_msg
        call print
        add esp, 4

        lidt [IDT_DESC]

    ; Configurar controlador de interrupciones

        call resetear_pic
        call habilitar_pic

    ; Cargar tarea inicial

        mov ax,0x68
        LTR ax
       
    ; Habilitar interrupciones

        sti

    ; Saltar a la primera tarea: Idle

        jmp 0x70:0

    ; Ciclar infinitamente (por si algo sale mal...)

    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;




%include "screen.asm"
%include "paging.asm"
%include "a20.asm"
extern GDT_DESC
extern IDT_DESC
extern idt_inicializar
extern mmu_inicializar_zombie
extern habilitar_pic
extern resetear_pic
extern mmu_inicializar
extern tss_inicializar
extern game_inicializarMapa
extern game_actualizarFrame
extern cargarTSS_zombie
extern inicializarTareaIdle
;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...',0
iniciando_vide_msg db   'Iniciando Pantalla...',0
iniciando_idt_msg db   'Iniciando IDT...',0
cargando_idt_msg db   'Cargando IDT...',0
iniciando_paginacion db "Inicando Paginacion",0
mensaje_bienvenida db "El Senior de los novillos",0
ok_msg db 'ok!',0
;;