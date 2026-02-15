{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = {
    self,
    nixpkgs,
  }: {
    gcc-i686-elf = nixpkgs.legacyPackages.x86_64-linux.pkgs.callPackage ./gcc-i686-elf.nix {};

    packages.x86_64-linux.default = self.packages.x86_64-linux.hello;
  };
}
