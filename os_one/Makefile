cc := i386-elf-g++
ld := i386-elf-ld

CFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-stack-protector

IMAGENAME := os_image

BOOTDIR := boot
SRCDIR := src
OBJDIR := src
INCLUDEDIR := include
SRCFILES := $(wildcard $(SRCDIR)/*/*.cc) $(wildcard $(SRCDIR)/*/*/*.cc)
#OBJFILES := $(patsubst $(SRCDIR)/*/%.cc,$(OBJDIR)/*/%.o,$(SRCFILES))
OBJFILES := ${SRCFILES:.cc=.o}

$(info $$var is [${OBJFILES}])

.PHONY: image
image: boot.bin kernel.bin
	cat $^ > $(IMAGENAME)

.PHONY: qemu
qemu: image
	qemu-system-x86_64 -fda $(IMAGENAME)

boot.bin: $(BOOTDIR)/boot.asm
	nasm $(BOOTDIR)/boot.asm -f bin -o boot.bin

kernel.bin: $(BOOTDIR)/kernel_entry.o $(SRCDIR)/kernel/arch/interrupt.o $(OBJFILES)
	$(ld) -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.cc
	echo $@
	$(cc) -I$(INCLUDEDIR) -c $< -o $@ $(CFLAGS)

%.o: %.asm
	echo $@
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

.PHONY:clean
clean:
	rm -rf $(OBJDIR)/*.o
	rm -rf $(OBJDIR)/*/*.o 
	rm -rf $(BOOTDIR)/*.o
	rm -rf ./*.bin
	rm -rf $(IMAGENAME)

