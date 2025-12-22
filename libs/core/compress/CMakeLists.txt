#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Compress" LANGUAGES "CXX")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "compress")
set(TARGET_ALIAS "${PROJECT_NAME}::${TARGET_NAME}")

add_library("${TARGET_NAME}" STATIC)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PUBLIC
    "include/core/compress/compressor.hpp"
    "include/core/compress/zlib_compressor.hpp"
    "include/core/compress/exceptions/compress_exception.hpp"
    "include/core/compress/exceptions/zlib_error.hpp"

  PRIVATE
    "src/zlib_compressor.cpp"
    "src/exceptions/zlib_error.cpp"
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

  PRIVATE
    "ZLib::zlib"
)
