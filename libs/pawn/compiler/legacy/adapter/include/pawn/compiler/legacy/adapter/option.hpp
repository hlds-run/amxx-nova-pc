#pragma once

#include <optional>
#include <string>

namespace PawnWrap {
    /**
     * @brief Describes a compiler command-line option.
     *
     * @note All flags are case-sensitive
     *       and may include required or optional parameters.
     */
    struct Option {
        /// Command-line flag representation.
        std::string flag{};

        /// Human-readable explanation of the option's purpose.
        std::string description{};

        /// Default value, if any, expressed as a string.
        std::optional<std::string> default_value{};
    };
}
