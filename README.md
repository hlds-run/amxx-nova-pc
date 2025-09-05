# AMXX Nova Pawn Compiler

[![License](https://img.shields.io/badge/license-GPL--3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Latest Release](https://img.shields.io/github/v/release/hlds-run/amxx-nova-pc)](https://github.com/hlds-run/amxx-nova-pc/releases/latest)

A modern, cross-platform Pawn language compiler for AMX Mod X. This is a command-line utility (`amxxpc`) that compiles Pawn source code (`.sma`) into executable plugins (`.amxx`).

## About The Project

`AMXX Nova Pawn Compiler` is a fork and a major overhaul of the standard compiler bundled with AMX Mod X 1.9.0 / 1.10.0. The primary goal of this project is to modernize the codebase, streamline the build process, and ensure full compatibility with existing AMXX plugins and standards.

The project has been refactored using C++23 and a modern CMake build system, enabling straightforward compilation on both Windows and Linux, and integrating contemporary tools to maintain high code quality.

## Key Features

*   **Full Compatibility**: Compiled plugins are 100% compatible with official AMX Mod X versions. Your existing code will work without any modifications.
*   **Cross-Platform**: Natively build and run the compiler on both Windows and Linux.
*   **Modern Codebase**: Transitioned from legacy C to modern C++23 for improved reliability, readability, and maintainability.
*   **Advanced Build System**: Utilizes CMake with presets for a unified and predictable build experience on any platform.
*   **Code Quality**: Integrated static analyzers (Clang-Tidy, Cppcheck) and formatters (Clang-Format) to maintain code cleanliness and stability.

## Usage

The compiler is a drop-in replacement for the standard `amxxpc`.

**Simple Compilation:**
```bash
amxxpc my_plugin.sma
```
This command will create `my_plugin.amxx` in the same directory.

**Specifying Output Path:**
```bash
amxxpc my_plugin.sma -oaddons/amxmodx/plugins/my_plugin.amxx
```

**Specifying Include Directories:**
```bash
amxxpc my_plugin.sma -i/path/to/scripting/include -i/path/to/custom/includes
```

Below is a complete list of supported command-line options. The full list of options can also be displayed in the terminal by running the compiler with the `--help` flag.

```
Options:
        -A<num>       Alignment for the data segment and stack in bytes. Must be a power of two.
        -a            Generate a human-readable assembler listing (.asm) instead of a binary (.amxx).
                      Useful for low-level debugging.
        -C[+/-]       Enable/disable (+/-) compact encoding to reduce the output file size.
        -c<name>      Codepage name or number for the source file (e.g., 1252 for Windows Latin-1).
        -D<path>      Set the compiler's working directory. Affects how relative paths are resolved.
        
        -d0           Disable debug info and checks. For maximum performance (release).
        -d1           [Default] Enable run-time checks only (bounds checking, etc.).
        -d2           Enable full debug info and checks. Recommended for development.
        -d3           Same as -d2, but with optimizations disabled. Useful for step-by-step debugging.
        
        -e<name>      Redirect all errors and warnings to a specified file. Useful for IDE integration
                      and automated build systems.
        -H<hwnd>      Window handle (HWND) to send a notification message on compilation finish (Windows only).
        -i<path>      Add a path to search for include files (#include <... A.I.>). Can be specified multiple times.
                      Paths with spaces must be quoted.
        -l            Create a listing file (.lst) showing the code after preprocessor handling
                      (#include, #define). Compilation is not performed.
        -o<name>      Set the base name/path for the output file. If not set, the name is derived
                      from the source file.
        -p<name>      Specify a "prefix" file that will be implicitly included at the top of every
                      compiled script. Useful for global constants or macros.
        -r[name]      Generate a cross-reference report (.xml) on symbol usage. If no filename is given,
                      the report is written to the console.
        -sui[+/-]     Show/hide (+/-) stack usage information after a successful compilation.
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

**Windows:**
*   Visual Studio 2022 (with the "Desktop development with C++" workload).
*   [CMake](https://cmake.org/download/) (version 3.21+).
*   [Ninja Build](https://github.com/ninja-build/ninja/releases) (recommended, add to `PATH`).

### Build Instructions

We provide convenient build scripts in the `tools` directory.

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
    *To build a Debug version, use the `-b Debug` flag.*

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
    *To build a Debug version, use the `-b Debug` flag.*

### Build Artifacts

The compiled binaries (`amxxpc` and `amxxpc32.so`/`amxxpc32.dll`) will be located in the `bin/` directory, inside a subfolder corresponding to the compiler and build type (e.g., `bin/gcc-Release`).

## Contributing

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1.  Fork the Project.
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`).
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`).
4.  Push to the Branch (`git push origin feature/AmazingFeature`).
5.  Open a Pull Request.

Please adhere to the code style defined in the `.clang-format` file.

## License

Distributed under the GNU General Public License v3.0. See `LICENSE` for more information.

## Acknowledgments

*   **ITB CompuPhase** for creating the Pawn language.
*   The **AMX Mod X Team** for developing and maintaining the original compiler and platform.
