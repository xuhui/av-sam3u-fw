CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
CP = arm-none-eabi-objcopy

BUILDDIR = out

SRC += startup.c
SRC += ll.c
SRC += irqs.c
SRC += main.c

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SRC))))

ELF = $(BUILDDIR)/program.elf
HEX = $(BUILDDIR)/program.hex
BIN = $(BUILDDIR)/program.bin

INCLUDES += -I.\

LKR_SCRIPT = sam3u1-sram.lds

CFLAGS  = -Os -g -Wall $(INCLUDES) -mcpu=cortex-m3 -mthumb \
   -DBOARD_MCK=48000000

LDFLAGS  = -nostartfiles -T$(LKR_SCRIPT) -Os -g -Wall \
   -mcpu=cortex-m3 -mthumb

$(BIN): $(ELF)
	$(CP) -O binary $< $@

$(HEX): $(ELF)
	$(CP) -O ihex $< $@

$(ELF): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILDDIR)
