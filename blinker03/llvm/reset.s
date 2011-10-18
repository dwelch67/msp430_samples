
.global reset
reset:

  mov #0x0240,r1
  call #notmain
  jmp hang

.global hang
hang:
  jmp hang

.globl intled
intled:
    xor.b #0x41,&0x0021
    reti

.globl bis_r2
bis_r2:
    bis.w r15,r2
    ret
