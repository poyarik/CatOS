bits 32

section .text
    align 4
    MULTIBOOT_PAGE_ALIGN  equ 1 << 0
    MULTIBOOT_MEMORY_INFO equ 1 << 1
    MULTIBOOT_VIDEO_MODE  equ 1 << 2  ; Бит 2 (или 6): просим GRUB настроить видеорежим!

    MULTIBOOT_HEADER_MAGIC equ 0x1BADB002
    ; Флаги: 1 | 2 | 4 = 0x07
    MULTIBOOT_HEADER_FLAGS equ (MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE)
    MULTIBOOT_CHECKSUM     equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

multiboot_header:
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM

    ; Неиспользуемые поля адресов (для ELF не нужны)
    dd 0, 0, 0, 0, 0

    ; --- Настройки VBE для GRUB ---
    dd 0    ; Mode type: 0 = linear framebuffer
    dd 1024 ; Width (Ширина)
    dd 768  ; Height (Высота)
    dd 32   ; Depth (Битрейт цвета: 32 bpp - RGBA)

global start
extern kmain

start:
    cli
	push ebx
    call kmain
    hlt
