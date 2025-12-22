#pragma once

#include "core/concepts/concepts.hpp"
#include <cstddef>
#include <cstdint>
#include <expected>
#include <istream>
#include <ranges>
#include <span>

namespace Core::Io {
    /**
     * @brief Provides binary stream reading utilities.
     *
     * Wraps an input stream to support reading arbitrary
     * byte-oriented values, containers, and buffers.
     */
    class BinaryReader {
      public:
        /**
         * @brief Represents possible error states during reading.
         */
        enum class Error : std::uint8_t {
            /// Underlying stream failure (e.g., I/O error).
            stream_failure,

            /// Unexpected end of file before completing read.
            end_of_file
        };

        /**
         * @brief Constructs a binary reader from an input stream.
         *
         * @param stream Input stream that must remain valid
         *               for the entire lifetime of the reader.
         */
        explicit BinaryReader(std::istream& stream);

        /// Deleted copy constructor.
        BinaryReader(const BinaryReader&) = delete;

        /// Deleted move constructor.
        BinaryReader(BinaryReader&&) noexcept = delete;

        /// Default destructor.
        ~BinaryReader() = default;

        /// Deleted copy assignment operator.
        BinaryReader& operator=(const BinaryReader&) = delete;

        /// Deleted move assignment operator.
        BinaryReader& operator=(BinaryReader&&) noexcept = delete;

        /**
         * @brief Returns the total size of the stream in bytes.
         *
         * @return Stream size in bytes.
         */
        [[nodiscard]] std::size_t size() const;

        /**
         * @brief Moves the read position within the stream.
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
         * @note Equivalent to \c tellg on the underlying stream.
         */
        [[nodiscard]] std::streampos position() const;

        /**
         * @brief Calculates the number of unread bytes remaining.
         *
         * @return Count of bytes available from the current position until end-of-file.
         */
        [[nodiscard]] std::size_t remaining() const;

        /**
         * @brief Checks whether the underlying stream is in a good state.
         *
         * @return \c true if no errors have occurred and stream is usable, otherwise \c false.
         */
        [[nodiscard]] bool good() const;

        /**
         * @brief Determines whether the stream has reached end-of-file.
         *
         * @return \c true if end-of-file was reached, otherwise \c false.
         */
        [[nodiscard]] bool eof() const;

        /**
         * @brief Reads a single value of trivially copyable type.
         *
         * @tparam T Type constrained by \c Core::Concepts::ByteSafe.
         *
         * @return The read value on success, or an error code on failure.
         */
        template <Concepts::ByteSafe T>
        [[nodiscard]] std::expected<T, Error> read();

        /**
         * @brief Reads data into an existing container.
         *
         * @tparam T Container type constrained by
         *         \c Core::Concepts::ContiguousByteSafeContainer.
         *
         * @param buffer Reference to container with sufficient capacity.
         *
         * @return Empty on success, or error code on failure.
         */
        template <Concepts::ContiguousByteSafeContainer T>
        std::expected<void, Error> read_into(T& buffer);

        /**
         * @brief Reads all remaining bytes into container.
         *
         * @tparam T Container type constrained by
         *         \c Core::Concepts::ResizableContiguousByteSafeContainer.
         *
         * @return Container filled with remaining bytes, or error code.
         *
         * @note Container is resized automatically as needed.
         */
        template <Concepts::ResizableContiguousByteSafeContainer T>
        [[nodiscard]] std::expected<T, Error> read_remaining();

        /**
         * @brief Reads the entire contents of the stream into container.
         *
         * @tparam T Container type constrained by
         *         \c Core::Concepts::ResizableContiguousByteSafeContainer.
         *
         * @return Container filled with all stream bytes, or error code.
         *
         * @throw Nothing
         *
         * @note Restores original read position after completion.
         */
        template <Concepts::ResizableContiguousByteSafeContainer T>
        [[nodiscard]] std::expected<T, Error> read_all();

        /**
         * @brief Reads raw bytes into a caller-provided buffer.
         *
         * @param buffer Span referencing writable memory.
         *
         * @return Empty on success, or error code on failure.
         */
        std::expected<void, Error> read_bytes(std::span<std::byte> buffer) const;

      private:
        /// The underlying input stream.
        std::istream& stream_;

        /**
         * @brief Creates a resizable container of given size.
         *
         * @tparam T Container type constrained by
         *         \c Core::Concepts::ResizableContiguousByteSafeContainer.
         *
         * @param size Desired number of elements.
         *
         * @return Container instance of specified size.
         */
        template <Concepts::ResizableContiguousByteSafeContainer T>
        [[nodiscard]] T make_bytes_container(std::size_t size);
    };

    inline std::streampos BinaryReader::position() const
    {
        return stream_.tellg();
    }

    inline bool BinaryReader::good() const
    {
        return stream_.good();
    }

    inline bool BinaryReader::eof() const
    {
        return stream_.eof();
    }

    template <Concepts::ByteSafe T>
    std::expected<T, BinaryReader::Error> BinaryReader::read()
    {
        T value{};
        const auto bytes = std::as_writable_bytes(std::span{&value, 1});

        if (auto result = read_bytes(bytes); !result) {
            return std::unexpected{result.error()};
        }

        return value;
    }

    template <Concepts::ContiguousByteSafeContainer T>
    std::expected<void, BinaryReader::Error> BinaryReader::read_into(T& buffer)
    {
        const auto bytes = std::as_writable_bytes(std::span{std::ranges::data(buffer), std::ranges::size(buffer)});
        return read_bytes(bytes);
    }

    template <Concepts::ResizableContiguousByteSafeContainer T>
    std::expected<T, BinaryReader::Error> BinaryReader::read_remaining()
    {
        const auto bytes_left = remaining();

        if (bytes_left == 0) {
            return T{};
        }

        using ElementType = std::ranges::range_value_t<T>;
        constexpr auto element_size = sizeof(ElementType);

        const auto element_count = (bytes_left + element_size - 1) / element_size;
        auto buffer = make_bytes_container<T>(element_count);

        if (auto result = read_into(buffer); !result) {
            return std::unexpected(result.error());
        }

        return buffer;
    }

    template <Concepts::ResizableContiguousByteSafeContainer T>
    std::expected<T, BinaryReader::Error> BinaryReader::read_all()
    {
        if (const auto cur_pos = position(); seek(0)) {
            if (auto result = read_remaining<T>(); seek(cur_pos) && result.has_value()) {
                return result;
            }
        }

        return std::unexpected(Error::stream_failure);
    }

    template <Concepts::ResizableContiguousByteSafeContainer T>
    T BinaryReader::make_bytes_container(const std::size_t size)
    {
        if constexpr (requires { T{size}; }) {
            return T{size};
        }
        else {
            T buffer{};
            buffer.resize(size);

            return buffer;
        }
    }
}
