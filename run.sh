echo Building..
nasm "Bootloader/boot.asm" -f bin -o "Outputs/boot.bin" -i Bootloader
nasm "Kernel/empty_end.asm" -f bin -o "Outputs/empty_end.bin"

i686-elf-c++ -ffreestanding -m32 -g -c "Kernel/kernel.cpp" -o "Outputs/kernel.o" -I Kernel
nasm "Kernel/kernel_entry.asm" -f elf -o "Outputs/kernel_entry.o" -i Kernel

i686-elf-c++ -ffreestanding -m32 -g -c "Drivers/port_io.cpp" -o "Outputs/Drivers/port_io.o" -I Drivers
i686-elf-c++ -ffreestanding -m32 -g -c "Drivers/VGA_Text.cpp" -o "Outputs/Drivers/VGA_Text.o" -I Drivers
i686-elf-c++ -ffreestanding -m32 -g -c "Drivers/Keyboard.cpp" -o "Outputs/Drivers/Keyboard.o" -I Drivers

i686-elf-c++ -ffreestanding -m32 -g -c "Library/mem.cpp" -o "Outputs/Library/mem.o" -I Library
i686-elf-c++ -ffreestanding -m32 -g -c "Library/string.cpp" -o "Outputs/Library/string.o" -I Library

i686-elf-c++ -ffreestanding -m32 -g -c "CPU/idt.cpp" -o "Outputs/CPU/idt.o" -I CPU
i686-elf-c++ -ffreestanding -m32 -g -c "CPU/isr.cpp" -o "Outputs/CPU/isr.o" -I CPU
i686-elf-c++ -ffreestanding -m32 -g -c "CPU/irq.cpp" -o "Outputs/CPU/irq.o" -I CPU
i686-elf-c++ -ffreestanding -m32 -g -c "CPU/timer.cpp" -o "Outputs/CPU/timer.o" -I CPU

i686-elf-ld -o "Outputs/kernel.bin" -Ttext 0x1000 "Outputs/kernel_entry.o" "Outputs/kernel.o" "Outputs/Drivers/VGA_Text.o" "Outputs/Drivers/port_io.o" "Outputs/Library/mem.o" "Outputs/CPU/idt.o" "Outputs/CPU/isr.o" "Outputs/CPU/irq.o" "Outputs/CPU/timer.o" "Outputs/Drivers/Keyboard.o" "Outputs/Library/string.o" --oformat binary

cat "Outputs/boot.bin" "Outputs/kernel.bin" > "Outputs/short.bin"
cat "Outputs/short.bin" "Outputs/empty_end.bin" > os.bin
echo Executed Build commands, running..
qemu-system-x86_64 -drive format=raw,file=os.bin,index=0,if=floppy


