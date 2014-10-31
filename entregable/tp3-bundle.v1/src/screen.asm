;;;;;;;;;;;;;;;;;;;Funciones Del Kernel;;;;;;;;;;;;;;;;;;
;void actualizarPantalla()
actualizarPantalla:

    push ebp
    mov ebp, esp
    pushad

    xor eax, eax ; fila
    xor esi, esi ; col
    mov ecx, 0xB8000; putero


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

    mov dword [lineaApuntada], 1

    popad
    pop ebp
    ret

lineaApuntada dd 1

;void print(string* str)

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
        dec ecx ; ecx = longitud del mensaje

    mov eax,[lineaApuntada]             ; eax = fila
    mov ebx, 80
    imul ebx            ; eax = fila * 80
    mov ebx, 1           ; ebx = columna
    add eax, ebx        ; eax = fila * 80 + columna

    ; Establecer el puntero al buffer de pantalla en (fila, columna)
    mov ebx, 0xb8000    ; ebx = puntero a la base del buffer de video
    shl eax, 1          ; Cada posicion de la memoria de video ocupa un word
    add ebx, eax        ; ebx = puntero a la posicion (fila, columna)

    ; Cargar mensaje, longitud y color
    mov edi, [ebp+8]            ; edi = puntero al mensaje    
    mov eax, 0x20             ; eax = color

    ; Acomodar color en la parte alta de ax
    shl ax, 8           ; ah = color

    ; Imprimir cadena
    .ciclo_cadena:
        mov     al, [edi]       ; al = caracter
        mov     [ebx], ax       ; Escribir en pantalla
        add     ebx, 2          ; Avanzar el puntero de la memoria de video
        inc     edi             ; Avanzar el puntero de la cadena
        loop    .ciclo_cadena

    inc dword [lineaApuntada]

    popad
    pop ebp
    ret