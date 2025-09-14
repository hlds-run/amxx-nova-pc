#pragma once

#include <stdexcept>
#include <string>

namespace Compress::Exceptions {
    /**
     * @brief Base class for compression-related exceptions.
     *
     * Represents errors occurring during compression or decompression operations.
     */
    class CompressException : public std::runtime_error {
      public:
        /**
         * @brief Constructs a compression exception with a descriptive message.
         *
         * @param message Human-readable error description.
         */
        explicit CompressException(const std::string& message);
    };

    inline CompressException::CompressException(const std::string& message)
        : std::runtime_error("Compression failed:\n" + message)
    {
    }
}
