global start
jmp start


instruccionLoca: mov cr1, eax ; va a expolotar

start:
	;me muevo unos cuantas casillas para adelante
	xor eax, eax
	mov eax, 0x83D
	int 0x66
	mov eax, 0x83D
	int 0x66
	mov eax, 0x83D
	int 0x66
	mov eax, 0x83D
	int 0x66
	mov eax, 0x83D
	int 0x66
_loop: 
	mov edi, [instruccionLoca]
	xor esi, esi
	_insertando:
		mov [0x08000000 + esi], edi
		cmp esi, 0x8000
		add esi, 0x30
		jl _loop
	mov eax, 0xAAA
	int 0x66
	jmp _loop
	ret

n equ 4096-$
times n db 0