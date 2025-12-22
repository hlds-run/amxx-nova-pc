#include "io/exceptions/stream_error.hpp"
#include "io/exceptions/stream_exception.hpp"
#include <string>

namespace {
    /**
     * @brief Formats a message for FileCloseError.
     *
     * @param filename File path.
     * @param details  Additional information about the failure.
     *
     * @return Formatted descriptive message.
     */
    [[nodiscard]] std::string format_message(const std::string& filename, const std::string& details) noexcept
    {
        std::string message = "Failed to close file";

        if (!filename.empty()) {
            message += ": '" + filename + "'";
        }

        if (!details.empty()) {
            message += " (" + details + ")";
        }

        return message;
    }
}

namespace Io::Exceptions {
    FileCloseError::FileCloseError(const std::string& filename, const std::string& details)
        : StreamException(format_message(filename, details))
    {
    }
}
