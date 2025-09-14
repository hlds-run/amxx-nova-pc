#pragma once

#include "amx/exceptions/reader_exception.hpp"
#include <cstdint>
#include <string>

namespace Amx::Exceptions {
    /**
     * @brief Raised when an invalid magic number is encountered in an AMX file.
     */
    class InvalidMagicError final : public ReaderException {
      public:
        /**
         * @brief Constructs the exception with expected and actual magic values.
         *
         * @param expected The correct magic number required by the AMX specification.
         * @param actual   The actual magic number encountered in the file.
         * @param context  Optional context string indicating where the error occurred.
         */
        InvalidMagicError(std::uint16_t expected, std::uint16_t actual, const std::string& context = "");

        /**
         * @brief Retrieves the expected magic number.
         *
         * @return Expected magic value as defined by AMX format.
         */
        [[nodiscard]] std::uint16_t expected() const noexcept;

        /**
         * @brief Retrieves the actual magic number that caused the error.
         *
         * @return Actual magic value found in the file.
         */
        [[nodiscard]] std::uint16_t actual() const noexcept;

      private:
        /// The expected magic value from AMX specification.
        std::uint16_t expected_;

        /// The actual magic value encountered in the file.
        std::uint16_t actual_;
    };

    /**
     * @brief Raised when debug information is expected but not found in an AMX file.
     */
    class NoDebugInfoError final : public ReaderException {
      public:
        /**
         * @brief Constructs the exception for missing debug information.
         *
         * @param context Optional string describing the context of the missing debug info.
         */
        explicit NoDebugInfoError(const std::string& context = "");
    };
}
