/* Linker script to configure memory regions. */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 256K
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 48K
  PERIPH (rw) : ORIGIN = 0x40000000, LENGTH = 0x10000000
}

INCLUDE stm32f103_common.ld
