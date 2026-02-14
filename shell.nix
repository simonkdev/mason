{pkgs ? import <nixpkgs> {}}: let
  binutils-i686 = pkgs.callPackage ./binutils-i686-elf.nix {};
  gcc-i686 = pkgs.callPackage ./gcc-i686-elf.nix {};
in
  pkgs.mkShell {
    # Ensure the i686‑elf GCC is in PATH
    buildInputs = [
      binutils-i686
      gcc-i686
    ];

    # Optionally set up environment variables
    #shellHook = ''
    # export PATH=${custom.installPrefix}/bin:$PATH
    # export CC=${custom.installPrefix}/bin/i686-elf-gcc
    # export AR=${custom.installPrefix}/bin/i686-elf-ar
    # export LD=${custom.installPrefix}/bin/i686-elf-ld
    #'';
  }
