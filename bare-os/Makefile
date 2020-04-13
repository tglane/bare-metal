cc=i386-elf-g++

CFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-stack-protector


SRCDIR=src
OBJDIR=src
SRCFILES := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCFILES))

.PHONY: all
all: kernel

boot.o: boot.asm
	nasm -f elf32 $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	echo $@
	$(cc) -c $< -o $@ $(CFLAGS)
.PHONY: kernel
kernel: $(OBJS) boot.o linker.ld
	$(cc) -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib boot.o $(OBJS) -lgcc

.PHONY: qemu
qemu: all
	qemu-system-x86_64 -fda kernel.bin

.PHONY: clean
clean:
	rm -f kernel.bin boot.o $(OBJDIR)/*.o

