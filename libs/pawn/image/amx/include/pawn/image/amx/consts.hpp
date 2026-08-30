#pragma once

#include <cstdint>

namespace Pawn::Image::Amx {
    /**
     * @brief A unique identifier for validating the binary format.
     */
    constexpr std::uint16_t file_magic = 0xF1E0;

    /**
     * @brief A unique identifier for validating the debug information format.
     */
    constexpr std::uint16_t file_magic_debug = 0xF1EF;

    /**
     * @brief Flag indicating availability of symbolic debug information.
     */
    constexpr std::uint16_t flag_debug = 0x02;

    /**
     * @brief Flag indicating compact encoding of the script.
     */
    constexpr std::uint16_t flag_compact = 0x04;

    /**
     * @brief Flag disabling array bounds checking and statement opcodes.
     */
    constexpr std::uint16_t flag_no_checks = 0x10;
}
