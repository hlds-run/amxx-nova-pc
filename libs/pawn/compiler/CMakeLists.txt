#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Pawn Compiler")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "pawncompiler")
set(TARGET_ALIAS "PawnCompiler::${TARGET_NAME}")

add_library("${TARGET_NAME}" INTERFACE)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Subdirectories
#-------------------------------------------------------------------------------

add_subdirectory("legacy/adapter")
add_subdirectory("legacy/core")

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${TARGET_NAME}"
  INTERFACE
    PawnAdapter::pawnadapter
    PawnLegacy::pawnlegacy
)
