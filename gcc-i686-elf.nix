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
}: let
  binutils = pkgs.callPackage ./binutils-i686-elf.nix {};
in
  stdenv.mkDerivation rec {
    pname = "i686-elf-cross-gcc";
    version = "1.0";

    crossTarget = "i686-elf";
    installPrefix = "$out/i686-elf";

    hardeningDisable = ["all" "format"];

    src = fetchurl {
      url = "https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.xz";
      sha256 = "1nj3qyswcgc650sl3h0480a171ixp33ca13zl90p61m689jffxg2";
    };

    nativeBuildInputs = [
      makeWrapper
      gmp
      mpfr
      mpc
      isl
      pkgs.texinfo
      pkgs.perl
      pkgs.libmpc
      binutils
      pkgs.valgrind
    ];

    buildInputs = [];

    unpackPhase = ''
      tar -xf $src -C . --strip-components=1
    '';

    configurePhase = ''

      mkdir -p build-gcc
      cd build-gcc

      export CFLAGS=""
      export CXXFLAGS=""

      ../configure --target=${crossTarget} --prefix=${installPrefix} --disable-nls --with-gmp=${gmp} --with-mpfr=${mpfr} --with-mpc=${mpc} --with-isl=${isl} --with-as=${binutils}/bin/i686-elf-as --with-ld=${binutils}/bin/true --disable-nls --disable-multilib --enable-languages=c,c++ --without-headers --disable-shared --disable-threads --disable-libssp --disable-libgomp --disable-libquadmath --disable-libatomic



    '';

    buildPhase = ''
      export PATH="${installPrefix}/bin:$PATH"
      make all-gcc -j$(nproc)
      make all-target-libgcc -j$(nproc)
      make all-target-libstdc++-v3 -j$(nproc)
    '';

    installPhase = ''
      make install-gcc
      make install-target-libgcc
      make install-target-libstdc++-v3

    '';

    meta = with lib; {
      description = "Cross GCC + Binutils for i686-elf (bare metal freestanding cross compiler)";
      homepage = "https://wiki.osdev.org/GCC_Cross-Compiler";
      license = licenses.gpl3Plus;
      platforms = ["x86_64-linux" "x86_64-darwin"];
    };
  }
