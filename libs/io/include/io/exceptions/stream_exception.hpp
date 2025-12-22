#pragma once

#include <stdexcept>
#include <string>

namespace Io::Exceptions {
    /**
     * @brief Base class for all stream-related exceptions.
     *
     * Provides a common interface for file and stream error handling.
     * Inherits from \c std::runtime_error to allow standard exception handling.
     */
    class StreamException : public std::runtime_error {
      public:
        /**
         * @brief Constructs a stream exception with a descriptive message.
         *
         * @param message Human-readable message describing the error condition.
         */
        explicit StreamException(const std::string& message);
    };

    inline StreamException::StreamException(const std::string& message)
        : std::runtime_error("File stream error:\n" + message)
    {
    }
}
