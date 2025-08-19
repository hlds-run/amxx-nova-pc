#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("AMXX Nova Pawn Compiler Lib")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "amxxpc32")
set(TARGET_ALIAS "PCLib::${TARGET_NAME}")

add_library("${TARGET_NAME}" SHARED)
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PRIVATE
    "src/libpawnc.c"
    "src/memfile.c"
    "src/pawncc.c"
    "src/prefix.c"
    "src/sc1.c"
    "src/sc2.c"
    "src/sc3.c"
    "src/sc4.c"
    "src/sc5.c"
    "src/sc6.c"
    "src/sc7.c"
    "src/sci18n.c"
    "src/sclist.c"
    "src/scmemfil.c"
    "src/scstate.c"
    "src/scvars.c"
    "src/sp_symhash.c"
)

#-------------------------------------------------------------------------------
# Include Directories
#-------------------------------------------------------------------------------

target_include_directories("${TARGET_NAME}"
  PUBLIC
    "${PROJECT_SOURCE_DIR}/src"
)

#-------------------------------------------------------------------------------
# Target Properties
#-------------------------------------------------------------------------------

set_target_properties("${TARGET_NAME}"
  PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
)

#-------------------------------------------------------------------------------
# Compile Definitions
#-------------------------------------------------------------------------------

target_compile_definitions("${TARGET_NAME}"
  PRIVATE
    NO_MAIN
    PAWNC_DLL
    HAVE_STDINT_H
    PAWN_CELL_SIZE=32

    $<$<PLATFORM_ID:Windows>:
      _MBCS
      _CRT_SECURE_NO_WARNINGS
      _CRT_SECURE_NO_DEPRECATE
    >

    $<$<NOT:$<PLATFORM_ID:Windows>>:
      LINUX
      ENABLE_BINRELOC
    >
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${TARGET_NAME}"
  PRIVATE
    $<$<NOT:$<PLATFORM_ID:Windows>>:
      "c"
      "m"
      "Threads::Threads"
    >
)
