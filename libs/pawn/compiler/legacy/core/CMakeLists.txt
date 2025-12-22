#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("Pawn Legacy" LANGUAGES "C")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "pawnlegacy")
set(TARGET_ALIAS "PawnLegacy::${TARGET_NAME}")

add_library("${TARGET_NAME}")
add_library("${TARGET_ALIAS}" ALIAS "${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PRIVATE
    "src/libpawnc.c"
    "src/lstring.c"
    "src/memfile.c"
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
  PUBLIC
    HAVE_STDINT_H

  PRIVATE
    NO_MAIN

    $<$<BOOL:${BUILD_SHARED_LIBS}>:
      PAWNC_DLL
    >

    $<$<PLATFORM_ID:Windows>:
      _CRT_SECURE_NO_WARNINGS
      strdup=_strdup
    >

    $<$<NOT:$<PLATFORM_ID:Windows>>:
      LINUX
      ENABLE_BINRELOC
    >
)
