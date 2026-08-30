#include "application.hpp"
#include "interface/console/user_interface.hpp"
#include "pawn/compiler/legacy/adapter/compiler.hpp"
#include "pawn/image/amx/reader.hpp"
#include "pawn/image/amxx/writer.hpp"
#include <cstdlib>
#include <filesystem>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

namespace {
    /**
     * @brief Attempts to remove a file from the filesystem.
     *
     * Provides error handling by throwing on failure instead of silently ignoring errors.
     *
     * @param file_path Path to the file to be removed.
     *
     * @throw \c std::system_error If removal fails with a filesystem error.
     *
     * @note Safe to call on non-existing files; exception thrown only if an error occurs.
     */
    void remove_file(const std::filesystem::path& file_path)
    {
        if (std::error_code ec{}; !std::filesystem::remove(file_path, ec) && ec) {
            throw std::system_error(ec, "Failed to remove file: " + file_path.string());
        }
    }

    /**
     * @brief Resolves the expected `.amx` output path for a given `.sma` input file.
     *
     * Determines the final output directory and base filename produced by the Pawn compiler.
     *
     * @param arguments Parsed command-line arguments wrapper.
     * @param sma_file  Path to the original `.sma` source file.
     *
     * @return Full filesystem path to the expected `.amx` file.
     */
    std::filesystem::path resolve_amx_path(
        const Interface::Cmdline::Arguments& arguments, const std::filesystem::path& sma_file)
    {
        auto output_dir = std::filesystem::current_path();
        auto output_name = sma_file.filename();

        if (arguments.has_option("D")) {
            if (const auto dir = arguments.get_last_option_value("D"); dir) {
                output_dir = std::filesystem::path(*dir);
            }
        }

        if (arguments.has_option("o")) {
            if (const auto name = arguments.get_last_option_value("o"); name) {
                output_name = *name;
            }
        }

        return output_dir / output_name.replace_extension(".amx");
    }

    /**
     * @brief Builds an AMXX file from a given SMA input file.
     *
     * Converts a source `.sma` file to `.amx`,
     * then to `.amxx`, using the provided builder.
     * Intermediate `.amx` file is removed after packaging.
     *
     * @param amxx_builder Initialized \c Pawn::Image::Amxx::Builder.
     * @param amx_file     Path to the already compiled `.amx` bytecode file.
     *
     * @throw \c std::runtime_error On errors during building or file handling.
     */
    void build_amxx_file(Pawn::Image::Amxx::Builder& amxx_builder, const std::filesystem::path& amx_file)
    {
        auto amxx_file = amx_file;
        amxx_file.replace_extension(".amxx");

        amxx_builder.set_input(std::make_unique<Pawn::Image::Amx::Reader>(amx_file));
        amxx_builder.set_output(std::make_unique<Pawn::Image::Amxx::Writer>(amxx_file));
        amxx_builder.build();

        remove_file(amx_file);
    }
}

namespace AmxxPc {
    void Application::set_arguments(std::unique_ptr<Interface::Cmdline::Arguments> arguments)
    {
        arguments_ = std::move(arguments);
    }

    void Application::set_user_interface(std::shared_ptr<Interface::Console::UserInterface> ui)
    {
        ui_ = std::move(ui);
    }

    void Application::set_amxx_builder(std::unique_ptr<Pawn::Image::Amxx::Builder> amxx_builder)
    {
        amxx_builder_ = std::move(amxx_builder);
    }

    int Application::run() const
    {
        if (arguments_->has_option("help") || arguments_->has_option("?")) {
            ui_->show_usage();
            ui_->show_help(PawnWrap::get_options());

            return EXIT_SUCCESS;
        }

        if (arguments_->get_input_files().empty()) {
            ui_->show_usage();
            return EXIT_FAILURE;
        }

        const auto& input_files = arguments_->get_input_files();
        const auto& arguments = arguments_->get_arguments();

        if (!PawnWrap::compile(input_files, arguments)) {
            return EXIT_FAILURE;
        }

        if (!arguments_->has_option("a") && !arguments_->has_option("l")) {
            for (const auto& input_file : input_files) {
                const auto amx_file = resolve_amx_path(*arguments_, input_file);
                build_amxx_file(*amxx_builder_, amx_file);
            }
        }

        return EXIT_SUCCESS;
    }
}
