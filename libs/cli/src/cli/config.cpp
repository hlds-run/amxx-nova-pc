#include "config.hpp"
#include "cli/arguments.hpp"
#include "cli/exceptions/cli_error.hpp"
#include <cctype>
#include <fstream>
#include <ranges>
#include <string>
#include <string_view>

namespace {
    /**
     * @brief Trims leading and trailing whitespace from a string.
     *
     * @param str Input string.
     *
     * @return A new string with surrounding whitespace removed.
     */
    [[nodiscard]] std::string trim(const std::string_view str)
    {
        auto is_whitespace = [](const auto ch) {
            return std::isspace(static_cast<unsigned char>(ch));
        };

        auto trimmed_view = str | std::views::drop_while(is_whitespace) | std::views::reverse |
                            std::views::drop_while(is_whitespace) | std::views::reverse;

        return std::string{trimmed_view.cbegin(), trimmed_view.cend()};
    }

    /**
     * @brief Locates a configuration file in the "target" directory.
     *
     * Constructs a candidate path and verifies its existence.
     *
     * @param target_dir  Target directory path.
     * @param config_name Configuration name without extension.
     *
     * @return Full path to the located configuration file.
     *
     * @throw \c Cli::Exceptions::CliError If the file does not exist.
     */
    [[nodiscard]] std::filesystem::path find_config_in_target(
        const std::filesystem::path& target_dir, const std::string& config_name)
    {
        const auto config_path = target_dir / (config_name + ".cfg");

        if (std::filesystem::exists(config_path)) {
            return config_path;
        }

        throw Cli::Exceptions::CliError("Configuration file not found: " + config_path.string());
    }

    /**
     * @brief Attempts to locate a default configuration file.
     *
     * Checks for the existence of common default configuration
     * files in predefined locations.
     *
     * @param exe_dir    Directory containing the executable.
     * @param target_dir Target subdirectory relative to \p exe_dir.
     *
     * @return Path to the default configuration if found, or \c std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<std::filesystem::path> find_default_config(
        const std::filesystem::path& exe_dir, const std::filesystem::path& target_dir)
    {
        std::filesystem::path config_path;

        if (config_path = target_dir / "default.cfg"; std::filesystem::exists(config_path)) {
            return config_path;
        }

        if (config_path = exe_dir / "pawn.cfg"; std::filesystem::exists(config_path)) {
            return config_path;
        }

        return std::nullopt;
    }

    /**
     * @brief Collects all user-specified configuration file paths from the command line.
     *
     * Extracts all values for the "-T" option and returns them as a vector of strings.
     *
     * @param arguments Reference to the parsed command-line arguments.
     *
     * @return Optional vector of user-specified configuration file paths.
     *         Returns \c std::nullopt if the option "-T" was not provided.
     *
     * @throw \c Cli::Exceptions::CliError If any "-T" occurrence does not have a value.
     */
    std::optional<std::vector<std::string>> collect_user_configs(const Cli::Arguments& arguments)
    {
        if (!arguments.has_option("T")) {
            return std::nullopt;
        }

        const auto user_config_values = arguments.get_option_values("T");

        if (!user_config_values || user_config_values->empty()) {
            throw Cli::Exceptions::CliError("Option -T requires a configuration file name");
        }

        std::vector<std::string> user_configs{};
        user_configs.reserve(user_config_values->size());

        for (const auto& config_value : *user_config_values) {
            if (!config_value) {
                throw Cli::Exceptions::CliError("Option -T requires a configuration file name");
            }

            user_configs.emplace_back(*config_value);
        }

        return user_configs;
    }

    /**
     * @brief Resolves a single configuration path to an existing file.
     *
     * @param raw_path   Raw configuration path or name.
     * @param target_dir Path to the "target" directory where configs are located.
     *
     * @return Resolved filesystem path to an existing configuration file.
     *
     * @throw \c Cli::Exceptions::CliError If the file does not exist.
     */
    std::filesystem::path resolve_config_path(
        const std::filesystem::path& raw_path, const std::filesystem::path& target_dir)
    {
        if (raw_path.has_parent_path()) {
            if (!std::filesystem::exists(raw_path)) {
                throw Cli::Exceptions::CliError("Specified configuration file does not exist: " + raw_path.string());
            }

            return raw_path;
        }

        return find_config_in_target(target_dir, raw_path.string());
    }
}

namespace Cli {
    std::optional<std::vector<std::filesystem::path>> resolve_config_paths(const Arguments& arguments)
    {
        const auto user_configs = collect_user_configs(arguments);
        const auto exe_dir = arguments.get_executable_path().parent_path();
        const auto target_dir = exe_dir / "target";

        if (user_configs) {
            std::vector<std::filesystem::path> config_paths{};
            config_paths.reserve(user_configs->size());

            for (const auto& user_config : *user_configs) {
                config_paths.emplace_back(resolve_config_path(user_config, target_dir));
            }

            return config_paths;
        }

        if (const auto config_path = find_default_config(exe_dir, target_dir)) {
            return std::vector{*config_path};
        }

        return std::nullopt;
    }

    std::vector<std::string> load_config(const std::filesystem::path& config_path)
    {
        std::ifstream config_file{config_path, std::ios::in};

        if (!config_file.is_open()) {
            throw Exceptions::CliError("Unable to open configuration file: " + config_path.string());
        }

        std::string line{};
        std::vector<std::string> args{};

        while (std::getline(config_file, line)) {
            line = trim(line);

            if (!line.empty() && line.front() != '#') {
                args.push_back(line);
            }
        }

        return args;
    }
}
