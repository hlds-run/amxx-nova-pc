#include "amx/reader.hpp"
#include "amx/consts.hpp"
#include "amx/exceptions/reader_error.hpp"
#include "io/exceptions/stream_error.hpp"
#include <expected>
#include <utility>
#include <vector>

namespace Amx {
    Reader::Reader(std::filesystem::path filepath) : file_path_(std::move(filepath))
    {
    }

    void Reader::open()
    {
        if (is_open()) {
            return;
        }

        stream_.open(file_path(), std::ios::in | std::ios::binary);
        is_closed_ = !stream_.is_open();

        if (!is_open()) {
            throw Io::Exceptions::FileOpenError{file_path().string(), "reading"};
        }
    }

    Header Reader::read_header()
    {
        constexpr auto offset = 0u;
        return read_header(offset);
    }

    Header Reader::read_header(const std::streamoff offset, const std::ios::seekdir dir)
    {
        if (!bin_reader_.seek(offset, dir)) {
            throw Io::Exceptions::SeekError{offset, dir, "AMX file header"};
        }

        auto result = bin_reader_.read<Header>();

        if (result.has_value()) {
            if (result.value().magic != file_magic) {
                throw Exceptions::InvalidMagicError(file_magic, result.value().magic, "AMX file header");
            }
        }
        else {
            throw_stream_exception(result.error(), "reading AMX file header");
        }

        return result.value();
    }

    std::optional<HeaderDebug> Reader::read_header_debug()
    {
        const auto header = read_header();

        if ((header.flags & flag_debug) == 0) {
            return std::nullopt;
        }

        return read_header_debug(header.size);
    }

    HeaderDebug Reader::read_header_debug(const std::streamoff offset, const std::ios::seekdir dir)
    {
        if (!bin_reader_.seek(offset, dir)) {
            throw Io::Exceptions::SeekError{offset, dir, "AMX file debug header"};
        }

        const auto result = bin_reader_.read<HeaderDebug>();

        if (result.has_value()) {
            if (result.value().magic != file_magic_debug) {
                throw Exceptions::InvalidMagicError(file_magic, result.value().magic, "AMX file debug header");
            }
        }
        else {
            throw_stream_exception(result.error(), "reading AMX file debug header");
        }

        return result.value();
    }

    Data Reader::read_data()
    {
        return {.header = read_header(), .header_debug = read_header_debug(), .body = read_all<decltype(Data::body)>()};
    }

    void Reader::close()
    {
        if (is_closed_ || !stream_.is_open()) {
            return;
        }

        stream_.close();
        is_closed_ = true;

        if (stream_.fail()) {
            throw Io::Exceptions::FileCloseError{file_path().string()};
        }
    }

    void Reader::throw_stream_exception(const Io::BinaryReader::Error error, const std::string& context)
    {
        if (error == Io::BinaryReader::Error::end_of_file) {
            throw Io::Exceptions::EndOfFileError{context};
        }

        throw Io::Exceptions::StreamFailureError{context};
    }
}
