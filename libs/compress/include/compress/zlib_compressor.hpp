#pragma once

#include "compress/compressor.hpp"

namespace Compress {
    /**
     * @brief Compressor implementation using the zlib library.
     *
     * Provides compression functionality based on the DEFLATE algorithm.
     *
     * @see Compressor
     */
    class ZlibCompressor final : public Compressor {
      public:
        using Compressor::compress;
        using Compressor::decompress;

        /**
         * @brief Constructs a compressor with the default compression level.
         */
        ZlibCompressor();

        /**
         * @brief Constructs a compressor with the specified compression level.
         *
         * @param level Desired compression level.
         *
         * @note If invalid, falls back to default compression level.
         */
        explicit ZlibCompressor(int level);

        /**
         * @brief Sets the compression level for subsequent operations.
         *
         * @param level Desired compression level.
         */
        void set_compression_level(int level);

        /**
         * @brief Retrieves the currently configured compression level.
         *
         * @return Compression level.
         */
        [[nodiscard]] int get_compression_level() const;

      protected:
        /**
         * @brief Executes zlib-based compression.
         *
         * @param data Contiguous span of raw bytes to compress.
         *
         * @return Compressed data buffer.
         *
         * @throw \c Exceptions::ZlibError If compression fails at the zlib layer.
         */
        [[nodiscard]] CompData do_compress(std::span<const std::byte> data) override;

      private:
        /// Compression level used by zlib backend.
        int compression_level_{};
    };

    inline int ZlibCompressor::get_compression_level() const
    {
        return compression_level_;
    }
}
