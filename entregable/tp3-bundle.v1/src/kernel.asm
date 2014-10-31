; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

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
    ;xchg bx,bx     ;;;;;;;;;;;;;;;;;; DEBUG LINE;;;;;;;;;;;;;;;;;

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

    ;imprimir_texto_mr iniciando_GDT_msg, iniciando_GDT_len, 0x07,0,0

    ; Saltar a modo protegido

    jmp 0x40:mp

    ;01000000
    ; Establecer selectores de segmentos
BITS 32
mp: 
    xor eax, eax

    ;mov ax, 0x40
    ;mov cs, ax  ; {Index: 8, gdt/ldt: 0, rpl = 0} kernel code, se setea con jmp no es necesario

    mov ax, 0x48
    mov es, ax  ; {Index: 9, gdt/ldt: 0, rpl = 3} App code

    mov ax, 0x50
    mov ds, ax  ; {Index: 10, gdt/ldt: 0, rpl = 0} kernel data
    mov ss, ax  ; {Index: 10, gdt/ldt: 0, rpl = 0} kernel stack
    
    mov ax, 1011011b 
    mov fs, ax ;{Index: 11, gdt/ldt: 0, rpl = 3} App data
    
    mov ax, 1100000b 
    mov gs, ax ;{Index: 12, gdt/ldt: 0, rpl = 0} video data


    ;;;;;;;;;;;;;;;;;;;;;;;;;;;Selector Map;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;           Sleector                                    LVL     ;
    ;           cs      ->      Kernel code         ->      0       ;
    ;           es      ->      App code            ->      3       ;
    ;           ds      ->      Kernel Data         ->      0       ;
    ;           ss      ->      Kernel Stak         ->      0       ;
    ;           fs      ->      App data            ->      3       ;
    ;           gs      ->      Video Data          ->      0       ;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ; Establecer la base de la pila
    
    mov ebp, 0x27000
    mov esp, 0x27000

    ; Imprimir mensaje de bienvenida

    push iniciando_mp_msg
    call print
    add esp, 4


    ; Inicializar pantalla

    push iniciando_vide_msg
    call print
    add esp, 4

    call actualizarPantalla

    push ok_msg
    call print
    add esp, 4

    ; Inicializar el manejador de memoria
   
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
    
       

       ; call mmu_inicializar_zombie

    push mensaje_bienvenida
    call print
    add esp, 4

    ; Inicializar tss



    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

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


    ;prueba de int 0
    ;xor esi, esi
    ;idiv esi

    ;prueba de int 13
    ;mov ecx, 0xffffffff
    ;mov [gs:ecx], di

    ; Configurar controlador de interrupciones

    call resetear_pic
    call habilitar_pic

    int 66

    ; Cargar tarea inicial
    xchg bx,bx



    ; Habilitar interrupciones

    sti

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)

    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;


;;;;;;;;;;;;;;;;;;;Funciones Del Kernel;;;;;;;;;;;;;;;;;;
actualizarPantalla:

    push ebp
    mov ebp, esp
    pushad

    xor eax, eax ; fila
    xor esi, esi ; col
    xor ecx, ecx ; putero


    xor edi, edi
    xor ebx, ebx
    xor edx, edx

    mov di, 0x2000 ;caracter nulo(00), fondo verde (A0)
    mov dx, 0x3000 ;caracter nulo(00), fondo rojo (C0)
    mov bx, 0x4000 ;caracter nulo(00), fondo azul horrible (B0)
    ;xchg bx,bx

newLine:
    mov [gs:ecx], bx
    add ecx, 2
    inc eax
init:
    mov [gs:ecx], di

    add ecx, 2
    inc eax
    cmp eax, 79
    jnz init

    mov [gs:ecx], dx
    add ecx, 2

    xor eax, eax
    inc esi
    cmp esi, 50
    jnz newLine

    popad
    pop ebp
    ret

lineaApuntada dd 1
;print(string* str)
print:
    push ebp
    mov ebp, esp
    pushad
    mov edi,[ebp+8]
    ;cuento caracteres
        sub ecx, ecx
        sub al, al
        not ecx
        cld
        repne   scasb
        not ecx
        dec ecx

    imprimir_texto_mp edi, ecx, 0x20, dword [lineaApuntada],1
    inc dword [lineaApuntada]

    popad
    pop ebp
    ret




%include "paging.asm"
%include "a20.asm"
extern GDT_DESC
extern IDT_DESC
extern idt_inicializar
extern mmu_inicializar_zombie
extern habilitar_pic
extern resetear_pic

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...',0
iniciando_mp_len equ    $ - iniciando_mp_msg

iniciando_vide_msg db   'Iniciando Pantalla...',0
iniciando_vide_len equ  $ - iniciando_vide_msg

iniciando_idt_msg db   'Iniciando IDT...',0
iniciando_idt_len equ  $ - iniciando_idt_msg

cargando_idt_msg db   'Cargando IDT...',0
cargando_idt_len equ  $ - cargando_idt_msg

iniciando_paginacion db "Inicando Paginacion",0
iniciando_paginacion_len equ $ - iniciando_paginacion

mensaje_bienvenida db "El Senior de los novillos",0
mensaje_bienvenida_len equ $ - mensaje_bienvenida

ok_msg db 'ok!'
ok_len equ $ - ok_msg
;;