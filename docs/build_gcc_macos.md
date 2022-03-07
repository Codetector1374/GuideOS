# Guide on creating a cross-compiler on macOS

## Gcc Configure
```shell
../gcc-11.2.0/configure --enable-targets=x86_64-pc-elf,i686-pc-elf --prefix=$PREFIX --disable-nls --enable-languages=c --without-headers
```

## Binutils
```shell
../binutils-2.38/configure --target=x86_64-pc-elf --enable-targets=x86_64-elf,i686-elf --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
```
