#include "core/compress/zlib_compressor.hpp"
#include "core/compress/exceptions/zlib_error.hpp"
#include <algorithm>
#include <span>
#include <string>
#include <zconf.h>
#include <zlib.h>

namespace Core::Compress {
    ZlibCompressor::ZlibCompressor() : ZlibCompressor(Z_DEFAULT_COMPRESSION)
    {
    }

    ZlibCompressor::ZlibCompressor(const int level)
    {
        set_compression_level(level);
    }

    void ZlibCompressor::set_compression_level(const int level)
    {
        if (level < Z_NO_COMPRESSION) {
            compression_level_ = Z_DEFAULT_COMPRESSION;
        }
        else {
            compression_level_ = std::clamp(level, Z_NO_COMPRESSION, Z_BEST_COMPRESSION);
        }
    }

    Compressor::CompData ZlibCompressor::do_compress(const std::span<const std::byte> data)
    {
        if (data.empty()) {
            return {};
        }

        const auto source_len = static_cast<uLong>(data.size());
        auto dest_len = compressBound(source_len);
        CompData buffer{dest_len};

        const auto* const source = reinterpret_cast<const Bytef*>(data.data());
        auto* const dest = reinterpret_cast<Bytef*>(buffer.data());
        const auto result = compress2(dest, &dest_len, source, source_len, compression_level_);

        if (result == Z_OK) {
            buffer.resize(static_cast<CompData::size_type>(dest_len));
            return buffer;
        }

        throw Exceptions::ZlibError(result, "compressing data");
    }
}
