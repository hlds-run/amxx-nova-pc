#include "core/io/exceptions/stream_error.hpp"
#include "core/io/exceptions/stream_exception.hpp"
#include <string>

namespace {
    /**
     * @brief Formats a message for FileOpenError.
     *
     * @param filename File path.
     * @param mode     Mode in which the file was attempted to open.
     * @param details  Additional information about the failure.
     *
     * @return Formatted descriptive message.
     */
    [[nodiscard]] std::string format_message(
        const std::string& filename, const std::string& mode, const std::string& details) noexcept
    {
        std::string message = "Failed to open file";

        if (!filename.empty()) {
            message += ": '" + filename + "'";
        }

        if (!mode.empty()) {
            message += " for " + mode;
        }

        if (!details.empty()) {
            message += " (" + details + ")";
        }

        return message;
    }
}

namespace Core::Io::Exceptions {
    FileOpenError::FileOpenError(const std::string& filename, const std::string& mode, const std::string& details)
        : StreamException(format_message(filename, mode, details))
    {
    }
}
