#pragma once

#include "core/concepts/concepts.hpp"
#include <cstddef>
#include <expected>
#include <ranges>
#include <span>
#include <vector>

namespace Core::Compress {
    /**
     * @brief Abstract interface for generic compression and decompression operations.
     *
     * Provides a common contract for data compression algorithms
     * operating on contiguous sequences of bytes.
     *
     * @note Designed for polymorphic use, allowing multiple compression
     *       backends (e.g., zlib, LZ4, custom algorithms).
     */
    class Compressor {
      public:
        /**
         * @brief A sequence of compressed or decompressed data.
         */
        using CompData = std::vector<std::byte>;

        /// Default constructor.
        Compressor() = default;

        /// Copy constructor.
        Compressor(Compressor&) = default;

        /// Move constructor.
        Compressor(Compressor&&) noexcept = default;

        /// Virtual destructor for safe polymorphic cleanup.
        virtual ~Compressor() = default;

        /// Copy assignment operator.
        Compressor& operator=(const Compressor&) = default;

        /// Move assignment operator.
        Compressor& operator=(Compressor&&) noexcept = default;

        /**
         * @brief Compresses a single trivially copyable byte-safe object.
         *
         * @tparam T Type satisfying \c Core::Concepts::ByteSafe.
         *
         * @param data The object to compress.
         *
         * @return Compressed representation of the object.
         */
        template <Concepts::ByteSafe T>
        [[nodiscard]] CompData compress(const T& data);

        /**
         * @brief Compresses data from a contiguous container of byte-safe elements.
         *
         * @tparam T Type satisfying \c Core::Concepts::ContiguousByteSafeContainer.
         *
         * @param data The container holding data to compress.
         *
         * @return Compressed representation of the container contents.
         */
        template <Concepts::ContiguousByteSafeContainer T>
        [[nodiscard]] CompData compress(const T& data);

        /**
         * @brief Decompresses data from a contiguous container of byte-safe elements.
         *
         * @tparam T Type satisfying \c Core::Concepts::ContiguousByteSafeContainer.
         *
         * @param data The container holding compressed data.
         *
         * @return Decompressed representation of the container contents.
         */
        template <Concepts::ContiguousByteSafeContainer T>
        [[nodiscard]] CompData decompress(const T& data);

      protected:
        /**
         * @brief Core compression routine implemented by subclasses.
         *
         * @param data Contiguous span of bytes to compress.
         *
         * @return Compressed sequence of bytes.
         */
        [[nodiscard]] virtual CompData do_compress(std::span<const std::byte> data) = 0;

        /**
         * @brief Core decompression routine, optionally overridden by subclasses.
         *
         * @param data Contiguous span of compressed data.
         *
         * @return Decompressed sequence of bytes.
         */
        [[nodiscard]] virtual CompData do_decompress(std::span<const std::byte> data);
    };

    template <Concepts::ByteSafe T>
    Compressor::CompData Compressor::compress(const T& data)
    {
        const auto bytes = std::as_bytes(std::span{&data, 1});
        return this->do_compress(bytes);
    }

    template <Concepts::ContiguousByteSafeContainer T>
    Compressor::CompData Compressor::compress(const T& data)
    {
        const auto bytes = std::as_bytes(std::span{std::ranges::data(data), std::ranges::size(data)});
        return this->do_compress(bytes);
    }

    template <Concepts::ContiguousByteSafeContainer T>
    Compressor::CompData Compressor::decompress(const T& data)
    {
        const auto bytes = std::as_bytes(std::span{std::ranges::data(data), std::ranges::size(data)});
        return this->do_decompress(bytes);
    }

    inline Compressor::CompData Compressor::do_decompress(const std::span<const std::byte>)
    {
        return {}; // Not implemented
    }
}
