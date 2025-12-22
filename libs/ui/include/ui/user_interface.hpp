#pragma once

#include "pawn/compiler/legacy/adapter/option.hpp"
#include "ui/app_version.hpp"
#include <string_view>
#include <vector>

namespace Ui {
    /**
     * @brief Abstract interface for user interaction in the application.
     *
     * Provides a contract for displaying messages to end-users.
     * Implementations define how the interaction is presented
     * (e.g., console, GUI, logging system).
     */
    class UserInterface {
      public:
        /// Default constructor.
        UserInterface() = default;

        /// Copy constructor.
        UserInterface(const UserInterface&) = default;

        /// Move constructor.
        UserInterface(UserInterface&&) noexcept = default;

        /// Virtual destructor for safe polymorphic cleanup.
        virtual ~UserInterface() = default;

        /// Copy assignment operator.
        UserInterface& operator=(const UserInterface&) = default;

        /// Move assignment operator.
        UserInterface& operator=(UserInterface&&) noexcept = default;

        /**
         * @brief Displays application header with version information.
         *
         * @param app_version Application version descriptor.
         */
        virtual void show_header(const AppVersion& app_version) = 0;

        /**
         * @brief Displays basic usage information.
         */
        virtual void show_usage() = 0;

        /**
         * @brief Displays a general informational message.
         *
         * @param message Message content to present.
         */
        virtual void show_info(std::string_view message) = 0;

        /**
         * @brief Displays a warning message to alert the user.
         *
         * @param message Message content to present.
         */
        virtual void show_warning(std::string_view message) = 0;

        /**
         * @brief Displays an error message.
         *
         * @param message Error content to present.
         */
        virtual void show_error(std::string_view message) = 0;

        /**
         * @brief Displays help documentation with available options.
         *
         * @param options List of supported compiler options with flags,
         *        descriptions, and optional default values.
         *
         * @see PawnWrap::Option for option structure details.
         */
        virtual void show_help(const std::vector<PawnWrap::Option>& options) = 0;
    };
}
