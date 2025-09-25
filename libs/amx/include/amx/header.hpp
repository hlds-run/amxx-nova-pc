#pragma once

#include <cstdint>

namespace Amx {
    /**
     * @brief AMX binary file header descriptor.
     *
     * Stores metadata about the binary representation of a compiled AMX script.
     */
    struct Header {
        /**
         * @brief Total size of the binary data in bytes.
         *
         * Represents the complete size of the compiled script, including all sections.
         */
        std::uint32_t size{};

        /**
         * @brief Signature identifying the AMX format.
         *
         * A unique identifier for validating the binary format.
         */
        std::uint16_t magic{};

        /**
         * @brief Version of the file format.
         *
         * Indicates the format version of the binary data.
         */
        std::int8_t file_version{};

        /**
         * @brief Required version of the AMX virtual machine.
         *
         * Specifies the minimum version of the AMX runtime needed for execution.
         */
        std::int8_t amx_version{};

        /**
         * @brief Flags defining script properties.
         *
         * Specifies attributes or options for the compiled script.
         */
        std::uint16_t flags{};

        /**
         * @brief Size of a definition record in bytes.
         *
         * Specifies the size of each record in definition tables.
         */
        std::int16_t def_size{};

        /**
         * @brief Offset to the start of the code block.
         *
         * Indicates the starting position of the executable code within the binary data.
         */
        std::int32_t cod{};

        /**
         * @brief Offset to the start of the data block.
         *
         * Indicates the starting position of the initialized data within the binary data.
         */
        std::int32_t dat{};

        /**
         * @brief Initial position of the heap.
         *
         * Represents the starting point of the heap memory for the script.
         */
        std::int32_t hea{};

        /**
         * @brief Initial position of the stack top.
         *
         * Represents the top of the stack and heap memory for the script.
         */
        std::uint32_t stp{};

        /**
         * @brief Initial instruction pointer.
         *
         * Specifies the starting address of the script's execution.
         */
        std::int32_t cip{};

        /**
         * @brief Offset to the public functions table.
         *
         * Indicates the position of the table containing public function entries.
         */
        std::int32_t publics{};

        /**
         * @brief Offset to the native functions table.
         *
         * Indicates the position of the table containing native function entries.
         */
        std::int32_t natives{};

        /**
         * @brief Offset to the libraries table.
         *
         * Indicates the position of the table listing required libraries.
         */
        std::int32_t libraries{};

        /**
         * @brief Offset to the public variables table.
         *
         * Indicates the position of the table containing public variable entries.
         */
        std::int32_t pub_vars{};

        /**
         * @brief Offset to the public tag names table.
         *
         * Indicates the position of the table containing tag name entries.
         */
        std::int32_t tags{};

        /**
         * @brief Offset to the name table.
         *
         * Indicates the position of the table containing symbol names.
         */
        std::int32_t name_table{};
    };
}
