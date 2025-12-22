#pragma once

#include "core/concepts/concepts.hpp"
#include "core/io/binary_writer.hpp"
#include "core/io/exceptions/stream_error.hpp"
#include "pawn/image/amxx/entry.hpp"
#include "pawn/image/amxx/header.hpp"
#include <concepts>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <string>

namespace Pawn::Image::Amxx {
    template <typename T>
    concept EntryContainer = std::ranges::forward_range<T> && std::same_as<std::ranges::range_value_t<T>, Entry>;

    template <typename T>
    concept BodyContainer =
        std::ranges::forward_range<T> && Core::Concepts::ContiguousByteSafeContainer<std::ranges::range_value_t<T>>;

    /**
     * @brief Provides functionality for writing AMXX files.
     */
    class Writer {
      public:
        /**
         * @brief Constructs a writer bound to a given file path.
         *
         * @param filepath Destination file path. The file will be created or
         *                 truncated when open is called.
         */
        explicit Writer(std::filesystem::path filepath);

        /**
         * @brief Opens the file stream for binary output.
         *
         * Truncates the target file if it already exists.
         * Subsequent write operations require the stream to be open.
         *
         * @throw \c Core::Io::Exceptions::FileOpenError If the file cannot be opened.
         */
        void open();

        /**
         * @brief Provides access to the associated file path.
         *
         * @return AMXX file path.
         */
        [[nodiscard]] const std::filesystem::path& file_path() const noexcept;

        /**
         * @brief Reports whether the file stream is currently open and not closed.
         *
         * @return \c true if the stream is open and active, \c false otherwise.
         */
        [[nodiscard]] bool is_open() const noexcept;

        /**
         * @brief Checks whether the underlying stream is in a good state.
         *
         * @return \c true if no errors have occurred and stream is usable, otherwise \c false.
         */
        [[nodiscard]] bool good() const noexcept;

        /**
         * @brief Writes an AMXX header at the start of the file.
         *
         * @param header \c Pawn::Image::Amxx::Header structure to write.
         */
        void write_header(const Header& header) const;

        /**
         * @brief Writes an AMXX header at a specified file position.
         *
         * @param header \c Pawn::Image::Amxx::Header structure to write.
         * @param offset Offset relative to \p dir where header should be written.
         * @param dir    Seek direction, defaults to beginning.
         */
        void write_header(const Header& header, std::streamoff offset, std::ios::seekdir dir = std::ios::beg) const;

        /**
         * @brief Writes a single AMXX entry immediately after the header.
         *
         * @param entry \c Pawn::Image::Amxx::Entry structure to write.
         */
        void write_entry(const Entry& entry) const;

        /**
         * @brief Writes a single AMXX entry at a specified file position.
         *
         * @param entry  \c Pawn::Image::Amxx::Entry structure to write.
         * @param offset Offset relative to @p dir where entry should be written.
         * @param dir    Seek direction, defaults to beginning.
         */
        void write_entry(const Entry& entry, std::streamoff offset, std::ios::seekdir dir = std::ios::beg) const;

        /**
         * @brief Writes multiple AMXX entries sequentially after the header.
         *
         * @tparam T Container type satisfying \c Pawn::Image::Amxx::EntryContainer.
         *
         * @param entries Collection of entries to write.
         */
        template <EntryContainer T>
        void write_entries(const T& entries) const;

        /**
         * @brief Writes multiple AMXX entries at a specified position.
         *
         * @tparam T Container type satisfying \c Pawn::Image::Amxx::EntryContainer.
         *
         * @param entries Collection of entries to write.
         * @param offset  Offset relative to \p dir where entries should be written.
         * @param dir     Seek direction, defaults to beginning.
         */
        template <EntryContainer T>
        void write_entries(const T& entries, std::streamoff offset, std::ios::seekdir dir = std::ios::beg) const;

        /**
         * @brief Writes a body immediately after header and first entry.
         *
         * @tparam T Container type satisfying \c Core::Concepts::ContiguousByteSafeContainer.
         *
         * @param body Container holding raw bytes of the body.
         */
        template <Core::Concepts::ContiguousByteSafeContainer T>
        void write_body(const T& body) const;

        /**
         * @brief Writes a body at a specified position.
         *
         * @tparam T Container type satisfying \c Core::Concepts::ContiguousByteSafeContainer.
         *
         * @param body   Container holding raw bytes of the body.
         * @param offset Offset relative to \p dir where body should be written.
         * @param dir    Seek direction, defaults to beginning.
         */
        template <Core::Concepts::ContiguousByteSafeContainer T>
        void write_body(const T& body, std::streamoff offset, std::ios::seekdir dir = std::ios::beg) const;

