#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Core")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "core")
set(TARGET_ALIAS "${PROJECT_NAME}::${TARGET_NAME}")

add_library("${TARGET_NAME}" INTERFACE)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Subdirectories
#-------------------------------------------------------------------------------

add_subdirectory("compress")
add_subdirectory("concepts")
add_subdirectory("io")

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${TARGET_NAME}"
  INTERFACE
    Compress::compress
    Concepts::concepts
    IO::io
)
