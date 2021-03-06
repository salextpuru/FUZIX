;
;    ZX Spectrum 128 hardware support
;
;
;    This goes straight after udata for common. Because of that the first
;    256 bytes get swapped to and from disk with the uarea (512 byte disk
;    blocks). This isn't a problem but don't put any variables in here.
;
;    If you make this module any shorter, check what follows next
;


        .module zxpentevo

        ; exported symbols
        .globl init_early
        .globl init_hardware
        .globl _program_vectors
        .globl platform_interrupt_all

        .globl map_kernel
        .globl map_process
        .globl map_process_always
        .globl map_save
        .globl map_restore

        .globl _kernel_flag

        ; exported debugging tools
        .globl _trap_monitor
        .globl outchar

        ; imported symbols
        .globl _ramsize
        .globl _procmem

        .globl outcharhex
        .globl outhl, outde, outbc
        .globl outnewline
        .globl outstring
        .globl outstringhex

        .include "kernel.def"
        .include "../kernel.def"
        .include "pages.def"

; -----------------------------------------------------------------------------
; COMMON MEMORY BANK (0xF000 upwards)
; -----------------------------------------------------------------------------
            .area _COMMONMEM

_trap_monitor:
        ;ld a, #128
        ; out (29), a ; TODO: go where? BASIC48?
platform_interrupt_all:
        ret

_trap_reboot:
        rst 0

; -----------------------------------------------------------------------------
; KERNEL MEMORY BANK (below 0xC000, only accessible when the kernel is mapped)
; -----------------------------------------------------------------------------
        .area _CODE

; Memory map in kernel mode
; 0x0000 - memory window 0 - page 0 
; 0x4000 - memory window 1 - page 5
; 0x8000 - memory window 0 - page 2
; 0xC000 - memory window 0 - page 1 (kernel_page, switch to process)

init_early:
	ld a,#kernel_page
        ld (current_map), a
        setmw3 #kernel_page
        ret

init_hardware:
        ; set system RAM size
	ld hl, #1088
        ld (_ramsize), hl

	; 64K for kernel
	ld hl, #1088 - 64
        ld (_procmem), hl

        ; screen initialization
        ; clear
        ld hl, #0x4000
        ld de, #0x4001
        ld bc, #0x1800
        xor a
        ld (hl), a
        ldir

        ; set color attributes
        ld a, #7            ; black paper, white ink
        ld bc, #0x300 - 1
        ld (hl), a
        ldir

        im 1 ; set CPU interrupt mode
        ret

;------------------------------------------------------------------------------
; COMMON MEMORY PROCEDURES FOLLOW

        .area _COMMONMEM

        ; our vectors are in ROM, so nothing to do here
_program_vectors:
	ret

        ; bank switching procedure. On entrance:
        ;  A - bank number to set
        ;  stack: AF, <ret addr>
switch_bank:
        push bc
        ld (current_map), a
	setmw3a
        pop bc
        pop af
        ret

map_kernel:
        push af
map_kernel_nosavea:          ; to avoid double reg A saving
	ld a,#kernel_page
        jr switch_bank

map_process:
        push af
        ld a, h
        or l
        jr z, map_kernel_nosavea
        ld a, (hl)
        jr switch_bank

map_process_always:
        push af
        ld a, (U_DATA__U_PAGE)
        jr switch_bank

map_save:
        push af
        ld a, (current_map)
        ld (map_store), a
        pop af
        ret

map_restore:
        push af
        ld a, (map_store)
        jr switch_bank

current_map:                ; place to store current page number. Is needed
        .db 0               ; because we have no ability to read 7ffd port
                            ; to detect what page is mapped currently 
map_store:
        .db 0

place_for_a:                ; When change mapping we can not use stack since it is located at the end of banked area.
        .db 0               ; Here we store A when needed
place_for_b:                ; And BC - here
        .db 0
place_for_c:
        .db 0

; outchar: TODO: add something here (char in A). Current port #15 is emulator stub
outchar:

        ret

_kernel_flag:
        .db 1
