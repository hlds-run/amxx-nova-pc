#pragma once

#include <cstdint>

namespace Amxx {
    /**
     * @brief A unique identifier for validating the binary format.
     */
    constexpr std::uint32_t file_magic = 0x414D5858;

    /**
     * @brief Version of the file format.
     */
    constexpr std::uint16_t file_version = 0x0300;
}
