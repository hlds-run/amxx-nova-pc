#include "application.hpp"
#include "core/compress/zlib_compressor.hpp"
#include "interface/cmdline/arguments.hpp"
#include "interface/console/app_version.hpp"
#include "interface/console/console_user_interface.hpp"
#include "interface/console/user_interface.hpp"
#include "pawn/image/amxx/builder.hpp"
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
     * Creates and returns a shared pointer to an instance of \c Interface::Console::ConsoleUserInterface,
     * which provides textual interaction with the user through standard streams.
     *
     * @return Shared pointer to a newly created \c Interface::Console::ConsoleUserInterface.
     *
     * @see Interface::Console::UserInterface
     */
    [[nodiscard]] std::shared_ptr<Interface::Console::UserInterface> make_ui()
    {
        return std::make_shared<Interface::Console::ConsoleUserInterface>();
    }

    /**
     * @brief Constructs an AMXX builder with a default compression strategy.
     *
     * Creates an \c Pawn::Image::Amxx::Builder, configures it with a Zlib compressor, and returns ownership.
     *
     * @return Unique pointer to a configured \c Pawn::Image::Amxx::Builder.
     *
     * @see Pawn::Image::Amxx::Builder
     */
    [[nodiscard]] std::unique_ptr<Pawn::Image::Amxx::Builder> make_amxx_builder()
    {
        auto amxx_builder = std::make_unique<Pawn::Image::Amxx::Builder>();
        amxx_builder->set_compressor(std::make_unique<Core::Compress::ZlibCompressor>());

        return amxx_builder;
    }

    /**
     * @brief Parses command-line arguments into a structured representation.
     *
     * Instantiates a \c Interface::Cmdline::Arguments object with the given argc/argv parameters,
     * enabling option lookup and retrieval of input files.
     *
     * @param argc Number of command-line arguments.
     * @param argv Array of argument strings.
     *
     * @return Unique pointer to a newly created Interface::Cmdline::Arguments instance.
     *
     * @throw \c std::runtime_error If parsing fails.
     *
     * @see Interface::Cmdline::Arguments
     */
    [[nodiscard]] std::unique_ptr<Interface::Cmdline::Arguments> make_arguments(
        const int argc, const char* const argv[])
    {
        return std::make_unique<Interface::Cmdline::Arguments>(argc, argv);
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
    const Interface::Console::AppVersion app_version{.major = AMXXPC_BUILD_MAJOR,
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
