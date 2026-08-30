#pragma once

#include <cstdint>

#pragma pack(push, 1)
namespace Pawn::Image::Amxx {
    /**
     * @brief AMXX binary file header descriptor.
     *
     * Stores metadata about the binary representation of a compiled AMXX script.
     */
    struct Header {
        /**
         * @brief Signature identifying the AMX format.
         *
         * A unique identifier for validating the binary format.
         */
        std::uint32_t magic{};

        /**
         * @brief Version of the file format.
         *
         * Indicates the format version of the binary data.
         */
        std::uint16_t file_version{};

        /**
         * @brief Number of compiled script entries in the file.
         *
         * Specifies how many entry structures follow this header.
         * Typically, 1 for standard AMXX files.
         */
        std::uint8_t entry_count{};
    };
}
#pragma pack(pop)
