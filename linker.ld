ENTRY(Reset_handler);

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
      *(.rodata)
      _etext = .;
  }> FLASH 

  .data :
  {
    _sdata = .;
    *(.data)
    _edata = .;

  }> SRAM AT> FLASH

  /* should include all .data section of all input files*/
  .bss :
  {
    _sbss = .;
    *(.bss)
    _ebss = .;
  }> SRAM
}
