include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c */*.c)
MODULEELF=0000-sampleCodeModule.elf
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=_loader.c
LOADEROBJECT=$(LOADERSRC:.c=.o)

all: $(MODULE)

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

$(MODULE): $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld -Wl,--oformat=elf64-x86-64 $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) -o ../$(MODULEELF)

$(LOADEROBJECT):
	$(GCC) $(GCCFLAGS) -I./include -c $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin */*.o */*.bin */*.elf

.PHONY: all clean print
