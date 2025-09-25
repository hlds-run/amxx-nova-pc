#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("AMXX" LANGUAGES "CXX")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "amxx")
set(TARGET_ALIAS "${PROJECT_NAME}::${TARGET_NAME}")

add_library("${TARGET_NAME}" STATIC)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PUBLIC
    "include/amxx/builder.hpp"
    "include/amxx/consts.hpp"
    "include/amxx/data.hpp"
    "include/amxx/entry.hpp"
    "include/amxx/header.hpp"
    "include/amxx/writer.hpp"

  PRIVATE
    "src/amxx/builder.cpp"
    "src/amxx/writer.cpp"
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
    "AMX::amx"
    "Binary::binary"
    "Compress::compress"
    "Concepts::concepts"
)
