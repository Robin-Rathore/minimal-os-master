all : buildsubdirs os-image.bin

CC = i386-elf-gcc
LD = i386-elf-ld

CFLAGS = -g -ffreestanding -Wall -Wextra -m32

ARCHIVES = kernel/kernel.o cpu/cpu.o libc/libc.o fs/fs.o shell/shell.o
DRIVERS = drivers/drivers.o

SUBDIRS = boot kernel cpu drivers libc fs shell

export

os-image.bin: boot/boot_sect.bin kernel.bin
		@cat $^ > os-image.bin
		@echo Built Successfully

kernel.bin: boot/kernel_entry.o ${ARCHIVES} ${DRIVERS}
		@${LD} -o $@ -Ttext 0x1000 $^ --oformat binary
		@echo LD $<

.PHONY: buildsubdirs $(SUBDIRS)

buildsubdirs: $(SUBDIRS)

$(SUBDIRS):
		@$(MAKE) --no-print-directory -C $@

run: buildsubdirs os-image.bin
		@qemu-system-i386 -fda os-image.bin

usb: buildsubdirs os-image.bin
		@echo "Creating bootable USB image..."
		@dd if=/dev/zero of=minimalos.img bs=1M count=10
		@mkfs.fat -F12 minimalos.img
		@dd if=os-image.bin of=minimalos.img conv=notrunc
		@echo "USB image created: minimalos.img"
		@echo "To write to USB drive (replace /dev/sdX with your USB device):"
		@echo "sudo dd if=minimalos.img of=/dev/sdX bs=4M status=progress"

clean:
		@rm -rf *.bin *.dis *.o os-image.bin minimalos.img
		@rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o libc/*.o cpu/*.o fs/*.o shell/*.o
