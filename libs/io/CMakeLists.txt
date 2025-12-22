#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("IO" LANGUAGES "CXX")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "io")
set(TARGET_ALIAS "${PROJECT_NAME}::${TARGET_NAME}")

add_library("${TARGET_NAME}" STATIC)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PUBLIC
    "include/io/binary_reader.hpp"
    "include/io/binary_writer.hpp"
    "include/io/exceptions/stream_error.hpp"
    "include/io/exceptions/stream_exception.hpp"

  PRIVATE
    "src/binary_reader.cpp"
    "src/binary_writer.cpp"
    "src/exceptions/end_of_file_error.cpp"
    "src/exceptions/file_close_error.cpp"
    "src/exceptions/file_open_error.cpp"
    "src/exceptions/seek_error.cpp"
    "src/exceptions/stream_failure_error.cpp"
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
)
