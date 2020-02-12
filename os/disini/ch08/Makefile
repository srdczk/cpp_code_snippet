run: os.img
	@qemu-system-i386 -vga std -fda $<

os.img: boot.bin kernel.bin
	@cat $^ > $@

boot.bin: boot/boot.asm
	@nasm -f bin $< -o $@

kernel.bin: kernel_entry.o kernel.o port.o screen.o
	@i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel_entry.o: boot/kernel_entry.asm
	@nasm $< -f elf -o $@

kernel.o: kernel/kernel.c
	@i686-elf-gcc -g -ffreestanding -c $< -o $@

port.o: driver/port.c
	@i686-elf-gcc -g -ffreestanding -c $< -o $@

screen.o: driver/screen.c
	@i686-elf-gcc -g -ffreestanding -c $< -o $@

clean:
	rm -rf *.o *.bin *.img
