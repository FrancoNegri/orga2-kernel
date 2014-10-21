BITS 32
page_init:	
;inicializar la primera entrada del directorio con la direccion de la tabla
	pushad

	xchg bx, bx

	xor ecx, ecx
	mov ebx, 0x27000
	
    .completar_page_directory:
        cmp ecx, 1024
        je .fin_completar_page_directory
    
        mov dword [ebx + ecx * 4], 0x00000002
        inc ecx
        jmp .completar_page_directory

    .fin_completar_page_directory:	
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

