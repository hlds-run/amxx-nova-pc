#include "binary/exceptions/stream_error.hpp"
#include "binary/exceptions/stream_exception.hpp"
#include <string>

namespace {
    /**
     * @brief Formats a message for StreamFailureError.
     *
     * @param details Optional descriptive details of the failure.
     *
     * @return Formatted descriptive message.
     */
    [[nodiscard]] std::string format_message(const std::string& details) noexcept
    {
        std::string message = "Stream operation failed";

        if (!details.empty()) {
            message += ": " + details;
        }

        return message;
    }
}

namespace Binary::Exceptions {
    StreamFailureError::StreamFailureError(const std::string& details) : StreamException(format_message(details))
    {
    }
}
