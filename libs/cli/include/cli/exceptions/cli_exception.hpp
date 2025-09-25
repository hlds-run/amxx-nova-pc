#pragma once

#include <stdexcept>
#include <string>

namespace Cli::Exceptions {
    /**
     * @brief Base class for command-line related exceptions.
     *
     * Provides a common interface for all CLI-specific errors by
     * extending the standard \c std::runtime_error.
     *
     * @note Intended as a parent type for more specific CLI errors.
     */
    class CliException : public std::runtime_error {
      public:
        /**
         * @brief Constructs an exception with a descriptive message.
         *
         * @param message Human-readable description of the error.
         */
        explicit CliException(const std::string& message);
    };

    inline CliException::CliException(const std::string& message)
        : std::runtime_error("Invalid command-line arguments:\n" + message)
    {
    }
}
