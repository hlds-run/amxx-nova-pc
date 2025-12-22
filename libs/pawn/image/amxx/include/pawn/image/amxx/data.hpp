#pragma once

#include "pawn/image/amxx/entry.hpp"
#include "pawn/image/amxx/header.hpp"
#include <cstddef>
#include <vector>

namespace Pawn::Image::Amxx {
    /**
     * @brief Container for AMXX file data.
     *
     * Aggregates the essential components of an AMXX file.
     */
    struct Data {
        /**
         * @brief Main AMXX file header.
         *
         * Contains metadata necessary to properly interpret the AMXX file.
         */
        Header header{};

        /**
         * @brief List of AMXX entry descriptors.
         *
         * Each entry describes one compiled script segment within the AMXX file.
         * Multiple entries allow a single AMXX file to contain
         * more than one compiled script segment.
         */
        std::vector<Entry> entries{};

        /**
         * @brief Raw AMX bytecode bodies.
         *
         * Each inner vector contains the executable code and data segment
         * corresponding to a single entry.
         * The number and order of bodies matches the number
         * and order of elements in \c entries
         */
        std::vector<std::vector<std::byte>> bodies{};
    };
}
