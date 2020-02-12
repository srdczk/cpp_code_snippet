run: os.img
	@qemu-system-i386 -fda $<

os.img: boot.bin kernel.bin
	@cat $^ > $@

boot.bin: boot.asm
	@nasm -f bin $< -o $@

kernel.bin: kernel_entry.o kernel.o
	@i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel_entry.o: kernel_entry.asm
	@nasm $< -f elf -o $@

kernel.o: kernel.c
	@i686-elf-gcc -g -ffreestanding -c $< -o $@


clean:
	rm -rf *.o *.bin *.img
