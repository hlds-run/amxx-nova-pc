#pragma once

#include "amx/data.hpp"
#include "amx/reader.hpp"
#include "amxx/writer.hpp"
#include "compress/compressor.hpp"
#include <concepts>
#include <memory>

namespace Amxx {
    template <typename T>
    concept CompressorDerived = std::derived_from<T, Compress::Compressor>;

    /**
     * @brief Constructs AMXX files from AMX input files.
     *
     * Encapsulates the workflow of reading AMX bytecode, compressing it,
     * and writing it into an AMXX container file.
     *
     * @note All resources must be configured before calling \c build.
     */
    class Builder {
      public:
        /**
         * @brief Sets the AMX input reader.
         *
         * Transfers ownership of the reader to the builder.
         *
         * @param amx_reader Unique pointer to an AMX reader instance.
         *
         * @return Reference to the \c Builder.
         */
        Builder& set_input(std::unique_ptr<Amx::Reader> amx_reader) noexcept;

        /**
         * @brief Sets the AMXX output writer.
         *
         * Transfers ownership of the writer to the builder.
         *
         * @param amxx_writer Unique pointer to an AMXX writer instance.
         *
         * @return Reference to the \c Builder.
         */
        Builder& set_output(std::unique_ptr<Writer> amxx_writer) noexcept;

        /**
         * @brief Sets the compression strategy.
         *
         * Transfers ownership of the compressor to the builder.
         *
         * @param compressor Unique pointer to a compressor implementation.
         *
         * @return Reference to the \c Builder.
         */
        Builder& set_compressor(std::unique_ptr<Compress::Compressor> compressor) noexcept;

        /**
         * @brief Executes the build process.
         *
         * Reads AMX input, compresses the bytecode body, constructs an AMXX header
         * and entry, and writes all components into the output file.
         *
         * @note Requires \c set_input, \c set_output,
         *       and \c set_compressor to be called prior to invocation.
         */
        void build() const;

      private:
        /// Compression strategy instance.
        std::unique_ptr<Compress::Compressor> compressor_;

        /// AMX bytecode reader instance.
        std::unique_ptr<Amx::Reader> amx_reader_{};

        /// AMXX file writer instance.
        std::unique_ptr<Writer> amxx_writer_{};

        /**
         * @brief Reads AMX data from the configured input reader.
         *
         * @return Parsed AMX data.
         *
         * @see Amx::Data
         */
        [[nodiscard]] Amx::Data read_amx() const;

        /**
         * @brief Writes AMXX components into the output writer.
         *
         * Writes header, single entry, and compressed body sequentially.
         *
         * @param header AMXX header structure.
         * @param entry  AMXX entry structure.
         * @param body   Compressed byte sequence representing the AMXX program body.
         */
        void write_amxx(const Header& header, const Entry& entry, const Compress::Compressor::CompData& body) const;

        /**
         * @brief Writes a complete AMXX data structure.
         *
         * Opens the output writer, writes header, entries and bodies.
         *
         * @param data Composite AMXX data object.
         *
         * @see Amxx::Data
         */
        void write_amxx(const Data& data) const;
    };
}
