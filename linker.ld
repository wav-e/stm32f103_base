ENTRY(Reset_handler);

/* Highest address of the user mode stack */
_estack = ORIGIN(SRAM) + LENGTH(SRAM); /* end of "RAM" Ram type memory */

_Min_Heap_Size = 0x200; /* required amount of heap */
_Min_Stack_Size = 0x400; /* required amount of stack */

MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  SRAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
  .text :
  {
      *(.isr_vector)
      *(.text)
      *(.text.*)
      *(.init)
      *(.fini)
      *(.rodata)
      *(.rodata.*)
      _etext = .;
  }> FLASH 

  _la_data = LOADADDR(.data);
  .data :
  {
    _sdata = .;
    *(.data)
    *(.data.*)
    _edata = .;

  }> SRAM AT> FLASH

  /* should include all .data section of all input files*/
  .bss :
  {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss.*)
    _ebss = .;
    __bss_end__ = _ebss;
    . = ALIGN(4);
    _end = .;

  }> SRAM



}

