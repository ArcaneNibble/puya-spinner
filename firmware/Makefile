.PHONY: all clean

ARM_CPU = cortex-m0plus

CC = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -ggdb3 -Os -mcpu=$(ARM_CPU) -ffunction-sections -fdata-sections
LDFLAGS = -ggdb3 -mcpu=$(ARM_CPU) -Wl,--gc-sections --specs=nosys.specs

OBJS = startup.o main.o

all: main.elf main.bin

main.elf: $(OBJS)

-include $(OBJS:.o=.d)

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.elf: linker.lds
	$(CC) -Xlinker -Map=$(@:.elf=.map) -Wl,--script=linker.lds -nostartfiles -o $@ $(filter-out linker.lds,$+)
	$(OBJDUMP) -xdsS $@ >$(@:.elf=.dump)

%.o %.d: %.S
	$(CC) $(CFLAGS) -c -MMD -MP $<

%.o %.d: %.c
	$(CC) $(CFLAGS) -c -MMD -MP $<

startup.o startup.d: startup.c
	$(CC) $(CFLAGS) -ffreestanding -c -MMD -MP $<

clean:
	rm -f *.o *.elf *.bin *.map *.dump *.uf2 *.d
