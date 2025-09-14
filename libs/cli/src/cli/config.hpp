#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Cli {
    /**
     * @brief Resolves the path to the configuration file.
     *
     * Determines the effective configuration file path based on
     * command-line arguments and default search locations.
     *
     * @param arguments Parsed command-line arguments.
     *
     * @return Absolute path to the configuration file if found,
     *         or \c std::nullopt if no applicable configuration exists.
     *
     * @throw \c Exceptions::CliError
     *        - If option \c -T is specified without a value.
     *        - If a user-specified configuration path does not exist.
     */
    [[nodiscard]] std::optional<std::filesystem::path> resolve_config_path(const class Arguments& arguments);

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
