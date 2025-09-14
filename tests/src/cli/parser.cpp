#include "cli/parser.hpp"
#include "cli/exceptions/cli_error.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string_view>

namespace {
    std::filesystem::path create_temp_file(const std::string_view name)
    {
        std::filesystem::path path{name};
        std::ofstream file{path};

        file.put('x');
        file.close();

        return path;
    }

    TEST(ParserTest, EmptyArguments_ReturnsEmptyContainers)
    {
        // Arrange
        Cli::Parser parser{};
        constexpr Cli::Parser::Arguments args = {};

        // Act
        parser.parse(args, false);

        // Assert
        EXPECT_TRUE(parser.options().empty());
        EXPECT_TRUE(parser.definitions().empty());
        EXPECT_TRUE(parser.input_files().empty());
    }

    TEST(ParserTest, SimpleOptionWithoutValue_CorrectlyParsed)
    {
        // Arrange
        Cli::Parser parser;
        const std::vector<std::string> args = {"-l"};

        // Act
        parser.parse(args, false);

        // Assert
        ASSERT_EQ(parser.options().size(), 1);
        ASSERT_TRUE(parser.options().contains("l"));
        EXPECT_FALSE(parser.options().at("l").has_value());
        EXPECT_TRUE(parser.definitions().empty());
        EXPECT_TRUE(parser.input_files().empty());
    }

    TEST(ParserTest, OptionWithValueColonSeparator_CorrectlyParsed)
    {
        // Arrange
        Cli::Parser parser{};
        const std::vector<std::string> args = {"-o:output.amxx"};

        // Act
        parser.parse(args, false);

        // Assert
        ASSERT_EQ(parser.options().size(), 1);
        ASSERT_TRUE(parser.options().contains("o"));
        ASSERT_TRUE(parser.options().at("o").has_value());
        EXPECT_EQ(parser.options().at("o").value(), "output.amxx");
    }

    TEST(ParserTest, OptionWithEmptyValueAfterSeparator_CorrectlyParsed)
    {
        // Arrange
        Cli::Parser parser{};
        const std::vector<std::string> args = {"-o:"};

        // Act
        parser.parse(args, false);

        // Assert
        ASSERT_EQ(parser.options().size(), 1);
        ASSERT_TRUE(parser.options().contains("o"));
        EXPECT_FALSE(parser.options().at("o").has_value());
    }

    TEST(ParserTest, OptionWithValueEqualSeparator_CorrectlyParsed)
    {
        // Arrange
        Cli::Parser parser{};
        const std::vector<std::string> args = {"/S=8192"};

        // Act
        parser.parse(args, false);

        // Assert
        ASSERT_EQ(parser.options().size(), 1);
        ASSERT_TRUE(parser.options().contains("S"));
        ASSERT_TRUE(parser.options().at("S").has_value());
        EXPECT_EQ(parser.options().at("S").value(), "8192");
    }

    TEST(ParserTest, OptionSeparatorsColonEqualsAndPlain_AreEquivalent)
    {
        {
            // Arrange
            Cli::Parser parser{};
            std::vector<std::string> args{"-d2"};

            // Act
            EXPECT_NO_THROW(parser.parse(args, false));

            // Assert
            EXPECT_TRUE(parser.options().contains("d"));
            EXPECT_EQ(parser.options().at("d").value(), "2");
        }
        {
            // Arrange
            Cli::Parser parser{};
            std::vector<std::string> args{"-d=2"};

            // Act
            EXPECT_NO_THROW(parser.parse(args, false));

            // Assert
            EXPECT_TRUE(parser.options().contains("d"));
            EXPECT_EQ(parser.options().at("d").value(), "2");
        }
        {
            // Arrange
            Cli::Parser parser{};
            std::vector<std::string> args{"-d:2"};

            // Act
            EXPECT_NO_THROW(parser.parse(args, false));

            // Assert
            EXPECT_TRUE(parser.options().contains("d"));
            EXPECT_EQ(parser.options().at("d").value(), "2");
        }
    }

    TEST(ParserTest, SpecialSuiOptionWithValue_CorrectlyParsed)
    {
        // Arrange
        Cli::Parser parser{};
        const std::vector<std::string> args = {"-sui+"};

        // Act
        parser.parse(args, false);

        // Assert
        ASSERT_EQ(parser.options().size(), 1);
        ASSERT_TRUE(parser.options().contains("sui"));
        ASSERT_TRUE(parser.options().at("sui").has_value());
        EXPECT_EQ(parser.options().at("sui").value(), "+");
    }

