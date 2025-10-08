#include "ui/console_user_interface.hpp"
#include "pawnwrap/option.hpp"
#include "ui/app_version.hpp"
#include <algorithm>
#include <cstddef>
#include <optional>
#include <print>
#include <string>
#include <vector>

namespace Ui {
    void ConsoleUserInterface::show_header(const AppVersion& app_version)
    {
        std::print("AMX Mod X Compiler {}\n\n", app_version.string);
        std::print("Copyright (c) 1997-2006 ITB CompuPhase\n");
        std::print("Copyright (c) 2004-2013 AMX Mod X Team\n");
        std::print("Copyright (c) 2025-{} hlds.run Team\n\n", app_version.year);
    }

    void ConsoleUserInterface::show_usage()
    {
        std::print("Usage:\n");
        std::print("  amxxpc <filename> [filename...] [options]\n");
        std::print("  Use --help or -? to see the full list of options.\n");
    }

    void ConsoleUserInterface::show_info(const std::string_view message)
    {
        std::print("{}", message);
    }

    void ConsoleUserInterface::show_warning(const std::string_view message)
    {
        std::print("\n{}.\n", message);
    }

    void ConsoleUserInterface::show_error(const std::string_view message)
    {
        std::print("\n{}.\n", message);
    }

    void ConsoleUserInterface::show_help(const std::vector<PawnWrap::Option>& options)
    {
        std::size_t max_flag_len = 0;

        for (const auto& option : options) {
            max_flag_len = std::max(max_flag_len, option.flag.length());
        }

        std::print("\nOptions:\n");

        for (const auto& [flag, description, default_value] : options) {
            if (default_value) {
                std::print("  {:<{}}    {} (default={})\n", flag, max_flag_len, description, *default_value);
            }
            else {
                std::print("  {:<{}}    {}\n", flag, max_flag_len, description);
            }
        }

        std::print("\nOptions may start with a dash or a slash.\n");
        std::print("The options \"-d0\" and \"/d0\" are equivalent.\n");
        std::print("\nOptions with a value may optionally separate the value from the option letter\n");
        std::print("with a colon (\":\") or an equal sign (\"=\"). That is, the options \"-d0\", \"-d=0\"\n");
        std::print("and \"-d:0\" are all equivalent.\n");
    }
}
