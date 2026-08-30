#pragma once

#include "core/concepts/concepts.hpp"
#include "core/io/binary_reader.hpp"
#include "pawn/image/amx/data.hpp"
#include "pawn/image/amx/header.hpp"
#include "pawn/image/amx/header_debug.hpp"
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>

namespace Pawn::Image::Amx {
    /**
     * @brief Provides functionality to read and validate AMX files.
     *
     * Encapsulates header parsing and data extraction from AMX binaries.
     */
    class Reader {
      public:
        /**
         * @brief Constructs a reader for a specified AMX file.
         *
         * @param filepath Path to the AMX file to be associated with this reader.
         *
         * @note The file is not opened immediately; call open to access the stream.
         */
        explicit Reader(std::filesystem::path filepath);

        /**
         * @brief Opens the underlying file stream in binary read mode.
         *
         * @throw \c Core::Io::Exceptions::FileOpenError If the file cannot be opened.
         */
        void open();

        /**
         * @brief Provides access to the associated file path.
         *
         * @return AMX file path.
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
         * @brief Reads and validates the AMX header at the beginning of the file.
         *
         * @return Parsed \c Pawn::Image::Amx::Header structure.
         */
        [[nodiscard]] Header read_header();

        /**
         * @brief Reads and validates the AMX header from a specific position.
         *
         * @param offset Byte offset relative to \p dir.
         * @param dir    Reference point for the seek operation.
         *               Defaults to beginning of stream.
         *
         * @return Parsed \c Pawn::Image::Amx::Header structure.
         */
        [[nodiscard]] Header read_header(std::streamoff offset, std::ios::seekdir dir = std::ios::beg);

        /**
         * @brief Attempts to read the AMX debug header if present.
         *
         * @return Parsed \c Pawn::Image::Amx::HeaderDebug if debug information exists, otherwise \c std::nullopt.
         */
        [[nodiscard]] std::optional<HeaderDebug> read_header_debug();

        /**
         * @brief Reads and validates the AMX debug header from a specific position.
         *
         * @param offset Byte offset relative to \p dir.
         * @param dir    Reference point for the seek operation.
         *               Defaults to beginning of stream.
         *
         * @return Parsed \c Pawn::Image::Amx::HeaderDebug structure.
         */
        [[nodiscard]] HeaderDebug read_header_debug(std::streamoff offset, std::ios::seekdir dir = std::ios::beg);

        /**
         * @brief Reads the entire contents of the file into a container.
         *
         * @tparam T A container type satisfying \c Core::Concepts::ResizableContiguousByteSafeContainer.
         *
         * @return Fully populated container with file data.
         */
        template <Core::Concepts::ResizableContiguousByteSafeContainer T>
        [[nodiscard]] T read_all();

        /**
         * @brief Reads and assembles the complete AMX data structure.
         *
         * Combines header, optional debug header, and file body into one structure.
         *
         * @return Fully populated \c Pawn::Image::Amx::Data object.
         */
        [[nodiscard]] Data read_data();

        /**
         * @brief Closes the underlying file stream.
         *
         * @throw \c Core::Io::Exceptions::FileCloseError If closing the file fails.
         */
        void close();

      private:
        /// Path to the AMX file being read.
        std::filesystem::path file_path_;

        /// Indicates whether the file stream has been explicitly closed.
        bool is_closed_{true};

        /// Input file stream used for reading.
        std::ifstream stream_{};

        /// Binary reader wrapping the input stream.
        Core::Io::BinaryReader bin_reader_{stream_};

        /**
         * @brief Translates low-level stream errors into domain-specific exceptions.
         *
         * @param error   Error code returned from binary reader.
         * @param context Additional context about the operation that failed.
         */
        [[noreturn]] static void throw_stream_exception(
            Core::Io::BinaryReader::Error error, const std::string& context = "");
    };

    inline const std::filesystem::path& Reader::file_path() const noexcept
    {
        return file_path_;
    }

    inline bool Reader::is_open() const noexcept
    {
        return !is_closed_ && stream_.is_open();
    }

    inline bool Reader::good() const noexcept
    {
        return !is_closed_ && stream_.good();
    }

    template <Core::Concepts::ResizableContiguousByteSafeContainer T>
    T Reader::read_all()
    {
        const auto result = bin_reader_.read_all<T>();

        if (!result.has_value()) {
            throw_stream_exception(result.error(), "failed to read '" + file_path().string() + "' file");
        }

        return result.value();
    }
}
