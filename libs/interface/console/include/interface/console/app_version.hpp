#pragma once

#include <string>

namespace Interface::Console {
    /**
     * @brief Structured application version metadata.
     */
    struct AppVersion {
        int major{};          ///< Major version number.
        int minor{};          ///< Minor version number.
        int patch{};          ///< Patch version number.
        int tweak{};          ///< Tweak or build iteration number.
        int year{};           ///< Year of the build.
        std::string string{}; ///< Human-readable version string.
    };
}
