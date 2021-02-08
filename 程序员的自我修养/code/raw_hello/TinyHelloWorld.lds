ENTRY(nomain)

SECTIONS
{
   . = 0x0848000 + SIZEOF_HEADERS;
   tinytext : { *(.text) *(.data) *(.rodata) }
   /DISCARD/ : { *(.comment) }
}
