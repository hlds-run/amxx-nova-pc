#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace PawnWrap {
    /**
     * @brief Provides access to the complete set of compiler options.
     *
     * Returns a collection of all supported options,
     * including flags, descriptions, and default values.
     *
     * @return Collection of available options.
     *
     * @see PawnWrap::Option for the option representation.
     */
    const std::vector<struct Option>& get_options();

    /**
     * @brief Compiles a single Pawn source file.
     *
     * Uses the Pawn compiler backend to process the specified source file
     * with the given command-line arguments.
     *
     * @param file_path Path to the Pawn source file to compile.
     * @param args      Command-line arguments.
     *
     * @return \c true if compilation succeeds without fatal errors, otherwise \c false.
     */
    bool compile(const std::filesystem::path& file_path, const std::vector<std::string>& args);

    /**
     * @brief Compiles multiple Pawn source files.
     *
     * Attempts to compile each file in the provided collection using
     * the specified arguments. Compilation stops at the first failure.
     *
     * @param file_paths Collection of source file paths to compile.
     * @param args       Command-line arguments.
     *
     * @return \c true if all files compile successfully, otherwise \c false.
     */
    bool compile(const std::vector<std::filesystem::path>& file_paths, const std::vector<std::string>& args);
}
