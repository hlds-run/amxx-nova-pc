#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Pawn Legacy Adapter" LANGUAGES "CXX")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "pawnadapter")
set(TARGET_ALIAS "PawnAdapter::${TARGET_NAME}")

add_library("${TARGET_NAME}" STATIC)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PUBLIC
    "include/pawn/compiler/legacy/adapter/compiler.hpp"
    "include/pawn/compiler/legacy/adapter/option.hpp"

  PRIVATE
    "src/compiler.cpp"
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
    "PawnLegacy::pawnlegacy"
)
