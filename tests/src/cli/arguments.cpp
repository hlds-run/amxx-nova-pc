#include "cli/arguments.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <span>

namespace {
    std::filesystem::path create_temp_file(const std::string_view name)
    {
        std::filesystem::path path{name};
        std::ofstream file{path};

        file.put('x');
        file.close();

        return path;
    }

    TEST(ArgumentsTest, ConstructorSpan_CorrectlyParsesArguments)
    {
        // Arrange
        const auto plugin_file = create_temp_file("plugin.sma");
        constexpr const char* argv[] = {"prog", "-a", "-bvalue", "def=42", "plugin.sma"};
        const std::span span{argv, std::size(argv)};

        // Act
        const Cli::Arguments args{span};

        // Assert
        EXPECT_EQ(args.get_executable_path().string(), "prog");
        EXPECT_TRUE(args.has_option("a"));
        EXPECT_TRUE(args.has_option("b"));
        EXPECT_EQ(args.get_option_value("b").value(), "value");
        EXPECT_EQ(args.get_arguments().front(), "prog");
        ASSERT_EQ(args.get_input_files().size(), 1u);
        EXPECT_EQ(args.get_input_files()[0].string(), "plugin.sma");

        // Cleanup
        std::filesystem::remove(plugin_file);
    }

    TEST(ArgumentsTest, ConstructorArgcArgv_CorrectlyParsesArguments)
    {
        // Arrange
        const auto file = create_temp_file("file.sma");
        constexpr const char* argv[] = {"prog", "-x", "foo=bar", "file.sma"};
        constexpr int argc = std::size(argv);

        // Act
        const Cli::Arguments args{argc, argv};

        // Assert
        EXPECT_EQ(args.get_executable_path().string(), "prog");
        EXPECT_TRUE(args.has_option("x"));
        EXPECT_EQ(args.get_option_value("x"), std::nullopt);
        EXPECT_EQ(args.get_arguments().front(), "prog");
        ASSERT_EQ(args.get_input_files().size(), 1u);
        EXPECT_EQ(args.get_input_files()[0].string(), "file.sma");

        // Cleanup
        std::filesystem::remove(file);
    }

    TEST(ArgumentsTest, EmptyArgs_ContainsOnlyExecutablePath)
    {
        // Arrange
        constexpr const char* argv[] = {"prog"};

        // Act
        const Cli::Arguments args{std::size(argv), argv};

        // Assert
        EXPECT_EQ(args.get_executable_path().string(), "prog");
        EXPECT_EQ(args.get_arguments().size(), 1u);
        EXPECT_TRUE(args.get_input_files().empty());
    }

    TEST(ArgumentsTest, Definitions_IncludedInNormalizedArgs)
    {
        // Arrange
        constexpr const char* argv[] = {"prog", "foo=bar", "baz=qux"};

        // Act
        const Cli::Arguments args{std::size(argv), argv};
        const auto& normalized = args.get_arguments();

        // Assert
        EXPECT_TRUE(std::ranges::any_of(normalized, [](const std::string& s) { return s == "foo=bar"; }));
        EXPECT_TRUE(std::ranges::any_of(normalized, [](const std::string& s) { return s == "baz=qux"; }));
    }

    TEST(ArgumentsTest, Options_ParsesBothValueAndNonValueOptions)
    {
        // Arrange
        constexpr const char* argv[] = {"prog", "-a", "-bval", "-c"};

        // Act
        const Cli::Arguments args{std::size(argv), argv};

        // Assert
        EXPECT_TRUE(args.has_option("a"));
        EXPECT_TRUE(args.has_option("b"));
        EXPECT_TRUE(args.has_option("c"));
        EXPECT_EQ(args.get_option_value("a"), std::nullopt);
        EXPECT_EQ(args.get_option_value("b").value(), "val");
        EXPECT_EQ(args.get_option_value("c"), std::nullopt);
    }

    TEST(ArgumentsTest, MultipleInputFiles_CorrectlyIdentified)
    {
        // Arrange
        const auto file1 = create_temp_file("file1.txt");
        const auto file2 = create_temp_file("file2.txt");
        const auto file3 = create_temp_file("file3.txt");
        constexpr const char* argv[] = {"prog", "file1.txt", "file2.txt", "file3.txt"};

        // Act
        const Cli::Arguments args{std::size(argv), argv};
        const auto& inputs = args.get_input_files();

        // Assert
        ASSERT_EQ(inputs.size(), 3u);
        EXPECT_EQ(inputs[0].string(), "file1.txt");
        EXPECT_EQ(inputs[1].string(), "file2.txt");
        EXPECT_EQ(inputs[2].string(), "file3.txt");

        // Cleanup
        std::filesystem::remove(file1);
        std::filesystem::remove(file2);
        std::filesystem::remove(file3);
    }

