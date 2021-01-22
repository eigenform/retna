PREFIX 	:= arm-none-eabi
OBJCOPY := $(PREFIX)-objcopy
AS 	:= $(PREFIX)-as
GCC 	:= $(PREFIX)-gcc

ENTRY 	:= 0x00010000
SRC 	:= start.s main.c

ASFLAGS := -mbig-endian -mcpu=arm926ej-s
CFLAGS 	:= -mbig-endian -I. -mcpu=arm926ej-s -nostdlib
LDFLAGS := --section-start=.text=$(ENTRY)

all: ../loader/data/stub.bin

stub.elf: $(SRC)
	$(GCC) $(CFLAGS) -Wl,$(LDFLAGS) $(SRC) -o $@

../loader/data/stub.bin: stub.elf
	$(OBJCOPY) -S -O binary $< $@

clean:
	rm -rfv stub.elf
	rm -rfv ../loader/data/stub.bin
