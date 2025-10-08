#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Cli {
    /**
     * @brief Resolves paths to configuration files.
     *
     * Determines the effective configuration file paths based on
     * command-line arguments and default search locations.
     * Supports multiple "-T" options, each representing a separate configuration file.
     *
     * @param arguments Parsed command-line arguments.
     *
     * @return Optional vector of absolute paths to configuration files:
     *         - Each element corresponds to one resolved configuration file.
     *         - Returns \c std::nullopt if no user-specified option "-T" is provided.
     *
     * @throw Cli::Exceptions::CliError
     *        - If any "-T" option is specified without a value.
     *        - If a user-specified configuration file does not exist.
     */
    [[nodiscard]] std::optional<std::vector<std::filesystem::path>> resolve_config_paths(
        const class Arguments& arguments);

    /**
     * @brief Loads configuration arguments from a file.
     *
     * Reads configuration lines.
     *
     * @param config_path Absolute or relative path to a configuration file.
     *
     * @return Collection of configuration arguments.
     *
     * @throw \c Exceptions::CliError If the file cannot be opened.
     *
     * @note Lines starting with \c '#' are treated as comments.
     */
    [[nodiscard]] std::vector<std::string> load_config(const std::filesystem::path& config_path);
}
