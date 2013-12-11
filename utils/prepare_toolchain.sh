#!/bin/bash
# This file is part of fotix. Copyright (c) 2013 Fotis Koutoulakis

# This file prepares and builds a toolchain to use get up to speed
# with building the system fast. It should work on Fedora, and may
# mod it in the future to work with Debian like systems.


GCC_VERSION=4.8.2
BINUTILS_VERSION=2.24
DIR="temp/"
PREFIX="$HOME/opt/cross"
TARGET=i586-elf
PATH="$PREFIX/bin:$PATH"

mkdir $DIR
pushd $DIR

echo "WGET: Preparing to pull gcc sources"
wget ftp://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz
tar -xf gcc-$GCC_VERSION.tar.gz
mv gcc-$GCC_VERSION/ gcc/

echo "WGET: Preparing to pull gmp (needed to build gcc)"
wget http://ftp.gnu.org/gnu/gmp/gmp-5.1.3.tar.xz
tar -xf gmp-5.1.3.tar.xz
mv gmp-5.1.3/ gmp/
mv gmp/ gcc/

echo "WGET: Preparing to pull mpfr (needed to build gcc)"
wget http://www.mpfr.org/mpfr-3.1.2/mpfr-3.1.2.tar.xz
tar -xf mpfr-3.1.2.tar.xz
mv mpfr-3.1.2/ mpfr/
mv mpfr/ gcc/

echo "WGET: Preparing to pull mpc (needed to build gcc)"
wget http://www.multiprecision.org/mpc/download/mpc-1.0.1.tar.gz
tar -xf mpc-1.0.1.tar.gz
mv mpc-1.0.1/ mpc/
mv mpc/ gcc/

echo "WGET: Preparing to pull binutils sources"
wget http://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz
tar -xf binutils-$BINUTILS_VERSION.tar.gz
mv binutils-$BINUTILS_VERSION/ binutils/

echo # Newline
echo "Preparing to build binutils"
echo
mkdir binutils-build
pushd binutils-build
../binutils/configure --target=$TARGET --prefix="$PREFIX" --disable-nls
make -j4
#make install #uncomment to use
popd

echo
echo "Preparing to build GCC"
echo
mkdir gcc-build
pushd gcc-build
../gcc/configure --target=$TARGET --prefix="$PREFIX" --disable-nls \
 --enable-languages=c,c++ --without-headers
make all-gcc -j4
make all-target-libgcc -j4
#make install-gcc
#make install-target-libgcc
popd

popd
rm -rf $DIR
