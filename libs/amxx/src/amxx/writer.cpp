#include "amxx/writer.hpp"
#include "amxx/data.hpp"
#include <expected>
#include <utility>

namespace Amxx {
    Writer::Writer(std::filesystem::path filepath) : file_path_(std::move(filepath))
    {
    }

    void Writer::open()
    {
        if (is_open()) {
            return;
        }

        stream_.open(file_path(), std::ios::out | std::ios::trunc | std::ios::binary);
        is_closed_ = !stream_.is_open();

        if (!is_open()) {
            throw Core::Io::Exceptions::FileOpenError{file_path().string(), "writing"};
        }
    }

    void Writer::write_header(const Header& header) const
    {
        write_header(header, 0u);
    }

    void Writer::write_header(const Header& header, const std::streamoff offset, const std::ios::seekdir dir) const
    {
        if (!bin_writer_.seek(offset, dir)) {
            throw Core::Io::Exceptions::SeekError{offset, dir, "AMXX file header"};
        }

        if (const auto result = bin_writer_.write(header); !result) {
            throw_stream_exception(result.error(), "writing AMXX file header");
        }
    }

    void Writer::write_entry(const Entry& entry) const
    {
        constexpr auto offset = sizeof(Header);
        write_entry(entry, offset);
    }

    void Writer::write_entry(const Entry& entry, const std::streamoff offset, const std::ios::seekdir dir) const
    {
        if (!bin_writer_.seek(offset, dir)) {
            throw Core::Io::Exceptions::SeekError{offset, dir, "AMXX file entry"};
        }

        if (const auto result = bin_writer_.write(entry); !result) {
            throw_stream_exception(result.error(), "writing AMXX file entry");
        }
    }

    void Writer::write_data(const Data& data) const
    {
        write_header(data.header);
        write_entries(data.entries);
        write_bodies(data.bodies);
    }

    void Writer::close()
    {
        if (is_closed_ || !stream_.is_open()) {
            return;
        }

        stream_.close();
        is_closed_ = true;

        if (stream_.fail()) {
            throw Core::Io::Exceptions::FileCloseError{file_path().string()};
        }
    }

    void Writer::throw_stream_exception(
        [[maybe_unused]] Core::Io::BinaryWriter::Error error, const std::string& context)
    {
        throw Core::Io::Exceptions::StreamFailureError{context};
    }
}
