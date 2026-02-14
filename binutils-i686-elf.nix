{
  lib,
  stdenv,
  fetchurl,
  makeWrapper,
  gmp,
  mpfr,
  mpc,
  isl,
  pkgs,
}:
stdenv.mkDerivation rec {
  pname = "i686-elf-cross-binutils";
  version = "1.0";

  crossTarget = "i686-elf";
  installPrefix = "$out/i686-elf";

  src = fetchurl {
    url = "https://ftp.gnu.org/gnu/binutils/binutils-2.40.tar.xz";
    sha256 = "1qfqr7gw9k5hhaj6sllmis109qxq5354l2nivrlz65vz5lklr2hg";
  };

  nativeBuildInputs = [
    makeWrapper
    gmp
    mpfr
    mpc
    isl
    pkgs.texinfo
    pkgs.perl
  ];

  buildInputs = [];

  unpackPhase = ''
    tar -xf $src -C . --strip-components=1
  '';

  configurePhase = ''
    mkdir -p build-binutils
    cd build-binutils

    ../configure --target=${crossTarget} --prefix=${installPrefix} --disable-nls --disable-werror
  '';

  buildPhase = ''
    make -j$(nproc)
  '';

  installPhase = ''
    make install
  '';

  meta = with lib; {
    description = "Cross GCC + Binutils for i686-elf (bare metal freestanding cross compiler)";
    homepage = "https://wiki.osdev.org/GCC_Cross-Compiler";
    license = licenses.gpl3Plus;
    platforms = ["x86_64-linux" "x86_64-darwin"];
  };
}
