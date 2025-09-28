#include "cli/arguments.hpp"
#include "config.hpp"
#include <algorithm>
#include <cstddef>
#include <ranges>
#include <unordered_map>
#include <utility>

namespace {
    /**
     * @brief Builds normalized argument list from parser state.
     *
     * Combines executable path, options, and definitions into a single argument vector.
     *
     * @param parser   Reference to a parser containing parsed options and definitions.
     * @param exe_path Executable path to place at the beginning of the list.
     *
     * @return Normalized argument list including executable path.
     */
    std::vector<std::string> build_arguments(const Cli::Parser& parser, std::string exe_path)
    {
        const auto& options = parser.options();
        const auto& definitions = parser.definitions();

        std::vector<std::string> arguments{};
        arguments.reserve(options.size() + definitions.size() + 1u);

        // the path to the executable file must be first
        arguments.emplace_back(std::move(exe_path));

        for (const auto& [key, values] : options) {
            // Skip the 'help' options
            if (key == "help" || key == "?") {
                continue;
            }

            // Skip the 'T' option since the configuration file is already loaded
            if (key == "T") {
                continue;
            }

            for (const auto& value : values) {
                if (value) {
                    arguments.emplace_back(("-" + key) += *value);
                }
                else {
                    arguments.emplace_back("-" + key);
                }
            }
        }

        for (const auto& [key, value] : definitions) {
            arguments.emplace_back((key + "=") += value);
        }

        return arguments;
    }
}

namespace Cli {
    Arguments::Arguments(const std::span<const char* const> argv)
        : exe_path_(argv[0]), parser_(std::make_unique<Parser>())
    {
        std::vector<std::string> args{argv.cbegin() + 1, argv.cend()};
        parser_->parse(args, false);

        if (const auto config_paths = resolve_config_paths(*this); config_paths) {
            for (const auto& config_path : *config_paths) {
                args = load_config(config_path);
                parser_->parse(args, false);
            }
        }

        arguments_ = build_arguments(*parser_, exe_path_.string());
    }

    Arguments::Arguments(const int argc, const char* const argv[])
        : Arguments(std::span{argv, static_cast<std::size_t>(argc)})
    {
    }

    bool Arguments::has_option(const std::string_view option) const
    {
        const auto& options = parser_->options();

        const auto is_match = [&](const auto& key) {
            return key == option;
        };

        return std::ranges::any_of(options | std::views::keys, is_match);
    }

    std::optional<std::vector<std::optional<std::string>>> Arguments::get_option_values(
        const std::string_view option) const
    {
        const auto& options = parser_->options();
        const auto& it = options.find(std::string{option});

        if (it != options.end()) {
            return it->second;
        }

        return std::nullopt;
    }
}
