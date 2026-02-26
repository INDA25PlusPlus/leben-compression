# leben-compression

## Usage

Setup and build using CMake: `cmake -B build`, `cmake --build build`

Encode: `EXE encode <input_file> <output_file>`

Decode: `EXE decode <input_file> <output_file>`

The input and output file may not be the same file.

## Results

### 100 MB sample txt

Source: https://examplefile.com/text/txt/100-mb-txt

Before: 100.0 MB

After: 52.6 MB (-47.4%)

### Hamlet (Shakespear)

Source: https://gist.github.com/provpup/2fc41686eab7400b796b

Before: 187.2 kB

After: 103.1 kB (-44.9%)
