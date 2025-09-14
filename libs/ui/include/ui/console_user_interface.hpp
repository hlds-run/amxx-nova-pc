#pragma once

#include "ui/user_interface.hpp"
#include <string_view>

namespace Ui {
    /**
     * @brief Console-based implementation of the UserInterface.
     *
     * Outputs all messages and documentation
     * to the standard console using formatted text.
     */
    class ConsoleUserInterface final : public UserInterface {
      public:
        /**
         * @brief Displays application header with version information.
         *
         * @param app_version Application version descriptor.
         */
        void show_header(const AppVersion& app_version) override;

        /**
         * @brief Displays basic usage information.
         */
        void show_usage() override;

        /**
         * @brief Displays a general informational message.
         *
         * @param message Message content to present.
         */
        void show_info(std::string_view message) override;

        /**
         * @brief Displays a warning message to alert the user.
         *
         * @param message Message content to present.
         */
        void show_warning(std::string_view message) override;

        /**
         * @brief Displays an error message.
         *
         * @param message Error content to present.
         */
        void show_error(std::string_view message) override;

        /**
         * @brief Displays help documentation with available options.
         *
         * @param options List of supported compiler options with flags,
         *        descriptions, and optional default values.
         *
         * @see PawnWrap::Option for option structure details.
         */
        void show_help(const std::vector<PawnWrap::Option>& options) override;
    };
}