    TEST(ArgumentsTest, NonExistingOption_ReturnsNullopt)
    {
        // Arrange
        constexpr const char* argv[] = {"prog", "-x", "-yval"};

        // Act
        const Cli::Arguments args{std::size(argv), argv};

        // Assert
        EXPECT_EQ(args.get_option_value("z"), std::nullopt);
        EXPECT_FALSE(args.has_option("z"));
    }

    TEST(ArgumentsTest, NormalizedArguments_FirstArgumentIsExecutablePath)
    {
        // Arrange
        constexpr const char* argv[] = {"prog", "-a", "def=val"};

        // Act
        const Cli::Arguments args{std::size(argv), argv};
        const auto& normalized = args.get_arguments();

        // Assert
        ASSERT_FALSE(normalized.empty());
        EXPECT_EQ(normalized.front(), "prog");
        EXPECT_TRUE(std::ranges::any_of(normalized, [](const std::string& s) { return s == "-a"; }));
        EXPECT_TRUE(std::ranges::any_of(normalized, [](const std::string& s) { return s == "def=val"; }));
    }

    TEST(ArgumentsTest, OptionsWithEmptyValue_CorrectlyParsesValue)
    {
        // Arrange
        constexpr const char* argv[] = {"prog", "-a=", "-bval"};

        // Act
        const Cli::Arguments args{std::size(argv), argv};

        // Assert
        EXPECT_TRUE(args.has_option("a"));
        EXPECT_EQ(args.get_option_value("a"), std::nullopt);
        EXPECT_TRUE(args.has_option("b"));
        EXPECT_EQ(args.get_option_value("b").value(), "val");
    }

    TEST(ArgumentsTest, HelpAndTOptions_ExcludedFromNormalizedArgs)
    {
        // Arrange
        const std::filesystem::path exe_dir = std::filesystem::current_path();
        const std::filesystem::path target_dir = exe_dir / "target";
        std::filesystem::create_directories(target_dir);

        const std::filesystem::path config_path = target_dir / "val.cfg";
        {
            std::ofstream(config_path) << "# dummy config\n";
        }

        constexpr const char* argv[] = {"prog", "--help", "-h", "-?", "-Tval"};

        // Act
        const Cli::Arguments args{std::size(argv), argv};
        const auto& norm_args = args.get_arguments();

        // Assert
        EXPECT_FALSE(std::ranges::any_of(norm_args, [](const std::string& s) { return s == "--help"; }));
        EXPECT_FALSE(std::ranges::any_of(norm_args, [](const std::string& s) { return s == "-?"; }));
        EXPECT_FALSE(std::ranges::any_of(norm_args, [](const std::string& s) { return s == "-Tval"; }));
        EXPECT_TRUE(std::ranges::any_of(norm_args, [](const std::string& s) { return s == "-h"; }));

        // Cleanup
        std::filesystem::remove(config_path);
    }

    TEST(ArgumentsTest, MixedArguments_CorrectlyIdentifiesInputFilesAndOptions)
    {
        // Arrange
        const auto file1 = create_temp_file("file1.txt");
        const auto file2 = create_temp_file("file2.txt");
        const auto file3 = create_temp_file("file3.txt");
        constexpr const char* argv[] = {"prog", "file1.txt", "-a", "file2.txt", "foo=bar", "-bval", "file3.txt"};

        // Act
        const Cli::Arguments args{std::size(argv), argv};
        const auto& inputs = args.get_input_files();
        const auto& normalized = args.get_arguments();

        // Assert
        ASSERT_EQ(inputs.size(), 3u);
        EXPECT_EQ(inputs[0].string(), "file1.txt");
        EXPECT_EQ(inputs[1].string(), "file2.txt");
        EXPECT_EQ(inputs[2].string(), "file3.txt");

        EXPECT_TRUE(std::ranges::any_of(normalized, [](const std::string& s) { return s == "-a"; }));
        EXPECT_TRUE(std::ranges::any_of(normalized, [](const std::string& s) { return s == "-bval"; }));
        EXPECT_TRUE(std::ranges::any_of(normalized, [](const std::string& s) { return s == "foo=bar"; }));
        EXPECT_EQ(normalized.front(), "prog");

        // Cleanup
        std::filesystem::remove(file1);
        std::filesystem::remove(file2);
        std::filesystem::remove(file3);
    }
}
