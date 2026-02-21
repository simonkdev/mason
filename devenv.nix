{
  pkgs,
  lib,
  config,
  inputs,
  ...
}: let
  i686-elf-gcc = pkgs.callPackage ./gcc-i686-elf.nix {};
  i686-elf-binutils = pkgs.callPackage ./binutils-i686-elf.nix {};
in {
  packages = [
    pkgs.git
    i686-elf-gcc
    i686-elf-binutils
    pkgs.grub2
    pkgs.qemu
    pkgs.libisoburn
  ];

  # https://devenv.sh/tasks/
  tasks = {
    "mason:clean" = {
      exec = ''
        rm -rf isodir build
      '';
    };
    "mason:build" = {
      exec = ''
        rm -rf isodir build
        mkdir -p build
        i686-elf-as ./src/boot/boot.s -o ./build/boot.o
        i686-elf-as ./src/kernel/gdt_load.s -o ./build/gdt_load.o
        i686-elf-gcc -c ./src/kernel/kernel.c -o ./build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        i686-elf-gcc -c ./src/kernel/gdt.c -o ./build/gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        i686-elf-gcc -T ./src/linker.ld -o ./build/mason -ffreestanding -O2 -nostdlib ./build/boot.o ./build/kernel.o ./build/gdt.o ./build/gdt_load.o -lgcc
        mkdir -p isodir/boot/grub
      '';
    };
    "mason:assemble_iso" = {
      exec = ''
        rm -rf isodir build
        mkdir -p build
        i686-elf-as ./src/boot/boot.s -o ./build/boot.o
        i686-elf-as ./src/kernel/gdt_load.s -o ./build/gdt_load.o
        i686-elf-gcc -c ./src/kernel/kernel.c -o ./build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        i686-elf-gcc -c ./src/kernel/gdt.c -o ./build/gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        i686-elf-gcc -T ./src/linker.ld -o ./build/mason -ffreestanding -O2 -nostdlib ./build/boot.o ./build/kernel.o ./build/gdt.o ./build/gdt_load.o -lgcc
        mkdir -p isodir/boot/grub
        cp ./build/mason isodir/boot/mason
        cp ./src/grub.cfg isodir/boot/grub/grub.cfg
        grub-mkrescue -o mason.iso isodir
      '';
    };
    "mason:test" = {
      exec = ''
        rm -rf isodir build
        mkdir -p build
        i686-elf-as ./src/boot/boot.s -o ./build/boot.o
        i686-elf-as ./src/kernel/gdt_load.s -o ./build/gdt_load.o
        i686-elf-gcc -c ./src/kernel/kernel.c -o ./build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        i686-elf-gcc -c ./src/kernel/gdt.c -o ./build/gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        i686-elf-gcc -T ./src/linker.ld -o ./build/mason -ffreestanding -O2 -nostdlib ./build/boot.o ./build/kernel.o ./build/gdt.o ./build/gdt_load.o -lgcc
        mkdir -p isodir/boot/grub
        cp ./build/mason isodir/boot/mason
        cp ./src/grub.cfg isodir/boot/grub/grub.cfg
        grub-mkrescue -o mason.iso isodir
        qemu-system-i386 -cdrom mason.iso
      '';
    };
  };
}
#./build/gdt.o ./build/gdt_load.o

