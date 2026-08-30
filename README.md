# AMXX Nova Pawn Compiler

[![License](https://img.shields.io/badge/license-GPL--3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Latest Release](https://img.shields.io/github/v/release/hlds-run/amxx-nova-pc)](https://github.com/hlds-run/amxx-nova-pc/releases/latest)

**English** | **[Русский](https://github.com/hlds-run/amxx-nova-pc/blob/main/README.ru.md)**

A modern, cross-platform Pawn language compiler for AMX Mod X. This is a command-line utility (`amxxpc`) that compiles Pawn source code (`.sma`) into executable plugins (`.amxx`).

## About The Project

`AMXX Nova Pawn Compiler` is a fork and a major overhaul of the standard compiler bundled with AMX Mod X 1.9.0 / 1.10.0. The primary goal of this project is to modernize the codebase, streamline the build process, and ensure full compatibility with existing AMXX plugins and standards.

The project has been refactored using C++23 and a modern CMake build system, enabling straightforward compilation on both Windows and Linux, and integrating contemporary tools to maintain high code quality.

## Key Features

*   **Full compatibility**: The project retains the original Pawn compiler core from AMX Mod X, which guarantees 100% compatibility of compiled plugins (.amxx) and eliminates the need to make any changes to the existing source code (.sma).
*   **Cross-Platform**: Natively build and run the compiler on both Windows and Linux.
*   **Modern Architecture and Wrapper**: The command-line wrapper (`amxxpc`) and the project's build system have been completely rewritten in modern C++23 using CMake. This ensures cross-platform compatibility and ease of compilation while preserving the original, time-tested compiler core.
*   **Code Quality**: Integrated static analyzers (Clang-Tidy, Cppcheck, PVS-Studio, IWYU) and formatters (Clang-Format) to maintain code cleanliness and stability.

## Usage

The compiler is a drop-in replacement for the standard `amxxpc` and offers flexible configuration methods.

### Basic Compilation

**Simple Compilation:**
```bash
amxxpc my_plugin.sma
```
This command will create `my_plugin.amxx` in the current working directory (use the `-o` option to change the output location).

**Specifying Output Path:**
```bash
amxxpc my_plugin.sma -oaddons/amxmodx/plugins/my_plugin.amxx
```

**Specifying Include Directories:**
```bash
amxxpc my_plugin.sma -i/path/to/scripting/include -i/path/to/custom/includes
```

### Full Command-Line Options List

The full list of options can also be displayed in the terminal by running the compiler with the `--help` flag.

```
Options:
        -A<num>       Alignment for the data segment and stack in bytes. Must be a multiple of the cell size (4 bytes).
        -a            Generate a human-readable assembler listing (.asm) instead of a binary (.amxx).
        -C[+/-]       Enable/disable (+/-) compact encoding to reduce the output file size.
        -c<name>      Codepage name or number for the source file (e.g., 1252 for Windows Latin-1).
        -D<path>      Set the compiler's working directory. Affects how relative paths are resolved. (Windows only)
        
        -d0           Disable debug info and checks. For maximum performance (release).
        -d1           Enable run-time checks only (bounds checking, etc.), without symbolic info.
        -d2           [Default] Enable full debug info (symbolic information) and run-time checks.
        -d3           Same as -d2, but with optimizations disabled. Useful for step-by-step debugging.
        
        -E            Treat warnings as errors.
        -e<name>      Redirect all errors and warnings to a specified file.
        -H<hwnd>      Window handle (HWND) to send a notification message on compilation finish. (Windows only)
        -h            Show the paths of the files included during compilation.
        -i<path>      Add a path to search for include files (#include <...>). Can be specified multiple times.
        -l            Create a listing file (.lst) showing the code after preprocessor handling.
        -o<name>      Set the base name/path for the output file.
        -p<name>      Specify a "prefix" file that will be implicitly included at the top of every script.
        -r[name]      Generate a cross-reference report (.xml) on symbol usage.
        -S<num>       Stack/heap size in cells.
        -s<num>       Skip the specified number of lines at the beginning of the source file.
        -sui[+/-]     Show/hide (+/-) stack usage information after a successful compilation.
        -T<name>      Load compilation options from the specified configuration file.
        -t<num>       TAB indent size (in character positions).
        -v<num>       Verbosity level; 0=quiet, 1=normal, 2=verbose.
        -w<num>       Disable a specific warning by its number.
        -X<num>       Abstract machine size limit in bytes.
        -\            Use '\' as the escape character.
        -^            Use '^' as the escape character.
        -;[+/-]       Require/don't require (+/-) a semicolon to terminate each statement.
        -([+/-]       Require/don't require (+/-) parentheses for function invocation.
```

Constant and macro definitions (`sym=value`, `sym=`) are described in the section below.

### Configuration Files (.cfg)

To avoid long and repetitive commands, you can specify compiler options in `.cfg` files. Each line contains a single option; lines starting with `#` are treated as comments. This is particularly useful for managing different build profiles (e.g., `debug` and `release`).

**Loading Configuration:**

1.  **Explicitly (`-T`)**: Use the `-T` option to load one or more configuration files (the option can be specified multiple times).
    *   `amxxpc -T D:\configs\my_build.cfg ...` — loads the file from a full path (the file must exist).
    *   `amxxpc -T release ...` — will look for the file `<compiler_dir>/target/release.cfg` (the `.cfg` extension is appended to a bare name automatically).

2.  **Default Loading** (if `-T` is not used):
    *   The compiler searches for `<compiler_dir>/target/default.cfg`.
    *   If not found, it looks for `pawn.cfg` next to the compiler's executable.

> [!NOTE]
> Independently of the options above, the compiler core additionally loads `<compiler_dir>/target/default.cfg` at startup. If that file exists, its options are applied even when a configuration file is selected with `-T`.

**Option Precedence:** Options are applied in the following order: first from the command line, then from the `.cfg` file(s). For options that accept a single value (e.g., `-d`, `-o`), **the value from the `.cfg` file overrides the command-line value**. Options that can be repeated (e.g., `-i`) are combined from all sources. Constant and macro definitions (`sym=value`) specified on the command line always take priority over definitions from the file.

### Defining Constants and Macros

You can define constants and macros directly from the command line, which is ideal for build scripts.

*   **Integer Constant**: `BUILD_ID=512`
*   **String Macro**: `VERSION_NAME="1.5.0 Release"` (equivalent to `#define VERSION_NAME "1.5.0 Release"`)
*   **Flag for Conditional Compilation**: `DEBUG_MODE=` (defines `DEBUG_MODE` as the constant `1`)

**Complex Command Example:**
```bash
amxxpc DEBUG_MODE= BUILD_ID=512 VERSION_NAME="1.5.0 Beta" my_plugin.sma
```

## Building from Source

### Prerequisites

**Linux (Debian-based, e.g., Ubuntu, Debian):**
*   Multi-arch support for building 32-bit applications on a 64-bit host is required.
*   Required packages: `build-essential`, `gcc-multilib`, `g++-multilib`, `cmake`, `ninja-build`.
    ```bash
    sudo apt-get update
    sudo apt-get install -y build-essential gcc-multilib g++-multilib cmake ninja-build git
    ```
*   GCC builds use the Gold linker by default (`ld.gold`); install the `binutils-gold` package if the linking stage fails.

**Windows:**
*   Visual Studio 2022 (with the "Desktop development with C++" workload).
*   [CMake](https://cmake.org/download/) (version 3.21+).
*   [Ninja Build](https://github.com/ninja-build/ninja/releases) (recommended, add to `PATH`).

**Alternative:** A ready-to-use VS Code development container (Ubuntu 24.04 with GCC, Clang and Cppcheck preinstalled) is provided in `.devcontainer/`.

### Build Instructions

We provide convenient build scripts in the `tools` directory. Both scripts support additional flags: `-c` (compiler: `gcc`/`clang` on Linux, `msvc`/`clang` on Windows), `-j` (number of parallel jobs), `-k` (keep the build directory between runs), `-s` (static linking of the runtime), `-g` (generator, Windows only) — and pass any extra CMake arguments after `--`.

#### Linux

1.  Clone the repository:
    ```bash
    git clone https://github.com/hlds-run/amxx-nova-pc.git
    cd amxx-nova-pc
    ```
2.  Make the build script executable:
    ```bash
    chmod +x tools/linux/build.sh
    ```
3.  Run the build:
    ```bash
    ./tools/linux/build.sh
    ```
    *The build type is set with the `-b` flag: `Release` (default), `Debug`, or `RelWithDebInfo`.*

#### Windows (PowerShell)

1.  Clone the repository:
    ```bash
    git clone https://github.com/hlds-run/amxx-nova-pc.git
    cd amxx-nova-pc
    ```
2.  Run the build:
    ```powershell
    .\tools\windows\build.ps1
    ```
    *The build type is set with the `-b` flag: `Release` (default), `Debug`, or `RelWithDebInfo`.*

### Build Artifacts

The compiled `amxxpc` binary (with the 32-bit compiler core statically linked in) will be located in the `bin/` directory, inside a subfolder named after the toolchain and build type (e.g., `bin/GNU-Release` for GCC on Linux or `bin/MSVC-Release` for Visual Studio on Windows).

## Contributing

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1.  Fork the Project.
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`).
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`).
4.  Push to the Branch (`git push origin feature/AmazingFeature`).
5.  Open a Pull Request.

Please adhere to the code style defined in the `.clang-format` file. A formatting pre-commit hook (using the pinned `clang-format-20` binaries bundled in `tools/`) is installed automatically when you configure the project with CMake.

The project also has unit tests based on [GoogleTest](https://github.com/google/googletest): build with `-D BUILD_UNIT_TESTS=ON` (the framework is downloaded at configure time) and run the `amxxpc.test` target.

## License

Distributed under the GNU General Public License v3.0. See `LICENSE` for more information.

## Acknowledgments

*   **ITB CompuPhase** for creating the Pawn language.
*   The **AMX Mod X Team** for developing and maintaining the original compiler and platform.
