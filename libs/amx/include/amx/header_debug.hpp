#pragma once

#include <cstdint>

namespace Amx {
    /**
     * @brief A descriptor for debug metadata of compiled AMX script binary data.
     *
     * Stores metadata about the debug information of a compiled AMX script.
     */
    struct HeaderDebug {
        /**
         * @brief Total size of the debug information in bytes.
         *
         * Represents the complete size of the debug data chunk.
         */
        std::uint32_t size{};

        /**
         * @brief Signature identifying the debug format.
         *
         * A unique identifier for validating the debug information format.
         */
        std::uint16_t magic{};

        /**
         * @brief Version of the debug file format.
         *
         * Indicates the format version of the debug data.
         */
        std::int8_t file_version{};

        /**
         * @brief Required version of the AMX virtual machine.
         *
         * Specifies the minimum version of the AMX runtime needed for the debug data.
         */
        std::int8_t amx_version{};

        /**
         * @brief Flags reserved for future use.
         *
         * Currently unused attributes for the debug data.
         */
        std::uint16_t flags{};

        /**
         * @brief Number of entries in the file table.
         *
         * Indicates the count of source file references in the debug information.
         */
        std::int16_t files{};

        /**
         * @brief Number of entries in the line table.
         *
         * Indicates the count of line number entries in the debug information.
         */
        std::int16_t lines{};

        /**
         * @brief Number of entries in the symbol table.
         *
         * Indicates the count of symbol entries in the debug information.
         */
        std::int16_t symbols{};

        /**
         * @brief Number of entries in the tag table.
         *
         * Indicates the count of tag name entries in the debug information.
         */
        std::int16_t tags{};

        /**
         * @brief Number of entries in the automaton table.
         *
         * Indicates the count of automaton entries in the debug information.
         */
        std::int16_t automatons{};

        /**
         * @brief Number of entries in the state table.
         *
         * Indicates the count of state entries in the debug information.
         */
        std::int16_t states{};
    };
}
