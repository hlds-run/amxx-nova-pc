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
    "include/pawn/image/amxx/builder.hpp"
    "include/pawn/image/amxx/consts.hpp"
    "include/pawn/image/amxx/data.hpp"
    "include/pawn/image/amxx/entry.hpp"
    "include/pawn/image/amxx/header.hpp"
    "include/pawn/image/amxx/writer.hpp"

  PRIVATE
    "src/builder.cpp"
    "src/writer.cpp"
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
    "Compress::compress"
    "Concepts::concepts"
    "IO::io"
)
