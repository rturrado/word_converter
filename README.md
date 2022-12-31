[![main CI](https://github.com/rturrado/word_converter/actions/workflows/main_ci.yml/badge.svg)](https://github.com/rturrado/word_converter/actions/workflows/main_ci.yml)
[![codecov](https://codecov.io/github/rturrado/word_converter/branch/unixlike-builds/graph/badge.svg?token=GAF6OX0ZVE)](https://codecov.io/github/rturrado/word_converter)

# Word Converter

Converts all numbers English written into digits of the provided text.

## Windows

### Requirements

- **Microsoft Visual C++ (MSVC) compiler toolset**.
- **CMake**: required minimum version is 3.22.
- **git**.

Most of the required software can be installed from the [Visual Studio 2022 Installer](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false).

- **Microsoft Visual C++ (MSVC) compiler toolset**: *Workloads* tab, *Desktop development with C++* item.
- **CMake**: *Individual components* tab, *Compilers, build tools, and runtimes* section, *C++ CMake tools for Windows* item.
- **git**: *Individual components* tab, *Code tools* section, *Git for Windows* item.

### Clone

From a `cmd`:
```bash
C:\projects> git clone https://github.com/rturrado/word_converter.git
```

### Config

There are several options to run CMake from Visual Studio.
- CMake should start automatically when choosing a *Configuration* and a *Build Preset* (e.g. `msvc Debug (tests)` and `Build windows-msvc-debug (tests)`) in the tool bar.
- CMake can be started manually from the *Configure Cache* option in the *Project* menu.
- Finally, CMake can also be started manually from a *Developer Command Prompt* (*Tools* menu, *Command Line* option):
```bash
C:\projects\word_converter> cmake --preset windows-msvc-debug-tests
```

### Build

From Visual Studio, once CMake finishes, type CTRL+B or build from *Build > Build All* menu.

Or, from the command line:
```bash
C:\projects\word_converter> cmake --build --preset windows-msvc-debug-tests
```

#### Build presets

The following build presets are available (the configuration presets have the same name):
- Debug:
    - **windows-msvc-debug-tests**: *tests* enabled.
    - **windows-msvc-debug-github**: *tests* and *asan* enabled. This is the Debug preset used in GitHub Actions.
- Release:
    - **windows-msvc-release-tests**: *tests* enabled.

#### Output binaries

All successful builds will generate:
- `word_converter.exe`: the main binary, a console application that interacts with the user to execute the different problems from the book.
- `word_converter-<MAJOR_VERSION>_<MINOR_VERSION>.lib`: a static library, used, for example, by the test and benchmark binaries.

Builds with the option `-DWORD_CONVERTER_BUILD_TESTS=ON` (*debug* build presets) will also generate:
- `word_converter_test.exe`: a console application to test the code.

### Run

From the command line:
```bash
C:\projects\word_converter> .\out\build\windows-msvc-debug-tests\src\Debug\word_converter.exe res
```

### Tests

Build with:
```bash
C:\projects\word_converter> cmake --preset windows-msvc-debug-tests
C:\projects\word_converter> cmake --build --preset windows-msvc-debug-tests
```

You can run the test executable directly:
```bash
C:\projects\word_converter> .\out\build\windows-msvc-debug-tests\test\Debug\word_converter_test.exe
```

Or execute the tests via `ctest`:
```bash
C:\projects\word_converter\out\build\windows-msvc-debug-tests> ctest -C Debug --output-on-failure
```

Alternatively, if you want a less verbose ouptut:
```bash
C:\projects\word_converter> .\out\build\windows-msvc-debug-tests\test\Debug\word_converter_test.exe --gtest_brief=1
```

Or:
```bash
C:\projects\word_converter\out\build\windows-msvc-debug-tests> ctest -C Debug --output-on-failure --progress
```

## Linux

### Requirements

- **CMake**: required minimum version is 3.22.
- **ninja**.
- **gcc**: this project has been tested with version 12.
- **git**.
- **pkg-config**.
- **curl**.
- **tar**.
- **zip**.
- **unzip**.
- **wget**.

From a `terminal`, as administrator:
```bash
$> sudo apt-get -qq update
$> sudo apt-get -qq upgrade
$> sudo apt-get -qq -y install \
       cmake \
       curl \
       g++-12 \
       gcc-12 \
       git \
       ninja-build \
       pkg-config \
       tar \
       unzip \
       wget \
       zip
$> sudo update-alternatives \
       --install /usr/bin/gcc gcc /usr/bin/gcc-12 100 \
       --slave /usr/bin/g++ g++ /usr/bin/g++-12 \
       --slave /usr/bin/gcov gcov /usr/bin/gcov-12
```

### Clone

From a `terminal`:
```bash
~/projects> git clone https://github.com/rturrado/word_converter
```

### Config

From a `terminal`:
```bash
~/projects/word_converter> cmake --preset unixlike-gcc-debug-tests
```

### Build

From a `terminal`:
```bash
~/projects/word_converter> cmake --build --preset unixlike-gcc-debug-tests
```

#### Build presets

The following build presets are available (the configuration presets have the same name):
- Debug:
  - **unixlike-gcc-debug-tests**: *tests* enabled.
  - **unixlike-gcc-debug-github**: *tests* and *code coverage* enabled. This is the Debug preset used in GitHub Actions.
- Release:
  - **unixlike-gcc-release-tests**: *tests* enabled.

#### Output binaries

All successful builds will generate:
- `word_converter`: the main binary, a console application.
- `the_modern_c++_challenge-<MAJOR_VERSION>_<MINOR_VERSION>.lib`: a static library, used, for example, by the test and test binary.

Builds with the option `-DWORD_CONVERTER_BUILD_TESTS=ON` (*debug* build presets) will also generate:
- `word_converter_test`: a console application to test the code.

### Run

From a `terminal`:
```bash
~/projects/word_converter> ./out/build/unixlike-gcc-debug-tests/src/Debug/word_converter
```

### Tests

Build with:
```bash
~/projects/word_converter> cmake --preset unixlike-gcc-debug-tests
~/projects/word_converter> cmake --build --preset unixlike-gcc-debug-tests
```

You can run the test executable directly:
```bash
~/projects/word_converter> ./out/build/unixlike-gcc-debug-tests/src/Debug/word_converter_test
```

Or execute the tests via `ctest`:
```bash
~/projects/word_converter/out/build/unixlike-gcc-debug-tests> ctest -C Debug --output-on-failure
```

Alternatively, if you want a less verbose ouptut:
```bash
~/projects/word_converter> ./out/build/unixlike-gcc-debug-tests/test/Debug/word_converter_test res --gtest_brief=1
```

Or:
```bash
~/projects/word_converter/out/build/unixlike-gcc-debug-tests> ctest -C Debug --output-on-failure --progress
```

## Implementation details

