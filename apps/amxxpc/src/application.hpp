#pragma once

#include "amxx/builder.hpp"
#include "cli/arguments.hpp"
#include <memory>

namespace Ui {
    class UserInterface;
}

namespace Ui {
    struct AppVersion;
}

namespace AmxxPc {
    /**
     * @brief Represents the main application controller.
     *
     * Coordinates command-line argument parsing and AMXX file building.
     * Acts as the central orchestrator of program flow from initialization to execution.
     *
     * @note Application must be configured with arguments, UI, and builder before calling run().
     */
    class Application {
      public:
        /**
         * @brief Assigns parsed command-line arguments to the application.
         *
         * @param arguments Unique pointer to a configured \c Cli::Arguments instance.
         */
        void set_arguments(std::unique_ptr<Cli::Arguments> arguments);

        /**
         * @brief Sets the user interface for interaction with the user.
         *
         * Associates the application with a UI implementation for displaying messages.
         *
         * @param ui Shared pointer to the \c Ui::UserInterface implementation.
         */
        void set_user_interface(std::shared_ptr<Ui::UserInterface> ui);

        /**
         * @brief Configures the AMXX builder used to generate compiled output.
         *
         * @param amxx_builder Unique pointer to an \c Amxx::Builder instance.
         */
        void set_amxx_builder(std::unique_ptr<Amxx::Builder> amxx_builder);

        /**
         * @brief Executes the application workflow.
         *
         * @return \c EXIT_SUCCESS if execution completes successfully,
         *         \c EXIT_FAILURE if an error occurs.
         *
         * @throw \c std::runtime_error If internal operations fail unexpectedly.
         *
         * @note Application must be properly configured with arguments,
         *       UI, and builder before invoking this function.
         */
        [[nodiscard]] int run() const;

      private:
        /// Interface for displaying output and errors to the user.
        std::shared_ptr<Ui::UserInterface> ui_{};

        /// Parsed command-line arguments governing program behavior.
        std::unique_ptr<Cli::Arguments> arguments_{};

        /// Builder responsible for producing AMXX binaries from AMX input.
        std::unique_ptr<Amxx::Builder> amxx_builder_{};
    };
}
