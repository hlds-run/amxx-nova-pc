#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Command-Line Interface" LANGUAGES "CXX")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "cmdline")
set(TARGET_ALIAS "CmdLine::${TARGET_NAME}")

add_library("${TARGET_NAME}" STATIC)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PUBLIC
    "include/interface/cmdline/arguments.hpp"
    "include/interface/cmdline/exceptions/cli_error.hpp"
    "include/interface/cmdline/exceptions/cli_exception.hpp"
    "include/interface/cmdline/parser.hpp"

  PRIVATE
    "src/arguments.cpp"
    "src/config.cpp"
    "src/config.hpp"
    "src/parser.cpp"
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
    "PawnAdapter::pawnadapter"
)
