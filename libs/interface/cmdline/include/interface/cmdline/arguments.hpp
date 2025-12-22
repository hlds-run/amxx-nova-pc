#pragma once

#include "interface/cmdline/parser.hpp"
#include <filesystem>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace Interface::Cmdline {
    /**
     * @brief Command-line arguments wrapper.
     */
    class Arguments {
      public:
        /**
         * @brief Constructs arguments object from a span of C-style strings.
         *
         * @param argv Array span representing program arguments.
         */
        explicit Arguments(std::span<const char* const> argv);

        /**
         * @brief Constructs arguments object from argc/argv pair.
         *
         * @param argc Number of arguments.
         * @param argv Array of C-style strings.
         */
        explicit Arguments(int argc, const char* const argv[]);

        /**
         * @brief Checks if an option is present.
         *
         * @param option Option name without prefix characters.
         *
         * @return \c true if the option exists, otherwise \c false.
         */
        [[nodiscard]] bool has_option(std::string_view option) const;

        /**
         * @brief Retrieves all values associated with a given option.
         *
         * @param option Option name without prefix characters.
         *
         * @return An optional vector of values for the specified option.
         *         Each element represents one occurrence of the option:
         *         - If the option was not provided, returns \c std::nullopt.
         *         - If an occurrence had no explicit value, the corresponding element is \c std::nullopt.
         */
        [[nodiscard]] std::optional<std::vector<std::optional<std::string>>> get_option_values(
            std::string_view option) const;

        /**
         * @brief Retrieves the last specified value for an option.
         *
         * Iterates through all occurrences of the option and returns the last one,
         * including empty (\c std::nullopt) values.
         *
         * @param option Option name without prefix characters.
         *
         * @return The last specified value of the option, or \c std::nullopt if the option does not exist.
         */
        [[nodiscard]] std::optional<std::string> get_last_option_value(std::string_view option) const;

        /**
         * @brief Retrieves input files.
         *
         * @return Collection of validated input file paths.
         */
        [[nodiscard]] const std::vector<std::filesystem::path>& get_input_files() const noexcept;

        /**
         * @brief Retrieves executable path.
         *
         * @return Path to the running executable.
         */
        [[nodiscard]] const std::filesystem::path& get_executable_path() const noexcept;

        /**
         * @brief Retrieves normalized argument list.
         *
         * @return Collection of arguments including executable path, options, and definitions.
         */
        [[nodiscard]] const std::vector<std::string>& get_arguments() const noexcept;

      private:
        /// Path to the running executable.
        std::filesystem::path exe_path_;

        /// Internal parser for processing raw arguments.
        std::unique_ptr<Parser> parser_;

        /// Normalized argument list.
        std::vector<std::string> arguments_{};
    };

    inline const std::vector<std::filesystem::path>& Arguments::get_input_files() const noexcept
    {
        return parser_->input_files();
    }

    inline const std::filesystem::path& Arguments::get_executable_path() const noexcept
    {
        return exe_path_;
    }

    inline const std::vector<std::string>& Arguments::get_arguments() const noexcept
    {
        return arguments_;
    }
}
