#include "binary/reader.hpp"

namespace Binary {
    Reader::Reader(std::istream& stream) : stream_(stream)
    {
    }

    std::size_t Reader::size() const
    {
        const auto cur_pos = position();
        stream_.seekg(0, std::ios::end);
        const auto end_pos = position();
        stream_.seekg(cur_pos);

        return static_cast<std::size_t>(end_pos);
    }

    bool Reader::seek(const std::streamoff offset, const std::ios::seekdir dir) const
    {
        stream_.clear();
        stream_.seekg(offset, dir);

        return static_cast<bool>(stream_);
    }

    std::size_t Reader::remaining() const
    {
        const auto pos = position();
        const auto total = size();

        return static_cast<std::size_t>(total - pos);
    }

    std::expected<void, Reader::Error> Reader::read_bytes(std::span<std::byte> buffer) const
    {
        stream_.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

        if (static_cast<std::size_t>(stream_.gcount()) != buffer.size()) {
            if (stream_.fail()) {
                return std::unexpected(Error::stream_failure);
            }

            return std::unexpected(Error::end_of_file);
        }

        return {};
    }
}
