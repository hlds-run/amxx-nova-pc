#pragma once

#include "cli/exceptions/cli_exception.hpp"
#include <string>

namespace Cli::Exceptions {
    /**
     * @brief Exception representing unrecoverable CLI parsing errors.
     *
     * Signals critical problems with command-line arguments.
     */
    class CliError final : public CliException {
      public:
        /**
         * @brief Constructs an error with a descriptive message.
         *
         * @param message Explanation of the specific parsing error.
         */
        explicit CliError(const std::string& message);
    };

    inline CliError::CliError(const std::string& message) : CliException(message)
    {
    }
}
