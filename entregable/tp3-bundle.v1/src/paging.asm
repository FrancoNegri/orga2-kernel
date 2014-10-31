BITS 32
page_init:	
;inicializar la primera entrada del directorio con la direccion de la tabla
	pushad

    push 0x27000

	call pedirPagina_0

    add esp, 4

    mov dword [0x27000], 0x28003 ;supervisor, read/write, present

	xor ecx, ecx
	mov ebx, 0x28000
	xor edx, edx

	.completar_page_table:
        cmp ecx, 1024
        je .fin_completar_page_table
	  
        mov [ebx + ecx * 4], edx
        add dword [ebx + ecx * 4], 0x00000003 ;supervisor, read/write, present
        add edx, 0x1000
        inc ecx
        jmp .completar_page_table
	.fin_completar_page_table:


    popad
    ret

global pedirPagina_0

;void *pedirPagina_0(void *direccion)
pedirPagina_0:
    push ebp
    mov ebp, esp
    push ebx
    push ecx
    


    xor ecx, ecx
    mov ebx, [ebp+8]


    .completar_page_directory:
        cmp ecx, 1024
        je .fin_completar_page_directory
    
        mov dword [ebx + ecx * 4], 0x00000002
        inc ecx
        jmp .completar_page_directory

    .fin_completar_page_directory:

    pop ecx
    pop ebx
    pop ebp

    ret


global mapearPagina
mascara: dd 0xFFFFF000
;void mapearPagina(void *direccionReal, void* direccionVirtual, void* directorioDePaginas,void* pagina)
mapearPagina:
    
    ;xchg bx,bx
    push ebp
    mov ebp, esp
    pushad
    mov eax, [ebp+8];direccionReal
    mov edx, [ebp+12];direccionVirtual
    mov edi, [ebp+16];directorioDePaginas
    mov ebx, [ebp+20]; pagina


    shr eax, 12; saco el offset
    mov esi, eax
    shr esi, 10; acá tengo el offset de la dir en el directorio
    shl esi, 12

    shl eax, 22
    shr eax, 10; acá tengo el offset de la dir en la pagina

    ;tengo que sacarle la ultima parte donde van los flags
    and edx, mascara
    and ebx, mascara
    
    mov [ebx + eax], edx; aca mapeo de la pagina a la dirreccion que quiero
    add dword [ebx + eax], 0x00000003; le digo que esta preente

    mov [edi + esi], ebx; aca apunto a la pagina que corresponde
    add dword [ebx + eax], 0x00000003; le digo que esta presente


    ;xchg bx,bx

    popad
    pop ebp
    ret
    ;hacemos shift para sacar el offset
    ;hacemos una mascara para separar el offset del page directory y el page completar page table
    ;seteamos bien el page dir y el page table
    ;fin...


global unmapearPagina


;void unmapearPagina(void* direccionVirtual, void* directorioDePaginas,void* pagina)
unmapearPagina:
    push ebp
    mov ebp, esp
    pushad
    mov edx, [esp+8];direccionVirtual
    mov edi, [esp+12];directorioDePaginas
    mov ebx, [esp+16]; pagina


    shr eax, 12; saco el offset
    mov esi, eax
    shr esi, 10; acá tengo el offset de la dir en el directorio
    shl esi, 12

    shl eax, 22
    shr eax, 10; acá tengo el offset de la dir en la pagina

    ;tengo que sacarle la ultima parte donde van los flags

    and ebx, mascara
    and edx, mascara
    mov dword [ebx + eax], 0; desmapeo la pagina

    popad
    pop ebp
    ret

global editarGDT

editarGDT:
    ret