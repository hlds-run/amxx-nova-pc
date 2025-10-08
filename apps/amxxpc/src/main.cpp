#include "amxx/builder.hpp"
#include "application.hpp"
#include "cli/arguments.hpp"
#include "compress/zlib_compressor.hpp"
#include "ui/app_version.hpp"
#include "ui/console_user_interface.hpp"
#include "ui/user_interface.hpp"
#include "version.hpp"
#include <cstdlib>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

namespace {
    /**
     * @brief Constructs a console-based user interface.
     *
     * Creates and returns a shared pointer to an instance of \c Ui::ConsoleUserInterface,
     * which provides textual interaction with the user through standard streams.
     *
     * @return Shared pointer to a newly created \c Ui::ConsoleUserInterface.
     *
     * @see Ui::UserInterface
     */
    [[nodiscard]] std::shared_ptr<Ui::UserInterface> make_ui()
    {
        return std::make_shared<Ui::ConsoleUserInterface>();
    }

    /**
     * @brief Constructs an AMXX builder with a default compression strategy.
     *
     * Creates an \c Amxx::Builder, configures it with a Zlib compressor, and returns ownership.
     *
     * @return Unique pointer to a configured \c Amxx::Builder.
     *
     * @see Amxx::Builder
     */
    [[nodiscard]] std::unique_ptr<Amxx::Builder> make_amxx_builder()
    {
        auto amxx_builder = std::make_unique<Amxx::Builder>();
        amxx_builder->set_compressor(std::make_unique<Compress::ZlibCompressor>());

        return amxx_builder;
    }

    /**
     * @brief Parses command-line arguments into a structured representation.
     *
     * Instantiates a \c Cli::Arguments object with the given argc/argv parameters,
     * enabling option lookup and retrieval of input files.
     *
     * @param argc Number of command-line arguments.
     * @param argv Array of argument strings.
     *
     * @return Unique pointer to a newly created Cli::Arguments instance.
     *
     * @throw \c std::runtime_error If parsing fails.
     *
     * @see Cli::Arguments
     */
    [[nodiscard]] std::unique_ptr<Cli::Arguments> make_arguments(const int argc, const char* const argv[])
    {
        return std::make_unique<Cli::Arguments>(argc, argv);
    }
}

/**
 * @brief Entry point of the application.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 *
 * @return \c EXIT_SUCCESS if the program completes normally,
 *         \c EXIT_FAILURE if an error occurs.
 */
int main(const int argc, const char* const argv[])
{
    const Ui::AppVersion app_version{.major = AMXXPC_BUILD_MAJOR,
        .minor = AMXXPC_BUILD_MINOR,
        .patch = AMXXPC_BUILD_PATCH,
        .year = CURRENT_YEAR,
        .string = AMXXPC_VERSION};

    const auto ui = make_ui();
    ui->show_header(app_version);

    try {
        AmxxPc::Application application{};
        application.set_user_interface(ui);
        application.set_arguments(make_arguments(argc, argv));
        application.set_amxx_builder(make_amxx_builder());

        return application.run();
    }
    catch (const std::runtime_error& ex) {
        ui->show_error(ex.what());
    }
    catch (const std::exception& ex) {
        ui->show_error(std::string{"Fatal error:\n"} + ex.what());
    }

    return EXIT_FAILURE;
}
