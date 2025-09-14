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
    "include/amx/consts.hpp"
    "include/amx/data.hpp"
    "include/amx/header_debug.hpp"
    "include/amx/header.hpp"
    "include/amx/reader.hpp"
    "include/amx/exceptions/reader_error.hpp"
    "include/amx/exceptions/reader_exception.hpp"

  PRIVATE
    "src/amx/reader.cpp"
    "src/amx/exceptions/invalid_magic_error.cpp"
    "src/amx/exceptions/no_debug_info_error.cpp"
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
    "Binary::binary"
    "Concepts::concepts"
)
