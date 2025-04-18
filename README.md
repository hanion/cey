![cey](https://github.com/user-attachments/assets/b7a3be16-f2c3-4269-bcea-b29e4fac8dee)
# C* (C Yıldız) - The Turkish C Programming Language

`cey` is the C* compiler. It transpiles `.cy` source files to C, then compiles them using `gcc` or `clang`.

## example.cy
```c
#ekle <stdgç.b>
#ekle <stdküt.b>

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
- `--int`: Preserve the intermediate files.

## Output
Intermediate files are written to `/tmp/cey_tmp_*/` and automatically deleted after compilation.

## Reverse cey - yec
`yec` is a separate binary used to reverse-transpile C code to C*.
It only performs transpilation, not compilation.
```bash
yec <source_file> <destination_file>
```

## Bootstrap
To fully rebuild the compiler from its own source using the current toolchain, run:
```bash
make bootstrap
```
This will:
- Use `amalgamator` to generate an amalgamated C source file from `src/cey.c` and save it as `build/amalgamation.c`.  
- Reverse-transpile the amalgamated C source into C* (`examples/cey.cy`) using `yec`.  
- Compile `cey.cy` with the current `cey` compiler to produce a new `cey` binary, replacing the existing one in `build/cey`.