        /**
         * @brief Writes multiple bodies sequentially after header and entries.
         *
         * @tparam T Container type satisfying \c Pawn::Image::Amxx::BodyContainer.
         *
         * @param bodies Collection of body containers to write.
         */
        template <BodyContainer T>
        void write_bodies(const T& bodies) const;

        /**
         * @brief Writes multiple bodies at a specified position.
         *
         * @tparam T Container type satisfying \c Pawn::Image::Amxx::BodyContainer.
         *
         * @param bodies Collection of body containers to write.
         * @param offset Offset relative to \p dir where bodies should be written.
         * @param dir    Seek direction, defaults to beginning.
         */
        template <BodyContainer T>
        void write_bodies(const T& bodies, std::streamoff offset, std::ios::seekdir dir = std::ios::beg) const;

        /**
         * @brief Writes a complete AMXX data structure (header, entries, bodies).
         *
         * @param data Data object containing header, entries, and bodies.
         */
        void write_data(const struct Data& data) const;

        /**
         * @brief Closes the underlying file stream.
         *
         * @throw \c Core::Io::Exceptions::FileCloseError If closing the file fails.
         */
        void close();

      private:
        /// Path to the target AMXX file.
        std::filesystem::path file_path_;

        /// Indicates whether the file stream has been explicitly closed.
        bool is_closed_{true};

        /// Output file stream used for binary writing.
        std::ofstream stream_{};

        /// Binary writer wrapping the output stream.
        Core::Io::BinaryWriter bin_writer_{stream_};

        /**
         * @brief Translates low-level stream errors into domain-specific exceptions.
         *
         * @param error   Error code returned from binary reader.
         * @param context Additional context about the operation that failed.
         */
        [[noreturn]] static void throw_stream_exception(
            Core::Io::BinaryWriter::Error error, const std::string& context = "");
    };

    inline const std::filesystem::path& Writer::file_path() const noexcept
    {
        return file_path_;
    }

    inline bool Writer::is_open() const noexcept
    {
        return !is_closed_ && stream_.is_open();
    }

    inline bool Writer::good() const noexcept
    {
        return !is_closed_ && stream_.good();
    }

    template <EntryContainer T>
    void Writer::write_entries(const T& entries) const
    {
        constexpr auto offset = sizeof(Header);
        write_entries(entries, offset);
    }

    template <EntryContainer T>
    void Writer::write_entries(const T& entries, const std::streamoff offset, const std::ios::seekdir dir) const
    {
        if (!bin_writer_.seek(offset, dir)) {
            throw Core::Io::Exceptions::SeekError{offset, dir, "AMXX file entries"};
        }

        for (const auto& entry : entries) {
            if (const auto result = bin_writer_.write(entry); !result) {
                throw_stream_exception(result.error(), "writing AMXX file entries");
            }
        }
    }

    template <Core::Concepts::ContiguousByteSafeContainer T>
    void Writer::write_body(const T& body) const
    {
        constexpr auto offset = sizeof(Header) + sizeof(Entry);
        write_body(body, offset);
    }

    template <Core::Concepts::ContiguousByteSafeContainer T>
    void Writer::write_body(const T& body, const std::streamoff offset, const std::ios::seekdir dir) const
    {
        if (!bin_writer_.seek(offset, dir)) {
            throw Core::Io::Exceptions::SeekError{offset, dir, "AMXX file body"};
        }

        if (const auto result = bin_writer_.write_from(body); !result) {
            throw_stream_exception(result.error(), "failed to write '" + file_path().string() + "' file body");
        }
    }

    template <BodyContainer T>
    void Writer::write_bodies(const T& bodies) const
    {
        const auto offset = sizeof(Header) + sizeof(Entry) * bodies.size();
        write_bodies(bodies, offset);
    }

    template <BodyContainer T>
    void Writer::write_bodies(const T& bodies, const std::streamoff offset, const std::ios::seekdir dir) const
    {
        if (!bin_writer_.seek(offset, dir)) {
            throw Core::Io::Exceptions::SeekError{offset, dir, "AMXX file bodies"};
        }

        for (const auto& body : bodies) {
            if (const auto result = bin_writer_.write_from(body); !result) {
                throw_stream_exception(result.error(), "failed to write '" + file_path().string() + "' file bodies");
            }
        }
    }
}
