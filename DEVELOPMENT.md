# Development

## Dependencies
Dependencies are managed using [Conan](https://conan.io/).

Some included libraries require to create local packages as no upstream packages exists yet. See [Open-GForce/conan-packaging](https://github.com/Open-GForce/conan-packaging) for more details.

Install libraries and generate build-macros:
```bash
mkdir -p build && cd build
conan install ..
```

## Compiling
For compiling the software please make sure that all required libraries are installed. Furthermore cmake and a C++ compiler with support for C++17 is needed.

Setup the build system using [CMake](https://cmake.org/):
```bash
cd build
cmake ..
```

Build system is ready now and you may start compiling:
```bash
make -j
```

## Unit tests
Unit tests may be executed by using the run_tests binary:
````bash
./bin/run_tests
````
