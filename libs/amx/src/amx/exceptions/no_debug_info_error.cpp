#include "amx/exceptions/reader_error.hpp"
#include "amx/exceptions/reader_exception.hpp"
#include <string>

namespace {
    /**
     * @brief Formats an error message for missing debug information.
     *
     * @param context Additional context string.
     *
     * @return Descriptive message suitable for exception reporting.
     */
    [[nodiscard]] std::string format_message(const std::string& context) noexcept
    {
        std::string message = "Missing debug information";

        if (!context.empty()) {
            message += " in " + context;
        }

        return message;
    }
}

namespace Amx::Exceptions {
    NoDebugInfoError::NoDebugInfoError(const std::string& context) : ReaderException(format_message(context))
    {
    }
}
