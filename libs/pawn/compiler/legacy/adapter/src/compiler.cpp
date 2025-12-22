#include "pawn/compiler/legacy/adapter/compiler.hpp"
#include "amx.h"
#include "pawn/compiler/legacy/adapter/option.hpp"
#include <algorithm>
#include <cstddef>
#include <optional>
#include <print>

extern "C" {
#include "sc.h"
}

namespace {
    /**
     * @brief Global repository of supported compiler options.
     *
     * Initialized when the program starts and is used to provide
     * a single view of the available command line flags.
     *
     * @note Includes platform-specific and configuration-dependent options.
     */
    const std::vector<PawnWrap::Option> options = [] {
        std::vector<PawnWrap::Option> opts{};
        initglobals();

        opts.emplace_back(PawnWrap::Option{.flag = "-A<num>",
            .description = "alignment in bytes of the data segment and the stack",
            .default_value = std::nullopt});

        opts.emplace_back(
            PawnWrap::Option{.flag = "-a", .description = "output assembler code", .default_value = std::nullopt});

#if AMX_COMPACTMARGIN > 2
        opts.emplace_back(PawnWrap::Option{.flag = "-C[+/-]",
            .description = "compact encoding for output file",
            .default_value = sc_compress ? "+" : "-"});
#endif
        opts.emplace_back(PawnWrap::Option{.flag = "-c<name>",
            .description = "codepage name or number; e.g. 1252 for Windows Latin-1",
            .default_value = std::nullopt});

#if defined dos_setdrive
        opts.emplace_back(
            PawnWrap::Option{.flag = "-Dpath", .description = "active directory path", .default_value = std::nullopt});
#endif
        opts.emplace_back(PawnWrap::Option{.flag = "-d0",
            .description = "no symbolic information, no run-time checks",
            .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{.flag = "-d1",
            .description = "[default] run-time checks, no symbolic information",
            .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{.flag = "-d2",
            .description = "full debug information and dynamic checking",
            .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{.flag = "-d3",
            .description = "full debug information, dynamic checking, no optimization",
            .default_value = std::nullopt});

        opts.emplace_back(
            PawnWrap::Option{.flag = "-E", .description = "treat warnings as errors", .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{.flag = "-e<name>",
            .description = "set name of error file (quiet compile)",
            .default_value = std::nullopt});

#if defined __WIN32__ || defined _WIN32 || defined _Windows
        opts.emplace_back(PawnWrap::Option{.flag = "-H<hwnd>",
            .description = "window handle to send a notification message on finish",
            .default_value = std::nullopt});
#endif
        opts.emplace_back(
            PawnWrap::Option{.flag = "-h", .description = "show included file paths", .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{
            .flag = "-i<name>", .description = "path for include files", .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{
            .flag = "-l", .description = "create list file (preprocess only)", .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{
            .flag = "-o<name>", .description = "set base name of (P-code) output file", .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{
            .flag = "-p<name>", .description = "set name of \"prefix\" file", .default_value = std::nullopt});

#if !defined SC_LIGHT
        opts.emplace_back(PawnWrap::Option{.flag = "-r[name]",
            .description = "write cross reference report to console or to specified file",
            .default_value = std::nullopt});
#endif
        opts.emplace_back(PawnWrap::Option{
            .flag = "-S<num>", .description = "stack/heap size in cells", .default_value = std::to_string(sc_stksize)});

        opts.emplace_back(PawnWrap::Option{
            .flag = "-s<num>", .description = "skip lines from the input file", .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{
            .flag = "-sui[+/-]", .description = "show stack usage info", .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{.flag = "-T<name>",
            .description = "set name of the configuration file to use",
            .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{.flag = "-t<num>",
            .description = "TAB indent size (in character positions)",
            .default_value = std::to_string(sc_tabsize)});

        opts.emplace_back(PawnWrap::Option{.flag = "-v<num>",
            .description = "verbosity level; 0=quiet, 1=normal, 2=verbose",
            .default_value = std::to_string(sc_verbosity)});

        opts.emplace_back(PawnWrap::Option{.flag = "-w<num>",
            .description = "disable a specific warning by its number",
            .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{
            .flag = "-X<num>", .description = "abstract machine size limit in bytes", .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{
            .flag = "-\\", .description = "use '\\' for escape characters", .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{
            .flag = "-^", .description = "use '^' for escape characters", .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{.flag = "-;[+/-]",
            .description = "require a semicolon to end each statement",
            .default_value = sc_needsemicolon ? "+" : "-"});

        opts.emplace_back(PawnWrap::Option{.flag = "-([+/-]",
            .description = "require parentheses for function invocation",
            .default_value = sc_optproccall ? "-" : "+"});

        opts.emplace_back(PawnWrap::Option{.flag = "sym=val",
            .description = R"(define constant "sym" with value "val")",
            .default_value = std::nullopt});

        opts.emplace_back(PawnWrap::Option{
            .flag = "sym=", .description = "define constant \"sym\" with value 1", .default_value = std::nullopt});

        return opts;
    }();
}

namespace PawnWrap {
    const std::vector<Option>& get_options()
    {
        return options;
    }

    bool compile(const std::filesystem::path& file_path, const std::vector<std::string>& args)
    {
        std::vector<const char*> compiler_args{};
        compiler_args.reserve(args.size() + 1u);

        // the path to the executable file must be first
        compiler_args.push_back(args[0].c_str());

        const auto file_path_string = file_path.string();
        compiler_args.push_back(file_path_string.c_str());

        for (std::size_t i = 1; i < args.size(); ++i) {
            compiler_args.push_back(args[i].c_str());
        }

        const auto success = pc_compile(static_cast<int>(compiler_args.size()), compiler_args.data()) <= 1;
        std::print("\n");

        return success;
    }

    bool compile(const std::vector<std::filesystem::path>& file_paths, const std::vector<std::string>& args)
    {
        const auto compile_single_file = [&](const auto& file_path) {
            return compile(file_path, args);
        };

        return std::ranges::all_of(file_paths, compile_single_file);
    }
}
