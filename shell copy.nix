# shell.nix
{
  pkgs ?
    import <nixpkgs> {
      localSystem = "x86_64-linux";
      crossSystem = "i686-elf";
    },
}:
pkgs.callPackage (
  {mkShell}:
    mkShell {
      # By default this provides gcc, ar, ld, and some other bare minimum tools
    }
) {}

 srcs = {
    binutilsSrc = fetchurl {
      url = "https://ftp.gnu.org/gnu/binutils/binutils-2.40.tar.xz";
      sha256 = "1qfqr7gw9k5hhaj6sllmis109qxq5354l2nivrlz65vz5lklr2hg";
    };

    gccSrc = fetchurl {
      url = "https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.xz";
      sha256 = "1nj3qyswcgc650sl3h0480a171ixp33ca13zl90p61m689jffxg2";
    };
  };