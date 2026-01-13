MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
  RAM   (rwx): ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
  .text : {
    *(.isr_vector)
    *(.text*)
  } > FLASH

  .data : {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM AT > FLASH

  _sidata = LOADADDR(.data);

  .bss : {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM
}

