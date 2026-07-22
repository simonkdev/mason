{
  pkgs,
  lib,
  config,
  inputs,
  ...
}:
let
  i686-elf-gcc = pkgs.callPackage ./gcc-i686-elf.nix { };
  i686-elf-binutils = pkgs.callPackage ./binutils-i686-elf.nix { };
in
{
  packages = [
    pkgs.git
    i686-elf-gcc
    i686-elf-binutils
    pkgs.grub2
    pkgs.qemu
    pkgs.libisoburn
    pkgs.cmake
    pkgs.gdb
  ];

  # https://devenv.sh/tasks/
  tasks = {
    "mason:clean" = {
      exec = ''
        make clean
      '';
    };
    "mason:build" = {
      exec = ''
        make build
      '';
    };
    "mason:assemble_iso" = {
      exec = ''
        make assemble_iso
      '';
    };
    "mason:test" = {
      exec = ''
        make test
      '';
    };
    "mason:test-nobuild" = {
      exec = ''
        make test-nobuild
      '';
    };
  };
}
