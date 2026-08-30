#include "core/io/exceptions/stream_error.hpp"
#include "core/io/exceptions/stream_exception.hpp"
#include <ios>
#include <string>

namespace {
    /**
     * @brief Converts \c std::ios::seekdir to human-readable string.
     *
     * @param direction Seek direction.
     *
     * @return String describing the seek origin.
     */
    [[nodiscard]] std::string seekdir_to_string(const std::ios::seekdir direction)
    {
        switch (direction) {
            case std::ios::beg:
                return "beginning";

            case std::ios::cur:
                return "current position";

            case std::ios::end:
                return "end";

            default:
                return "unknown";
        }
    }

    /**
     * @brief Formats a message for SeekError.
     *
     * @param offset    Target offset.
     * @param direction Base direction for seeking.
     * @param context   Optional operation description.
     *
     * @return Formatted descriptive message.
     */
    [[nodiscard]] std::string format_message(
        const std::streamoff offset, const std::ios::seekdir direction, const std::string& context) noexcept
    {
        const std::string& direction_str = seekdir_to_string(direction);
        std::string message = "Failed to seek to offset " + std::to_string(offset) + " from " + direction_str;

        if (!context.empty()) {
            message += " while navigating to " + context;
        }

        return message;
    }
}

namespace Core::Io::Exceptions {
    SeekError::SeekError(const std::streamoff offset, const std::ios::seekdir direction, const std::string& context)
        : StreamException(format_message(offset, direction, context))
    {
    }
}
