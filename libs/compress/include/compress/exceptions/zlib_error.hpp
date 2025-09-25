#pragma once

#include "compress/exceptions/compress_exception.hpp"
#include <string>

namespace Compress::Exceptions {
    /**
     * @brief Exception representing errors reported by the zlib library.
     *
     * Encapsulates a zlib error code along with contextual information.
     * Converts raw zlib error codes into human-readable descriptions.
     */
    class ZlibError final : public CompressException {
      public:
        /**
         * @brief Constructs a zlib error exception.
         *
         * @param error_code Numeric error code returned by zlib.
         * @param context    Optional description of the operation being performed.
         */
        explicit ZlibError(int error_code, const std::string& context = "");

        /**
         * @brief Retrieves the raw zlib error code.
         *
         * @return Integer error code as returned by zlib.
         */
        [[nodiscard]] int error_code() const noexcept;

      private:
        /// Numeric error code provided by the zlib library.
        int error_code_;
    };

    inline int ZlibError::error_code() const noexcept
    {
        return error_code_;
    }
}
