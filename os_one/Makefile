cc = i386-elf-g++
ld = i386-elf-ld

CFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-stack-protector

BOOTDIR := boot
SRCDIR := kernel
OBJDIR := kernel
SRCFILES := $(wildcard $(SRCDIR)/*.cc)
OBJFILES := $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(SRCFILES))

.PHONY: image
image: boot.bin kernel.bin
	cat $^ > os_image

.PHONY: qemu
qemu: image
	qemu-system-x86_64 -fda os_image

boot.bin: $(BOOTDIR)/boot.asm
	nasm $(BOOTDIR)/boot.asm -f bin -o boot.bin

kernel.bin: $(BOOTDIR)/kernel_entry.o $(OBJFILES)
	$(ld) -o $@ -Ttext 0x1000 $^ --oformat binary

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	echo $@
	$(cc) -c $< -o $@ $(CFLAGS)

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

.PHONY:clean
clean:
	rm -rf $(OBJDIR)/*.o $(BOOTDIR)/*.o
	rm -rf ./*.bin

