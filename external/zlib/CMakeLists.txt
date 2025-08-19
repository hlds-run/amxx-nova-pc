#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("ZLib")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "zlib")
set(TARGET_ALIAS "${PROJECT_NAME}::${TARGET_NAME}")

add_library("${TARGET_NAME}")
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PRIVATE
    "src/adler32.c"
    "src/compress.c"
    "src/crc32.c"
    "src/deflate.c"
    "src/gzclose.c"
    "src/gzlib.c"
    "src/gzread.c"
    "src/gzwrite.c"
    "src/infback.c"
    "src/inffast.c"
    "src/inflate.c"
    "src/inftrees.c"
    "src/trees.c"
    "src/uncompr.c"
    "src/zutil.c"
)

#-------------------------------------------------------------------------------
# Include Directories
#-------------------------------------------------------------------------------

target_include_directories("${TARGET_NAME}" SYSTEM
  PUBLIC
    "${PROJECT_SOURCE_DIR}/src"
)

#-------------------------------------------------------------------------------
# Compile Definitions
#-------------------------------------------------------------------------------

target_compile_definitions("${TARGET_NAME}"
  PRIVATE
    $<$<PLATFORM_ID:Windows>:
      _MBCS
    >

    $<$<NOT:$<PLATFORM_ID:Windows>>:
      HAVE_UNISTD_H
    >
)
