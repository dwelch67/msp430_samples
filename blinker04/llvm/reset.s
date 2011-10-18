
.global reset
reset:

  mov #0x0280,r1
  call #notmain
  jmp hang

.global hang
hang:
  jmp hang

.globl intled
intled:
    call #ccifg_int_handler
    reti

.globl bis_r2
bis_r2:
    bis.w r15,r2
    ret
