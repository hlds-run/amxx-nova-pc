#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("AMX" LANGUAGES "CXX")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "amx")
set(TARGET_ALIAS "${PROJECT_NAME}::${TARGET_NAME}")

add_library("${TARGET_NAME}" STATIC)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PUBLIC
    "include/pawn/image/amx/consts.hpp"
    "include/pawn/image/amx/data.hpp"
    "include/pawn/image/amx/header_debug.hpp"
    "include/pawn/image/amx/header.hpp"
    "include/pawn/image/amx/reader.hpp"
    "include/pawn/image/amx/exceptions/reader_error.hpp"
    "include/pawn/image/amx/exceptions/reader_exception.hpp"

  PRIVATE
    "src/reader.cpp"
    "src/exceptions/invalid_magic_error.cpp"
    "src/exceptions/no_debug_info_error.cpp"
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
  PUBLIC
    "Concepts::concepts"
    "IO::io"
)
