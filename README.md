# Microkernel – Bootloader + Kernel (VGA + Teclado + Jogo da Forca)

## O que foi feito

- **Bootloader em Assembly (16-bit)**: carrega `KERNEL_SECTORS` da própria imagem para o endereço físico `0x00010000`, configura uma **GDT mínima**, entra em **Protected Mode** (32-bit) e chama `kernel_main`.
- **Kernel em C (32-bit, freestanding)**:
  - Escrita em **VGA texto** (0xB8000).
  - **IDT** e **remapeamento do PIC** (IRQs em `0x20..0x2F`).
  - **Interrupção de teclado (IRQ1)** lendo scancodes (porta `0x60`) e traduzindo ASCII básico.
  - **Jogo da Forca** jogável via teclado, com menu simples.

## Como rodar

Requisitos:
- `qemu-system-i386`
- `nasm`
- `gcc` com suporte a `-m32` (multilib)
- `ld` e `objcopy`

```bash
make run
