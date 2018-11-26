TARGET    = main

MCU      ?= msp430g2553
#MCU      ?= msp430fr2111

#DEBUGGER ?= rf2500
DEBUGGER ?= tilib

TOOLCHAIN = /usr
CC        = $(TOOLCHAIN)/bin/msp430-gcc
AS        = $(TOOLCHAIN)/bin/msp430-as
LD        = $(TOOLCHAIN)/bin/msp430-ld
OC        = $(TOOLCHAIN)/bin/msp430-objcopy
OD        = $(TOOLCHAIN)/bin/msp430-objdump
OS        = $(TOOLCHAIN)/bin/msp430-size

CFLAGS   += -g
CFLAGS   += -Os
CFLAGS   += -Wall
CFLAGS   += -mmcu=$(MCU)

LFLAGS   += -g
LFLAGS   += -mmcu=$(MCU)

C_SRC     = ./main.c

INCLUDE   = -I./

OBJS      = $(C_SRC:.c=.o)

.PHONY: all
all: $(TARGET).bin

%.o: %.S
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin

.PHONY: flash
flash:
	mspdebug $(DEBUGGER) 'erase' 'load $(TARGET).elf' 'exit'