    TEST(ParserTest, SpecialHelpOption_CorrectlyParsed)
    {
        // Arrange
        Cli::Parser parser{};
        const std::vector<std::string> args = {"--help", "-h", "-?"};

        // Act
        parser.parse(args, false);

        // Assert
        ASSERT_EQ(parser.options().size(), 3);
        ASSERT_TRUE(parser.options().contains("help"));
        ASSERT_TRUE(parser.options().contains("h"));
        ASSERT_TRUE(parser.options().contains("?"));
        ASSERT_FALSE(parser.options().at("help").has_value());
        ASSERT_FALSE(parser.options().at("h").has_value());
        ASSERT_FALSE(parser.options().at("?").has_value());
    }

    TEST(ParserTest, DuplicateOptionWithoutOverride_DoesNotUpdateValue)
    {
        // Arrange
        Cli::Parser parser{};
        parser.parse({"-A8"}, false);

        // Act
        parser.parse({"/A16"}, false); // Should not override

        // Assert
        ASSERT_EQ(parser.options().size(), 1);
        ASSERT_TRUE(parser.options().contains("A"));
        EXPECT_EQ(parser.options().at("A"), "8");
    }

    TEST(ParserTest, DuplicateOptionWithOverride_DoesUpdateValue)
    {
        // Arrange
        Cli::Parser parser{};
        parser.parse({"-A8"}, false);

        // Act
        parser.parse({"/A16"}, true); // Should override

        // Assert
        ASSERT_EQ(parser.options().size(), 1);
        ASSERT_TRUE(parser.options().contains("A"));
        EXPECT_EQ(parser.options().at("A"), "16");
    }

    TEST(ParserTest, DuplicateInputFiles_Ignored)
    {
        // Arrange
        Cli::Parser parser{};
        const auto temp_file = create_temp_file("main.sma");
        const Cli::Parser::Arguments args = {"main.sma", "main.sma"};

        // Act
        parser.parse(args, false);

        // Assert
        EXPECT_EQ(parser.input_files().size(), 1);

        // Cleanup
        EXPECT_NO_THROW(std::filesystem::remove(temp_file));
    }

    TEST(ParserTest, NonExistentInputFile_ThrowsException)
    {
        // Arrange
        Cli::Parser parser{};
        const Cli::Parser::Arguments args = {"nonexistent.sma"};

        // Act & Assert
        EXPECT_THROW(parser.parse(args, false), Cli::Exceptions::CliError);
    }

    TEST(ParserTest, ComprehensiveMixOfArguments_AreParsedCorrectly)
    {
        // Arrange
        Cli::Parser parser{};
        const auto plugin_path = create_temp_file("plugin.sma");

        std::vector<std::string> args{
            plugin_path.string(), // input file
            "-C+",                // option -C +
            "-DC:\\",             // option -D value "C:\"
            "-d2",                // option -d value "2"
            "-sui+",              // option -sui value "+"
            "-E",                 // flag -E
            "-\\",                // option -\ (escape char selection)
            "-TC:\\config.cfg",   // -T value "C:\config.cfg"
            "-;+",                // option -;
            "-(-",                // option -(
            "abc=",               // definition abc -> "1"
            "bca=22"              // definition bca -> "22"
        };

        // Act
        EXPECT_NO_THROW(parser.parse(args, false));

        // Assert
        const auto& inputs = parser.input_files();
        ASSERT_EQ(inputs.size(), 1u);
        EXPECT_EQ(inputs.front(), plugin_path);

        // Keys
        const auto& opts = parser.options();
        EXPECT_TRUE(opts.contains("C"));
        EXPECT_TRUE(opts.contains("D"));
        EXPECT_TRUE(opts.contains("d"));
        EXPECT_TRUE(opts.contains("sui"));
        EXPECT_TRUE(opts.contains("E"));
        EXPECT_TRUE(opts.contains("\\"));
        EXPECT_TRUE(opts.contains("T"));
        EXPECT_TRUE(opts.contains(";"));
        EXPECT_TRUE(opts.contains("("));

        // Values
        EXPECT_EQ(opts.at("C").value(), "+");
        EXPECT_EQ(opts.at("D").value(), "C:\\");
        EXPECT_EQ(opts.at("d").value(), "2");
        EXPECT_EQ(opts.at("sui").value(), "+");
        EXPECT_FALSE(opts.at("E").has_value());
        EXPECT_FALSE(opts.at("\\").has_value());
        EXPECT_EQ(opts.at("T").value(), "C:\\config.cfg");
        EXPECT_EQ(opts.at(";").value(), "+");
        EXPECT_EQ(opts.at("(").value(), "-");

        // Definitions
        const auto& defs = parser.definitions();
        EXPECT_TRUE(defs.contains("abc"));
        EXPECT_TRUE(defs.contains("bca"));
        EXPECT_EQ(defs.at("abc"), "1"); // empty value -> becomes "1"
        EXPECT_EQ(defs.at("bca"), "22");

        // Cleanup
        EXPECT_NO_THROW(std::filesystem::remove(plugin_path));
    }
}
