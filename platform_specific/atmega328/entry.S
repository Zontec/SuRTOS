.global _start
.org 0x0000
_start:
    call os_entry
    call _exit

