run: qemu


boot.bin: boot.asm
	@nasm -f bin $< -o $@

qemu: boot.bin
	@qemu-system-i386 -fda $<

clean:
	rm -rf *.bin
