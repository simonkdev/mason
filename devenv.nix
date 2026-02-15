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
  packages = [pkgs.git i686-elf-gcc i686-elf-binutils];

  # https://devenv.sh/tasks/
  # tasks = {
  #   "myproj:setup".exec = "mytool build";
  #   "devenv:enterShell".after = [ "myproj:setup" ];
  # };
}
