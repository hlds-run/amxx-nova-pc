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
        auto is_space = [](const auto ch) {
            return std::isspace(static_cast<unsigned char>(ch));
        };

        auto view = str | std::views::drop_while(is_space) | std::views::reverse | std::views::drop_while(is_space) |
                    std::views::reverse;

        return std::string{view.begin(), view.end()};
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
}

namespace Cli {
    std::optional<std::filesystem::path> resolve_config_path(const Arguments& arguments)
    {
        std::filesystem::path config_path{};

        if (arguments.has_option("T")) {
            const auto user_config = arguments.get_option_value("T");

            if (!user_config) {
                throw Exceptions::CliError("Option -T requires a configuration file name");
            }

            if (config_path = *user_config; config_path.has_parent_path()) {
                if (std::filesystem::exists(config_path)) {
                    return config_path;
                }

                throw Exceptions::CliError("Specified configuration file does not exist: " + config_path.string());
            }
        }

        const auto exe_dir = arguments.get_executable_path().parent_path();
        const auto target_dir = exe_dir / "target";

        if (config_path.empty()) {
            return find_default_config(exe_dir, target_dir);
        }

        return find_config_in_target(target_dir, config_path.string());
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
