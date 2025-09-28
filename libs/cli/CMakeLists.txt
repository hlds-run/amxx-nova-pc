#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Command-Line Interface" LANGUAGES "CXX")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "cli")
set(TARGET_ALIAS "CLI::${TARGET_NAME}")

add_library("${TARGET_NAME}" STATIC)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PUBLIC
    "include/cli/arguments.hpp"
    "include/cli/parser.hpp"
    "include/cli/exceptions/cli_error.hpp"
    "include/cli/exceptions/cli_exception.hpp"

  PRIVATE
    "src/cli/arguments.cpp"
    "src/cli/config.cpp"
    "src/cli/config.hpp"
    "src/cli/parser.cpp"
)

#-------------------------------------------------------------------------------
# Include Directories
#-------------------------------------------------------------------------------

target_include_directories("${TARGET_NAME}"
  PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${TARGET_NAME}"
  PRIVATE
    "PawnCompiler::pawnwrap"
)
