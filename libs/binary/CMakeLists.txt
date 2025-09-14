#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Binary" LANGUAGES "CXX")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "binary")
set(TARGET_ALIAS "${PROJECT_NAME}::${TARGET_NAME}")

add_library("${TARGET_NAME}" STATIC)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PUBLIC
    "include/binary/reader.hpp"
    "include/binary/writer.hpp"
    "include/binary/exceptions/stream_error.hpp"
    "include/binary/exceptions/stream_exception.hpp"

  PRIVATE
    "src/binary/reader.cpp"
    "src/binary/writer.cpp"
    "src/binary/exceptions/end_of_file_error.cpp"
    "src/binary/exceptions/file_close_error.cpp"
    "src/binary/exceptions/file_open_error.cpp"
    "src/binary/exceptions/seek_error.cpp"
    "src/binary/exceptions/stream_failure_error.cpp"
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
