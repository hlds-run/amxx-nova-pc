#include "cli/parser.hpp"
#include "cli/exceptions/cli_error.hpp"
#include "pawnwrap/compiler.hpp"
#include "pawnwrap/option.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <string_view>
#include <utility>

namespace {
    /**
     * @brief Types of recognized command-line arguments.
     */
    enum class ArgType : std::uint8_t {
        option,     ///< Command-line option.
        definition, ///< Symbol definition.
        input_file  ///< Input file path.
    };

    /**
     * @brief Determines the type of the argument.
     *
     * @param arg Argument string to analyze.
     *
     * @return Type of argument.
     */
    [[nodiscard]] constexpr ArgType get_argument_type(const std::string_view arg) noexcept
    {
        if (arg.length() <= 1) {
            return ArgType::input_file;
        }

        if (arg[0] == '-' || arg[0] == '/') {
            return ArgType::option;
        }

        if (arg.find('=') != std::string::npos) {
            return ArgType::definition;
        }

        return ArgType::input_file;
    }

    /**
     * @brief Determines the key length of an option.
     *
     * @param arg Option argument string.
     *
     * @return Length of the option key portion.
     */
    [[nodiscard]] std::size_t get_option_key_length(const std::string_view arg) noexcept
    {
        if (arg.starts_with("--")) {
            return arg.length();
        }

        return arg.starts_with("-sui") || arg.starts_with("/sui") ? 4u : 2u;
    }

    /**
     * @brief Parses an option argument into key-value form.
     *
     * @param arg Option argument string (with prefix).
     *
     * @return Pair of normalized option key and optional value.
     */
    [[nodiscard]] std::pair<std::string, std::optional<std::string>> parse_option(const std::string_view arg) noexcept
    {
        const auto key_length = get_option_key_length(arg);

        if (arg.starts_with("--")) {
            return {std::string{arg.substr(2)}, std::nullopt};
        }

        if (arg.length() <= key_length) {
            return {std::string{arg.substr(1)}, std::nullopt};
        }

        std::string key{arg.substr(0, key_length)};
        std::string value{arg.substr(key_length)};

        if (key.front() == '-' || key.front() == '/') {
            key.erase(0, 1);
        }

        if (value.front() == ':' || value.front() == '=') {
            value.erase(0, 1);
        }

        if (value.empty()) {
            return {std::move(key), std::nullopt};
        }

        return {std::move(key), std::make_optional(std::move(value))};
    }

    /**
     * @brief Parses a definition argument into symbol and value.
     *
     * @param arg Definition string in the form `symbol=value`.
     *
     * @return Pair of symbol and value.
     *
     * @throw \c Cli::Exceptions::CliError if format is invalid or symbol is empty.
     */
    [[nodiscard]] std::pair<std::string, std::string> parse_definition(const std::string_view arg)
    {
        const auto eq_pos = arg.find('=');

        if (eq_pos == std::string_view::npos) {
            throw Cli::Exceptions::CliError{
                "Invalid definition format: expected 'symbol=value', actual: " + std::string{arg}};
        }

        auto symbol = std::string{arg.substr(0, eq_pos)};
        auto value = std::string{arg.substr(eq_pos + 1)};

        if (symbol.empty()) {
            throw Cli::Exceptions::CliError("Definition symbol cannot be empty");
        }

        if (value.empty()) {
            value = "1";
        }

        return std::make_pair(std::move(symbol), std::move(value));
    }

    /**
     * @brief Validates whether a given option key matches any of the declared compiler options.
     *
     * @param option Normalized option key without prefix characters.
     *
     * @return \c true if the key matches any known flag prefix, otherwise \c false.
     *
     * @note Does not validate the semantic correctness of a numeric or symbolic value.
     *
     * @see \c Cli::Parser::add_option
     * @see \c PawnWrap::get_options
     */
    [[nodiscard]] bool is_valid_option(const std::string& option)
    {
        if (option.empty()) {
            return false;
        }

        if (option == "help" || option == "?") {
            return true;
        }

        const auto check_prefix = [&](const auto& opt) {
            return opt.flag.starts_with("-" + option);
        };

        return std::ranges::any_of(PawnWrap::get_options(), check_prefix);
    }
}

namespace Cli {
    void Parser::parse(const std::vector<std::string>& args, const bool override)
    {
        for (const auto& arg : args) {
            if (arg.empty()) {
                continue;
            }

            switch (get_argument_type(arg)) {
                case ArgType::option:
                    add_option(parse_option(arg), override);
                    break;

                case ArgType::definition:
                    add_definition(parse_definition(arg), override);
                    break;

                case ArgType::input_file:
                    add_input_file(std::filesystem::path{arg});
                    break;
            }
        }
    }

    void Parser::add_option(const std::pair<std::string, std::optional<std::string>>& option, const bool override)
    {
        const auto& key = option.first;
        const auto& value = option.second;

        if (key.empty()) {
            return;
        }

        if (!is_valid_option(key)) {
            throw Exceptions::CliError("Unknown option: -" + key);
        }

        if (override) {
            options_[key].clear();
        }

        options_[key].emplace_back(value);
    }

    void Parser::add_definition(const std::pair<std::string, std::string>& definition, const bool override)
    {
        const auto& key = definition.first;
        const auto& value = definition.second;

        if (key.empty()) {
            return;
        }

        if (override || !definitions_.contains(key)) {
            definitions_[key] = value;
        }
    }

    void Parser::add_input_file(std::filesystem::path file_path)
    {
        if (std::ranges::find(input_files_, file_path) != input_files_.end()) {
            return;
        }

        if (!std::filesystem::exists(file_path)) {
            throw Exceptions::CliError("Input file does not exist: " + file_path.string());
        }

        input_files_.emplace_back(std::move(file_path));
    }
}
