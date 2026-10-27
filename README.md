<h1 align="center">CHIP-8 Emulator</h1>

<p align="center">
  A CHIP-8 emulator written in C, All 35 opcodes implemented, using <a href="https://www.raylib.com/">raylib</a> for graphics and input.
  <br>
  still a work in progress currently
</p>

## Dependencies

- A C compiler (GCC via MSYS2 UCRT64 on Windows)
- [CMake](https://cmake.org/) 3.14 or newer
- [Ninja](https://ninja-build.org/) (build system)
- [raylib](https://www.raylib.com/) 5.0 or newer

### Windows (MSYS2)

1. Install [MSYS2](https://www.msys2.org/).
2. Open the **MSYS2 UCRT64** terminal (not the plain MSYS or MINGW64 one) and run:

   ```
   pacman -S mingw-w64-ucrt-x86_64-gcc \
             mingw-w64-ucrt-x86_64-cmake \
             mingw-w64-ucrt-x86_64-ninja \
             mingw-w64-ucrt-x86_64-raylib
   ```

### macOS

```
brew install cmake ninja raylib
```

### Linux

Install `gcc`, `cmake`, `ninja`, and `raylib` from your distro's package manager. If your distro has no raylib package, build and install it from source following the [raylib instructions](https://github.com/raysan5/raylib#build-and-installation).

## Build

From the project root (on Windows, use the UCRT64 terminal):

```
cmake -B build -G Ninja
cmake --build build
```

After the first configure, `cmake --build build` is enough, even after editing `CMakeLists.txt`.

If you change compilers or install raylib after a failed configure, delete the `build/` folder and configure again.

## Run

```
./build/app
```

On Windows the executable is `./build/app.exe`.

### Contribution and Motivations
I would be appreciative of any advice or tips for this project, however I don't plan on accepting pull request or issues. I built this as a learning project to get a look into how emulation technology works and gain expereince in low level systems.
