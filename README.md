# C* (C Yıldız) - The Turkish C Programming Language

`cey` is the C* compiler. It transpiles `.cy` source files to C, then compiles them using `gcc` or `clang`.

## example.cy
```c
#ekle <stdgç.b>

tam ana(boşluk) {
    tam* dizi = (tam*) bellekal(5 * boyut(tam));
    için (tam i = 0; i < 5; i++) {
        dizi[i] = i * 2;
    }
    yazdırf("Merhaba C*\n");
    bırak(dizi);
    döndür 0;
}
```


## Usage
```bash
cey <input-files> [compiler-args] [cey-options]
```

## Example
```bash
cey ./example.cy -o ./build/example -Wall -Werror -g3 -- --cc=clang
```

## File Extension
Only files ending with `.cy` are compiled by `cey`. All other files and arguments are passed directly to the C compiler.

## Options
Use `--` to separate C compiler arguments from `cey` options, if any.
- `--pack`: Minify the generated C file (removes whitespace, breaks error messages).
- `--cc=gcc` or `--cc=clang`: Choose the backend compiler. Default is `gcc`.

## Output
Intermediate C files are written to: `./build/int/<filename>.cy`
