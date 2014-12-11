global start
start:
jmp hola

;zombie messi

contador db 0
instruccionLoca: mov cr1, eax ; va a expolotar
times 500 dd instruccionLoca


hola:
	mov ebx, 0x7000 
	xor ebp, ebp
	xor edx, edx
	xor ecx, ecx
_borrarRastros:
		mov dword [0x08000000 + esi], 0
		add esi, 0x4
		cmp esi, 70
		Jl _borrarRastros
	mov ebx, 0x7000 
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
		mov [0x08001000 + esi], edi
		add esi, 0x5
		cmp esi, ebx
		Jl _insertando
	mov eax, 0xAAA
	int 0x66
	jmp _loop
	ret

times 500 dd instruccionLoca
