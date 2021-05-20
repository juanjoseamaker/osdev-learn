[bits 16]
[org 0x7c00]

; Where to load the kernel
KERNEL_OFFSET equ 0x1000

; BIOS sets boot drive in 'dl'
mov [BOOT_DRIVE], dl

; Setup stack
mov bp, 0x9000
mov sp, bp

call load_kernel
call switch_to_32bit

jmp $

%include "./bootloader/disk.asm"
%include "./bootloader/gdt.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET   ; bx -> destination
    mov dh, 50              ; dh -> num sectors
    mov dl, [BOOT_DRIVE]    ; dl -> disk
    call disk_load
    ret

[bits 16]
switch_to_32bit:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load GDT descriptor
    mov eax, cr0
    or eax, 0x1             ; Enable protected mode
    mov cr0, eax
    jmp CODE_SEG:init_32bit ; far jump

[bits 32]
start_32bit:
    call KERNEL_OFFSET      ; Execute kernel
    jmp $                   ; Infinite loop

[bits 32]
init_32bit:
    mov ax, DATA_SEG        ; Update segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; Setup stack
    mov esp, ebp

    call start_32bit

BOOT_DRIVE db 0

; Padding
times 510 - ($-$$) db 0

; Magic number
dw 0xaa55
