[org 0x7c00]





KERNEL_LOCATION equ 0x1000	; Was 0x7e00 but I changed it because of buffer overflow problems
				; Implementing SSP asap
				
mov [BOOT_DISK], dl		; Stores the boot disk number

mov bx, OK
call PrintString

xor ax, ax			; clear bits of ax
mov es, ax			; set es to 0
mov ds, ax			; set ds to 0
mov bp, 0x8000 		; stack base
mov sp, bp			; stack pointer to stack base
					; A:B = A*d16 + B
mov bx, KERNEL_LOCATION	; ES:BX is the location to read from, e.g. 0x0000:0x9000 = 0x00000 + 0x9000 = 0x9000
mov dh, 35			; read 20 sectors (blank sectors: empty_end)

call readDisk


AFTER_DISK_READ: 

mov bx, OK
call PrintString

mov ah, 0x0			; clear screen (set text mode)
mov al, 0x3
int 0x10

;call GraphicsMode		; uhm, later on

call switchToPM

	jmp $

GraphicsMode:
	mov ah, 0		; switch to graphics mode
	mov al, 0x13
	;int 10h
	ret
	
%include "PrintString.asm"
%include "ReadFromDisk.asm"
%include "PrintDecimal.asm"
%include "GDT.asm"

%include "EnterPM.asm"

OK:
	db 'Ok', 10, 13, 0
[bits 32]

BEGIN_PM:
	jmp KERNEL_LOCATION	; jumps to entry_kernel
	;mov bl, OK
	;call PrintStringPM
	;jmp $

%include "PrintStringPM.asm"
times 510-($-$$) db 0
dw 0xaa55
