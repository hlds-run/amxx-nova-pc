#include "io/exceptions/stream_error.hpp"
#include "io/exceptions/stream_exception.hpp"
#include <string>

namespace {
    /**
     * @brief Formats a message for EndOfFileError.
     *
     * @param context Optional operation description to include in the message.
     *
     * @return Formatted descriptive message.
     */
    [[nodiscard]] std::string format_message(const std::string& context) noexcept
    {
        std::string message = "An unexpected end of file was reached";

        if (!context.empty()) {
            message += " while " + context;
        }

        return message;
    }
}

namespace Io::Exceptions {
    EndOfFileError::EndOfFileError(const std::string& context) : StreamException(format_message(context))
    {
    }
}
