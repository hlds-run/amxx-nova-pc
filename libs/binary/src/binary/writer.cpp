#include "binary/writer.hpp"

namespace Binary {
    Writer::Writer(std::ostream& stream) : stream_(stream)
    {
    }

    bool Writer::seek(const std::streamoff offset, const std::ios::seekdir dir) const
    {
        stream_.clear();
        stream_.seekp(offset, dir);

        return static_cast<bool>(stream_);
    }

    std::expected<void, Writer::Error> Writer::write_bytes(const std::span<const std::byte> bytes) const
    {
        stream_.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());

        if (stream_.fail()) {
            return std::unexpected(Error::stream_failure);
        }

        return {};
    }
}
