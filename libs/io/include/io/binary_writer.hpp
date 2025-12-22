#pragma once

#include "concepts/concepts.hpp"
#include <cstddef>
#include <cstdint>
#include <expected>
#include <ostream>
#include <ranges>
#include <span>

namespace Io {
    /**
     * @brief Provides binary data writing capabilities to an output stream.
     *
     * Encapsulates low-level write operations to provide type safety and error reporting.
     */
    class BinaryWriter {
      public:
        /**
         * @brief Represents error conditions during writing operations.
         */
        enum class Error : std::uint8_t {
            /// Output stream encountered a failure state
            stream_failure
        };

        /**
         * @brief Constructs a writer bound to an existing output stream.
         *
         * @param stream Output stream that must remain valid
         *               for the entire lifetime of the writer.
         */
        explicit BinaryWriter(std::ostream& stream);

        /// Deleted copy constructor.
        BinaryWriter(const BinaryWriter&) = delete;

        /// Deleted move constructor.
        BinaryWriter(BinaryWriter&&) noexcept = delete;

        /// Default destructor.
        ~BinaryWriter() = default;

        /// Deleted copy assignment operator.
        BinaryWriter& operator=(const BinaryWriter&) = delete;

        /// Deleted move assignment operator.
        BinaryWriter& operator=(BinaryWriter&&) noexcept = delete;

        /**
         * @brief Moves the write position within the stream.
         *
         * @param offset Byte offset relative to \p dir.
         * @param dir    Reference point for the seek operation.
         *               Defaults to beginning of stream.
         *
         * @return \c true if the seek succeeded, otherwise \c false.
         */
        [[nodiscard]] bool seek(std::streamoff offset, std::ios::seekdir dir = std::ios::beg) const;

        /**
         * @brief Obtains the current position within the stream.
         *
         * @return Current read position as a stream offset.
         *
         * @note Equivalent to \c tellp on the underlying stream.
         */
        [[nodiscard]] std::streampos position() const;

        /**
         * @brief Checks whether the underlying stream is in a good state.
         *
         * @return \c true if no errors have occurred and stream is usable, otherwise \c false.
         */
        [[nodiscard]] bool good() const;

        /**
         * @brief Writes a trivially byte-safe object to the stream.
         *
         * @tparam T Type satisfying \c Concepts::ByteSafe.
         *
         * @param value The object to be written.
         *
         * @return Empty on success, or an error code on failure.
         */
        template <Concepts::ByteSafe T>
        std::expected<void, Error> write(const T& value) const;

        /**
         * @brief Writes the contents of a container to the stream.
         *
         * @tparam T Type satisfying \c Concepts::ContiguousByteSafeContainer.
         *
         * @param data Container whose elements will be written in sequence.
         *
         * @return Empty on success, or an error code on failure.
         */
        template <Concepts::ContiguousByteSafeContainer T>
        std::expected<void, Error> write_from(const T& data) const;

        /**
         * @brief Writes raw bytes directly to the stream.
         *
         * @param bytes Sequence of bytes to be written.
         *
         * @return Empty on success, or an error code on failure.
         */
        std::expected<void, Error> write_bytes(std::span<const std::byte> bytes) const;

      private:
        /// The underlying output stream.
        std::ostream& stream_;
    };

    inline std::streampos BinaryWriter::position() const
    {
        return stream_.tellp();
    }

    inline bool BinaryWriter::good() const
    {
        return stream_.good();
    }

    template <Concepts::ByteSafe T>
    std::expected<void, BinaryWriter::Error> BinaryWriter::write(const T& value) const
    {
        auto bytes = std::as_bytes(std::span{&value, 1});
        return write_bytes(bytes);
    }

    template <Concepts::ContiguousByteSafeContainer T>
    std::expected<void, BinaryWriter::Error> BinaryWriter::write_from(const T& data) const
    {
        auto bytes = std::as_bytes(std::span{std::ranges::data(data), std::ranges::size(data)});
        return write_bytes(bytes);
    }
}
