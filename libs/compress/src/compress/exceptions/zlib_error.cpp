#include "compress/exceptions/zlib_error.hpp"
#include <zlib.h>

namespace {
    /**
     * @brief Translates a zlib error code into a human-readable description.
     *
     * Provides a textual explanation of the zlib error for diagnostic purposes.
     *
     * @param error_code Integer code returned by zlib.
     *
     * @return Textual description of the error.
     */
    [[nodiscard]] std::string get_error_description(const int error_code) noexcept
    {
        switch (error_code) {
            case Z_OK:
                return "No error";

            case Z_STREAM_END:
                return "Stream end - compression/decompression completed successfully";

            case Z_NEED_DICT:
                return "Need dictionary for decompression";

            case Z_ERRNO:
                return "System error (check errno for details)";

            case Z_STREAM_ERROR:
                return "Invalid compression level, invalid stream state, or inconsistent stream";

            case Z_DATA_ERROR:
                return "Invalid or incomplete deflate data, incorrect checksum";

            case Z_MEM_ERROR:
                return "Out of memory";

            case Z_BUF_ERROR:
                return "Buffer error - output buffer too small or no progress possible";

            case Z_VERSION_ERROR:
                return "Version mismatch - incompatible zlib library version";

            default:
                return "Unknown zlib error";
        }
    }

    /**
     * @brief Formats a complete error message.
     *
     * Produces a diagnostic message suitable for inclusion in exceptions.
     *
     * @param error_code Integer code returned by zlib.
     * @param context    Optional description of the operation that failed.
     */
    [[nodiscard]] std::string format_message(const int error_code, const std::string& context) noexcept
    {
        const std::string& error_description = get_error_description(error_code);
        std::string message = "Zlib error: " + error_description + " (error code: " + std::to_string(error_code) + ")";

        if (!context.empty()) {
            message += " while " + context;
        }

        return message;
    }
}

namespace Compress::Exceptions {
    ZlibError::ZlibError(const int error_code, const std::string& context)
        : CompressException(format_message(error_code, context)), error_code_(error_code)
    {
    }
}
