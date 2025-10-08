#pragma once

#include <cstdint>

#pragma pack(push, 1)
namespace Amxx {
    /**
     * @brief A descriptor for the metadata of a compiled AMX script inside an AMXX container.
     *
     * Stores information about one compiled AMX script in an AMXX container.
     */
    struct Entry {
        /**
         * @brief Size of a cell in the AMX virtual machine.
         *
         * Specifies the size in bytes of a single cell (e.g., 4 bytes for a 32-bit AMX).
         */
        std::uint8_t cell_size{};

        /**
         * @brief Size of the script data in its compressed form on disk.
         *
         * Represents the size in bytes of the compressed AMX script data stored in the AMXX file.
         */
        std::uint32_t disk_size{};

        /**
         * @brief Size of the uncompressed script data.
         *
         * Represents the size in bytes of the original uncompressed AMX script data.
         */
        std::uint32_t image_size{};

        /**
         * @brief Amount of memory required to load and run the script.
         *
         * Specifies the minimum memory size (in bytes) required
         * for the AMX virtual machine to load and run this script.
         * Corresponds to the 'stp' value from the AMX header.
         */
        std::uint32_t memory_size{};

        /**
         * @brief The offset of the start of the compressed data for this script.
         *
         * Specifies the starting position (in bytes) of the compressed AMX script data,
         * relative to the beginning of the AMXX file.
         */
        std::uint32_t offset{};
    };
}
#pragma pack(pop)
