#pragma once

#include "amx/header.hpp"
#include "amx/header_debug.hpp"
#include <cstddef>
#include <optional>
#include <vector>

namespace Amx {
    /**
     * @brief Container for AMX file data.
     *
     * Aggregates the essential components of an AMX file.
     */
    struct Data {
        /**
         * @brief Main AMX header.
         *
         * Contains metadata necessary to properly interpret the AMX file.
         */
        Header header{};

        /**
         * @brief Optional debug header.
         *
         * Present only if the AMX file was compiled with debug information.
         * Provides additional metadata such as line number tables
         * and symbol information for debugging purposes.
         */
        std::optional<HeaderDebug> header_debug{};

        /**
         * @brief Raw AMX bytecode body.
         *
         * Stores the entire executable code and data segment of the AMX file.
         */
        std::vector<std::byte> body{};
    };
}
