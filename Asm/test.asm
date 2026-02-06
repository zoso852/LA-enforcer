; Assemble: nasm -f bin test.asm -o test.com

org 100h

start:
    ; --- Mode 13h ---
    mov ax, 0013h
    int 10h

    push cs
    pop ds           ; DS = CS pour accéder aux données du COM

    mov bx, 50      ; X
    mov dx, 170       ; Y
    mov si, sprite   ; adresse sprite
call put_tire
    call put_car
	call put_tire
    ; Attendre touche
    mov ah, 0
    int 16h

    ; Retour au mode texte
    mov ax, 0003h
    int 10h

    ret

put_tire:
	push ax
	push cx
	push di
	push es

mov ax, 0A000h
mov es, ax

mov ax, 6520 ; tire starts at x=120
mov di, ax
lea si, [sprite]

mov cx,8

row_loop2:
push cx
mov cx,8
col_loop2:
lodsb
stosb
loop col_loop2
add di, 320-8
pop cx
loop row_loop2

pop es
pop di
pop cx
pop ax
ret
; ==============================================
; put_sprite_8x8
; IN: BX=X, DX=Y, SI->sprite
; ==============================================
put_car:
    push ax
    push cx
    push di
    push es

    mov ax, 0A000h
    mov es, ax       ; segment VRAM
lea si, [sprite]
    ; offset = y*320 + x
    mov ax, dx
    mov cx, 320
    mul cx           ; AX = y*320
    add ax, bx       ; AX = offset
    mov di, ax

    mov cx, 8        ; 8 lignes
row_loop:
    push cx

    mov cx, 8        ; 8 colonnes
col_loop:
    lodsb            ; AL = [DS:SI]
    stosb            ; [ES:DI] = AL
    loop col_loop

    add di, 320-8    ; ligne suivante

    pop cx
    loop row_loop

    pop es
    pop di
    pop cx
    pop ax
    ret


; ==============================================
; SPRITE 8×8 — un octet par pixel
; ==============================================
sprite:
    db 0,0,40,40,40,40,0,0
    db 0,0,40,40,40,40,0,0
    db 0,0,28,0,0,28,0,0
    db 0,0,28,28,28,28,0,0
    db 0,0,28,28,28,28,0,0
    db 0,0,28,28,28,28,0,0
    db 0,0,28,28,28,28,0,0
    db 0,0,28,28,28,28,0,0

