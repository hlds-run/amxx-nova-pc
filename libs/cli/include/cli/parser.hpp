#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Cli {
    /**
     * @brief Command-line arguments parser.
     *
     * Interprets raw command-line arguments and categorizes them into
     * options, preprocessor-style definitions, and input file paths.
     */
    class Parser {
      public:
        /**
         * @brief Collection of raw command-line arguments.
         */
        using Arguments = std::vector<std::string>;

        /**
         * @brief Mapping of recognized options.
         */
        using OptionsMap = std::unordered_map<std::string, std::optional<std::string>>;

        /**
         * @brief Mapping of symbol definitions.
         */
        using DefinitionsMap = std::unordered_map<std::string, std::string>;

        /**
         * @brief Collection of validated input file paths.
         */
        using InputFiles = std::vector<std::filesystem::path>;

        /**
         * @brief Parses command-line arguments into options, definitions, and input files.
         *
         * Iterates over provided arguments and dispatches them
         * into their respective categories based on syntax conventions.
         *
         * @param args     Argument list to parse.
         * @param override If \c true, previously set options or definitions may be replaced by newer ones.
         *
         * @throw \c Exceptions::CliError if a definition is malformed or if an input file does not exist.
         */
        void parse(const Arguments& args, bool override);

        /**
         * @brief Retrieves parsed options.
         *
         * @return Options map.
         *
         * @note Lookup by key should account for normalized option names
         *       without prefix characters.
         */
        [[nodiscard]] const OptionsMap& options() const noexcept;

        /**
         * @brief Retrieves parsed symbol definitions.
         *
         * @return Definitions map.
         */
        [[nodiscard]] const DefinitionsMap& definitions() const noexcept;

        /**
         * @brief Retrieves validated input files.
         *
         * @return Input files list.
         */
        [[nodiscard]] const InputFiles& input_files() const noexcept;

      private:
        /// Stores parsed options keyed by normalized option name.
        OptionsMap options_{};

        /// Stores parsed definitions keyed by symbol name.
        DefinitionsMap definitions_{};

        /// Stores validated input files.
        InputFiles input_files_{};

        /**
         * @brief Adds an option to the internal map.
         *
         * @param option   Pair of option key and optional value.
         * @param override Determines if existing entries can be replaced.
         */
        void add_option(const std::pair<std::string, std::optional<std::string>>& option, bool override);

        /**
         * @brief Adds a definition to the internal map.
         *
         * @param definition Pair of symbol and associated value.
         * @param override   Determines if existing entries can be replaced.
         *
         * @throw \c Exceptions::CliError if symbol is empty.
         */
        void add_definition(const std::pair<std::string, std::string>& definition, bool override);

        /**
         * @brief Adds a file path to the list of input files.
         *
         * @param file_path Filesystem path to be added.
         *
         * @throw \c Exceptions::CliError if file does not exist.
         *
         * @note Duplicate file paths are ignored.
         */
        void add_input_file(std::filesystem::path file_path);
    };

    inline const Parser::OptionsMap& Parser::options() const noexcept
    {
        return options_;
    }

    inline const Parser::DefinitionsMap& Parser::definitions() const noexcept
    {
        return definitions_;
    }

    inline const Parser::InputFiles& Parser::input_files() const noexcept
    {
        return input_files_;
    }
}
