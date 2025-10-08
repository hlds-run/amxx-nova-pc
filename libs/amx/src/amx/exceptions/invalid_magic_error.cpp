#include "amx/exceptions/reader_error.hpp"
#include "amx/exceptions/reader_exception.hpp"
#include <cstdint>
#include <string>

namespace {
    /**
     * @brief Formats an error message for invalid magic numbers.
     *
     * Constructs a diagnostic string indicating the expected and actual
     * magic values, with optional context information.
     *
     * @param expected The correct magic number.
     * @param actual   The magic number found in the file.
     * @param context  Additional context such as "AMX file header".
     *
     * @return Descriptive message suitable for exception reporting.
     */
    [[nodiscard]] std::string format_message(
        const std::uint16_t expected, const std::uint16_t actual, const std::string& context) noexcept
    {
        const auto expected_str = std::to_string(expected);
        const auto actual_str = std::to_string(actual);
        std::string message = "Invalid magic number. Expected: " + expected_str + ", actual: " + actual_str;

        if (!context.empty()) {
            message += " in " + context;
        }

        return message;
    }
}

namespace Amx::Exceptions {
    InvalidMagicError::InvalidMagicError(
        const std::uint16_t expected, const std::uint16_t actual, const std::string& context)
        : ReaderException(format_message(expected, actual, context)), expected_(expected), actual_(actual)
    {
    }

    std::uint16_t InvalidMagicError::expected() const noexcept
    {
        return expected_;
    }

    std::uint16_t InvalidMagicError::actual() const noexcept
    {
        return actual_;
    }
}
