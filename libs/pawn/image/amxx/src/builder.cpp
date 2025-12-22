#include "pawn/image/amxx/builder.hpp"
#include "pawn/image/amx/header.hpp"
#include "pawn/image/amx/header_debug.hpp"
#include "pawn/image/amxx/consts.hpp"
#include "pawn/image/amxx/entry.hpp"
#include "pawn/image/amxx/header.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace {
    /**
     * @brief Size of a Pawn cell in bytes.
     *
     * Determined at compile time according to the \c PAWN_CELL_SIZE macro.
     */
    constexpr auto pawn_cell_size_bytes = +[]() constexpr {
#if PAWN_CELL_SIZE == 16
        return static_cast<std::uint8_t>(sizeof(std::int16_t));
#elif PAWN_CELL_SIZE == 32
        return static_cast<std::uint8_t>(sizeof(std::int32_t));
#elif PAWN_CELL_SIZE == 64
        return static_cast<std::uint8_t>(sizeof(std::int64_t));
#else
        static_assert(false, "Unsupported pawn cell size.");
#endif
    }();

    /**
     * @brief Constructs a default AMXX header.
     *
     * @param entry_count  Number of entries in the file. Defaults to 1.
     *
     * @return Initialized AMXX header.
     *
     * @see Pawn::Image::Amxx::Header
     */
    [[nodiscard]] Pawn::Image::Amxx::Header make_amxx_header(const std::uint8_t entry_count = 1) noexcept
    {
        return Pawn::Image::Amxx::Header{.magic = Pawn::Image::Amxx::file_magic,
            .file_version = Pawn::Image::Amxx::file_version,
            .entry_count = entry_count};
    }

    /**
     * @brief Constructs a single AMXX entry.
     *
     * Creates an entry describing AMX image
     * and memory layout, compressed size, and file offset.
     *
     * @param amx       Source AMX data.
     * @param disk_size Size of the compressed body on disk.
     * @param offset    File offset of the body. Defaults to after header + entry.
     *
     * @return Initialized AMXX entry.
     *
     * @see Pawn::Image::Amxx::Entry
     */
    [[nodiscard]] Pawn::Image::Amxx::Entry make_amxx_entry(
        const Pawn::Image::Amx::Data& amx, const std::uint32_t disk_size, std::uint32_t offset = 0)
    {
        auto image_size = amx.header.size;
        const auto memory_size = amx.header.stp;

        if (amx.header_debug.has_value()) {
            image_size += amx.header_debug->size;
        }

        if (offset == 0) {
            offset = static_cast<std::uint32_t>(sizeof(Pawn::Image::Amxx::Header) + sizeof(Pawn::Image::Amxx::Entry));
        }

        return Pawn::Image::Amxx::Entry{.cell_size = pawn_cell_size_bytes,
            .disk_size = disk_size,
            .image_size = image_size,
            .memory_size = memory_size,
            .offset = offset};
    }
}

namespace Pawn::Image::Amxx {
    Builder& Builder::set_input(std::unique_ptr<Amx::Reader> amx_reader) noexcept
    {
        amx_reader_ = std::move(amx_reader);
        return *this;
    }

    Builder& Builder::set_output(std::unique_ptr<Writer> amxx_writer) noexcept
    {
        amxx_writer_ = std::move(amxx_writer);
        return *this;
    }

    Builder& Builder::set_compressor(std::unique_ptr<Core::Compress::Compressor> compressor) noexcept
    {
        compressor_ = std::move(compressor);
        return *this;
    }

    void Builder::build() const
    {
        const auto amx = read_amx();

        const auto amxx_header = make_amxx_header();
        const auto amxx_body = compressor_->compress(amx.body);
        const auto amxx_entry = make_amxx_entry(amx, amxx_body.size());

        write_amxx(amxx_header, amxx_entry, amxx_body);
    }

    Amx::Data Builder::read_amx() const
    {
        amx_reader_->open();
        const auto data = amx_reader_->read_data();
        amx_reader_->close();

        return data;
    }

    void Builder::write_amxx(const Header& header, const Entry& entry, const std::vector<std::byte>& body) const
    {
        amxx_writer_->open();
        amxx_writer_->write_header(header);
        amxx_writer_->write_entry(entry);
        amxx_writer_->write_body(body);
        amxx_writer_->close();
    }

    void Builder::write_amxx(const Data& data) const
    {
        amxx_writer_->open();
        amxx_writer_->write_data(data);
        amxx_writer_->close();
    }
}
