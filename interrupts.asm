bits 32

global asm_keyboard_addr
global asm_idt_load

extern keyboard_handler_c

asm_idt_load:
	mov eax, [esp + 4]
	lidt [eax]
	ret

asm_keyboard_addr:
	pusha

	call keyboard_handler_c

	popa
	iretd


