CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
CP = arm-none-eabi-objcopy

BUILDDIR = out

SRC += startup.c
SRC += ll.c
SRC += irqs.c
SRC += hw.c
SRC += main.c

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SRC))))

ELF = $(BUILDDIR)/program.elf
BIN = $(BUILDDIR)/program.bin

INCLUDES += -I.\

LKR_SCRIPT = sam3u1-flash.ld

CFLAGS  = -Os -g -Wall $(INCLUDES) -mcpu=cortex-m3 -mthumb \
   -DBOARD_MCK=48000000

LDFLAGS  = -nostartfiles -T$(LKR_SCRIPT) -Os -g -Wall \
   -mcpu=cortex-m3 -mthumb

LDLIBS =

$(BIN): $(ELF)
	$(CP) -O binary $< $@

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
