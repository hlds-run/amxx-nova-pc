#include "application.hpp"
#include "amx/reader.hpp"
#include "amxx/writer.hpp"
#include "pawnwrap/compiler.hpp"
#include "ui/user_interface.hpp"
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
     * @brief Builds an AMXX file from a given SMA input file.
     *
     * Converts a source `.sma` file to `.amx`,
     * then to `.amxx`, using the provided builder.
     * Intermediate `.amx` file is removed after packaging.
     *
     * @param amxx_builder Initialized \c Amxx::Builder.
     * @param sma_file     Path to the input `.sma` source file.
     *
     * @throw \c std::runtime_error On errors during building or file handling.
     */
    void build_amxx_file(Amxx::Builder& amxx_builder, const std::filesystem::path& sma_file)
    {
        auto amx_file = sma_file;
        amx_file.replace_extension(".amx");

        auto amxx_file = amx_file;
        amxx_file.replace_extension(".amxx");

        amxx_builder.set_input(std::make_unique<Amx::Reader>(amx_file));
        amxx_builder.set_output(std::make_unique<Amxx::Writer>(amxx_file));
        amxx_builder.build();

        remove_file(amx_file);
    }
}

namespace AmxxPc {
    void Application::set_arguments(std::unique_ptr<Cli::Arguments> arguments)
    {
        arguments_ = std::move(arguments);
    }

    void Application::set_user_interface(std::shared_ptr<Ui::UserInterface> ui)
    {
        ui_ = std::move(ui);
    }

    void Application::set_amxx_builder(std::unique_ptr<Amxx::Builder> amxx_builder)
    {
        amxx_builder_ = std::move(amxx_builder);
    }

    int Application::run(const Ui::AppVersion& app_version) const
    {
        ui_->show_header(app_version);

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
                build_amxx_file(*amxx_builder_, input_file);
            }
        }

        return EXIT_SUCCESS;
    }
}
