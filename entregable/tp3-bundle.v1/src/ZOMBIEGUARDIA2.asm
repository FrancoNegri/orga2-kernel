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
	jmp _borrarRastros
	times 20 dd instruccionLoca

_borrarRastros:
	mov dword [0x08000000 + esi], 0
	add esi, 0x4
	cmp esi, 70
	Jl _borrarRastros
	jmp _loop

	times 20 dd instruccionLoca

_loop:
	mov edx, [0x08001000]
	inc ecx
	mov edi, [instruccionLoca]
	xor esi, esi
	jmp _insertando

	times 20 dd instruccionLoca

	_insertando:
		mov [0x08001000 + esi], edi
		add esi, 0x10
		cmp esi, ebx
		Jl _insertando
	
	;RdRand edx
	cmp edx, 0
	je saltoAdelante
	inc ebp
	cmp ebp, 3
	je reset
	mov edx, [0x08005000]
	cmp edx, 0
	je saltoAIzq
	jmp saltoADer

	times 20 dd instruccionLoca

saltoADer:
	mov eax, 0x441
	jmp salto

	times 20 dd instruccionLoca

saltoAIzq:
	mov eax, 0xAAA
	jmp salto
	times 30 dd instruccionLoca

reset:
	xor ebp, ebp
saltoAdelante:
	mov eax, 0x83D
	jmp salto
	times 20 dd instruccionLoca

salto:
	int 0x66
	jmp _loop
	ret

times 200 dd instruccionLoca
