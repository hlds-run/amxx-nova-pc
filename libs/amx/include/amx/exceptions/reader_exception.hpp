#pragma once

#include <stdexcept>
#include <string>

namespace Amx::Exceptions {
    /**
     * @brief Base class for AMX reader-related exceptions.
     *
     * Encapsulates errors that occur during AMX file reading.
     */
    class ReaderException : public std::runtime_error {
      public:
        /**
         * @brief Constructs a new reader exception with a detailed message.
         *
         * @param message Human-readable description of the error.
         */
        explicit ReaderException(const std::string& message);
    };

    inline ReaderException::ReaderException(const std::string& message)
        : std::runtime_error("Failed to read AMX file:\n" + message)
    {
    }
}
