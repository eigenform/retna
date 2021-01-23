# retna
Loader for [`iris`](https://github.com/eigenform/iris).

## Boot Process

- The `retna` ELF is loaded, which includes an `.stub` segment filled with an
  **ARM stub loader**, loaded at physical address `0x00010000`

- `retna` makes sure that IOS58 is loaded

- `retna` fetches some **target kernel binary**, i.e. from a storage device, 
  and loads it at physical address `0x10100000` 
  - A kernel binary consists of a kernel stub and the kernel ELF:

    ```c
    struct {
      u32 header_len;
      u32 elf_offset;
      u32 elf_len;
      u32 pad;

      u8  stub_loader_data[elf_offset - header_len];
      u8  elf_data[elf_size];
    };
    ```

- A **magic trick** causes the ES thread in ARM-world to jump to physical 
  address `0x00010000` in Thumb mode, and then into the ARM stub

- The ARM stub jumps to the **target kernel stub** at physical address
  `0x10100010` after the currently-running kernel:
  - Disables ARM FIQ and IRQ interrupts
  - Masks all ARM IRQs
  - Disables the instruction cache, data cache, and the MMU
  
- The kernel stub loads the **target kernel ELF** into SRAM and then jumps 
  to the newly-loaded reset exception vector at `0xffff0000`

## Memory Map
The physical memory layout 

```
0x0000_0000 - start of MEM1
0x0001_0000 - ARM stub
0x0002_0000 - retna ELF
...
0x017f_ffff - end of MEM1

0x1000_0000 - start of MEM2
0x1010_0000 - target kernel binary
...
0x1365_0000 - IOS58 usb
...	    - IOS58 modules
0x13ed_0000 - IOS58 wl
...
0x13ff_ffff - end of MEM2

0xfffe_0000 - IOS58 kernel / target kernel ELF segments
0xffff_0000 - IOS58 kernel / target kernel ELF segments
```

