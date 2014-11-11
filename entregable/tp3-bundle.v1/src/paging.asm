BITS 32

extern pedirPag

page_init:	
;inicializar la primera entrada del directorio con la direccion de la tabla
	pushad

    push 0x27000

	call pedirPag

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

;global editarGDT

; editarGDT:
;     ret


; global stop
; stop:
;     xchg bx, bx
;     ret  