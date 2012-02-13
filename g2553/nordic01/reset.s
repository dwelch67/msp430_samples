
.global reset
reset:

  mov #0x03FF,r1
  call #notmain
  jmp hang

.global hang
hang:
  jmp hang


.globl dummy
dummy:
  ret
