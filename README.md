# Microkernel – Bootloader + Kernel (VGA + Teclado + Pedra Papel Tesoura)

## O que foi feito
[Para ver o video clique aqui](video.mp4)

- **Bootloader em Assembly (16-bit)**: carrega `KERNEL_SECTORS` da própria imagem para o endereço físico `0x00010000`, configura uma **GDT mínima**, entra em **Protected Mode** (32-bit) e chama `kernel_main`.
- **Kernel em C (32-bit, freestanding)**:
  - Escrita em **VGA texto** (0xB8000).
  - **IDT** e **remapeamento do PIC** (IRQs em `0x20..0x2F`).
  - **Interrupção de teclado (IRQ1)** lendo scancodes (porta `0x60`) e traduzindo ASCII básico.
  - **Jogo do Pedra Papel Tesoura** jogável via teclado, com menu simples.

## Como rodar

Requisitos:
- `qemu-system-i386`
- `nasm`
- `gcc` com suporte a `-m32` (multilib)
- `ld` e `objcopy`

```bash
nasm -f elf32 kernel.asm -o kernel_asm.o

gcc -m32 -ffreestanding -fno-pie -c kernel.c -o kernel.o
gcc -m32 -ffreestanding -fno-pie -c keyboard_map.c -o keyboard_map.o
gcc -m32 -ffreestanding -fno-pie -c rps_game.c -o rps_game.o

ld -m elf_i386 -T link.ld -o kernel.elf \               
   kernel_asm.o kernel.o keyboard_map.o rps_game.o

qemu-system-i386 -kernel kernel.elf 
