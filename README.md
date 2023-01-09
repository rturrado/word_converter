[![main CI](https://github.com/rturrado/word_converter/actions/workflows/main_ci.yml/badge.svg)](https://github.com/rturrado/word_converter/actions/workflows/main_ci.yml)
[![codecov](https://codecov.io/gh/rturrado/word_converter/branch/main/graph/badge.svg?token=ZWGPNDS1TZ)](https://codecov.io/gh/rturrado/word_converter)

# Word Converter

Converts all numbers English written into digits of the provided text.

## Windows

### Requirements

- **Microsoft Visual C++ (MSVC) compiler toolset**.
- **CMake**: required minimum version is 3.22.
- **git**.

Most of the required software can be installed from the
[Visual Studio 2022 Installer](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false).

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
- CMake should start automatically when choosing a *Configuration* and a *Build Preset* (e.g. `msvc Debug (tests)` and
`Build windows-msvc-debug (tests)`) in the tool bar.
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

Builds with the option `-DWORD_CONVERTER_BUILD_TESTS=ON` (*debug* build presets) will also generate:
- `word_converter_test.exe`: a console application to test the code.

### Run

From the command line:
```bash
C:\projects\word_converter\out\build\windows-msvc-debug-tests\src\Debug> .\word_converter.exe -i <INPUT_FILE> [-o <OUTPUT_FILE>]
```

### Tests

Build with:
```bash
C:\projects\word_converter> cmake --preset windows-msvc-debug-tests
C:\projects\word_converter> cmake --build --preset windows-msvc-debug-tests
```

You can run the test executable directly (notice **tests have to be run from the folder where the binary lives**,
because they contain hardcoded paths to the resource folder):
```bash
C:\projects\word_converter\out\build\windows-msvc-debug-tests\test\Debug> .\word_converter_test.exe
```

Or execute the tests via `ctest`:
```bash
C:\projects\word_converter\out\build\windows-msvc-debug-tests> ctest -C Debug --output-on-failure
```

Alternatively, if you want a less verbose ouptut:
```bash
C:\projects\word_converter\out\build\windows-msvc-debug-tests\test\Debug> .\word_converter_test.exe --gtest_brief=1
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
  - **unixlike-gcc-debug-github**: *tests*, *asan*, and *code coverage* enabled. This is the Debug preset used in GitHub Actions.
- Release:
  - **unixlike-gcc-release-tests**: *tests* enabled.

#### Output binaries

All successful builds will generate:

- `word_converter`: the main binary, a console application.

Builds with the option `-DWORD_CONVERTER_BUILD_TESTS=ON` (*debug* build presets) will also generate:
- `word_converter_test`: a console application to test the code.

### Run

From a `terminal`:
```bash
~/projects/word_converter/out/build/unixlike-gcc-debug-tests/src/Debug> ./word_converter -i <INPUT_FILE> [-o <OUTPUT_FILE>]
```

### Tests

Build with:
```bash
~/projects/word_converter> cmake --preset unixlike-gcc-debug-tests
~/projects/word_converter> cmake --build --preset unixlike-gcc-debug-tests
```

You can run the test executable directly (notice **tests have to be run from the folder where the binary lives**,
because they contain hardcoded paths to the resource folder):
```bash
~/projects/word_converter/out/build/unixlike-gcc-debug-tests/test/Debug> ./word_converter_test
```

Or execute the tests via `ctest`:
```bash
~/projects/word_converter/out/build/unixlike-gcc-debug-tests> ctest -C Debug --output-on-failure
```

Alternatively, if you want a less verbose ouptut:
```bash
~/projects/word_converter/out/build/unixlike-gcc-debug-tests/test/Debug> ./word_converter_test --gtest_brief=1
```

Or:
```bash
~/projects/word_converter/out/build/unixlike-gcc-debug-tests> ctest -C Debug --output-on-failure --progress
```

## Implementation details

### Project structure

- An `include/word_converter` folder with all the includes.
- A `res` folder with the resource files.
- A `src` folder with the source files.
- A `test` folder with the test files.
- After a build, an `out/build` folder is also created.

The implementation of each class is done at the header files.<br/>
This leaves us with only one source file, `main.cpp`.<br/>
The `test` folder contains a `main.cpp` and one source file for each header file in `include/word_converter`.<br/>
The `res` folder contains files used by the tests. The test binary hardcodes a relative path to this resource directory, and,
for that reason, it has to be run from the folder where the binary lives (e.g. `out/build/unixlike-gcc-debug-tests/test/Debug`).

There is a `CMakeLists.txt` file at the root of the project, and at the root of the `src` and `test` folders.<br/>
CMake presets are also used via a `CMakePresets.json` file.

### Architecture

#### Main

The `main` function logic is quite simple:
- Parses the command line options.
- Creates an input reader.
- Creates a stream output writer (that will write to standard output), and, if requested by the user, a file output writer.
- Runs the conversion manager, passing it the reader and the writers.

Exceptions thrown whether during the parsing of the command line options, or while creating the reader or the writers, are captured,
and make the program terminate.<br/>
Both readers and writers are implemented as runtime polymorphic objects. A pure virtual base class, e.g. `input_reader` defines an interface,
and concrete classes, e.g. `file_reader`, implement that interface.
Using polymorphic readers is not mandatory for the task, but makes the implementation symmetric to that of the writers.
Apart from the fact that opens the possibility to read the input directly as a string from the command line, which is useful for testing.

#### Command line parser

We only accept 3 or 5 arguments, the executable name always being the first of them.<br/>
If the user enters 3 arguments, the second one has to be `-i`.<br/>
If the user enters 5 arguments, the second and fourth have to be whether `-i` and `-o`, or `-o` and `-i`.<br/>
If any of these conditions aren't met, a custom runtime error is thrown.<br/>
No further checks are made at this point (e.g. the file passed as a parameter exists).<br/>
Using a library such as `boost/program_options` may have simplified the parsing.

#### Input reader

Three classes are defined in this file: a pure virtual base class, `input reader`, and two concrete clases, `file_reader` and
`stream_reader`.<br/>
Each concrete class holds an input stream: `file_reader` reads from a file, and holds a file stream;
while `stream_reader` reads from any input stream. They also implement a virtual method to retrieve a reference to that stream<br/>
Upon construction, `file_reader` receives a file path, and checks that the path corresponds to a regular file. Otherwise,
it throws a custom runtime error.<br/>
The base class has a three-method public API: `read`, `eof`, and `fail`. `read` reads a sentence, i.e. until a period is found,
or until the end of file, if no period is found, and returns it. `eof` and `fail` let a client check the input stream's state.

#### Output writer

The implementation of the writers is quite similar to that of the readers.<br/>
There are also three classes: a pure virtual base class, `output_writer`, and two concrete classes, `file_writer` and `stream_writer`.
Again, each concrete class holds a stream, in this case an output stream.<br/>
The `file_writer` constructor just checks that the file stream is good. It doesn't check the file already exists.
The base class just exposes one `write` method, which grabs the output stream and writes a text to it.

##### Conversion manager

The `conversion_manager`:
- reads an input text from an `input_reader`,
- processes it using a `parser`, and
- writes it out to a list of `output_writer`s.

It basically contains a static `run` function that:
- Keeps reading sentences from an `input_reader` until the end of the file is reached.
- Texts that do not form a sentence (i.e. that do not end in a period) are not converted. All the texts are written out though.
- Every input sentence that needs to be processed is sent to the `parser`, and the result of this parsing appended to an output sentence.
- Once an input sentence has been processed, the output sentence is sent out to the different writers. 

##### Lexer

##### Parser
