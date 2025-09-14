#pragma once

#include "binary/exceptions/stream_exception.hpp"
#include <ios>

namespace Binary::Exceptions {
    /**
     * @brief Signals failure when attempting to open a file.
     *
     * Indicates that the file could not be accessed in the specified mode.
     */
    class FileOpenError final : public StreamException {
      public:
        /**
         * @brief Constructs an exception indicating file open failure.
         *
         * @param filename File path that failed to open.
         * @param mode     Mode used for opening (e.g., "r", "wb").
         * @param details  Additional context or error information.
         */
        explicit FileOpenError(
            const std::string& filename = "", const std::string& mode = "", const std::string& details = "");
    };

    /**
     * @brief Signals failure when attempting to close a file.
     *
     * Indicates an error occurred while flushing or releasing resources associated with the file.
     */
    class FileCloseError final : public StreamException {
      public:
        /**
         * @brief Constructs an exception indicating file close failure.
         *
         * @param filename File path being closed.
         * @param details  Optional additional details about the failure.
         */
        explicit FileCloseError(const std::string& filename = "", const std::string& details = "");
    };

    /**
     * @brief Signals a general failure during stream operations.
     *
     * Used for low-level I/O errors not covered by other specific exceptions.
     */
    class StreamFailureError final : public StreamException {
      public:
        /**
         * @brief Constructs an exception indicating general stream failure.
         *
         * @param details Optional descriptive context for the failure.
         */
        explicit StreamFailureError(const std::string& details = "");
    };

    /**
     * @brief Signals an unexpected end of file.
     *
     * Indicates an attempt to read beyond the available stream data.
     */
    class EndOfFileError final : public StreamException {
      public:
        /**
         * @brief Constructs an exception indicating premature EOF.
         *
         * @param context Optional description of the operation during which the EOF occurred.
         */
        explicit EndOfFileError(const std::string& context = "");
    };

    /**
     * @brief Signals failure to reposition the stream cursor.
     *
     * Indicates that seeking to a given offset from a specified direction failed.
     */
    class SeekError final : public StreamException {
      public:
        /**
         * @brief Constructs an exception indicating seek failure.
         *
         * @param offset    Target offset for seeking.
         * @param direction Base position for offset (default \c std::ios::beg).
         * @param context   Optional description of the operation during which the seek failed.
         */
        explicit SeekError(
            std::streamoff offset, std::ios::seekdir direction = std::ios::beg, const std::string& context = "");
    };
}
