#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project("AMXX Nova Pawn Compiler")

#-------------------------------------------------------------------------------
# Target Definition
#-------------------------------------------------------------------------------

set(TARGET_NAME "amxxpc")
add_executable("${TARGET_NAME}")

#-------------------------------------------------------------------------------
# Source Files
#-------------------------------------------------------------------------------

target_sources("${TARGET_NAME}"
  PRIVATE
    "src/amx.cpp"
    "src/amxxpc.cpp"
    "src/binary.cpp"
)

set(VERSION_FILENAME "version.h")
set(VERSION_FILEPATH "${PROJECT_BINARY_DIR}/${VERSION_FILENAME}")

configure_file(
  "src/${VERSION_FILENAME}.in"
  "${VERSION_FILEPATH}"
  @ONLY
)

target_sources("${TARGET_NAME}"
  PRIVATE
    "${VERSION_FILEPATH}"
)

if(WIN32)
  set(RESOURCE_FILENAME "${TARGET_NAME}.exe.rc")
  set(MANIFEST_FILENAME "${TARGET_NAME}.exe.manifest")

  configure_file(
    "src/platform/windows/${RESOURCE_FILENAME}.in"
    "${PROJECT_BINARY_DIR}/${RESOURCE_FILENAME}"
    @ONLY
  )

  configure_file(
    "src/platform/windows/${MANIFEST_FILENAME}.in"
    "${PROJECT_BINARY_DIR}/${MANIFEST_FILENAME}"
    @ONLY
  )

  target_sources("${TARGET_NAME}"
    PRIVATE
      "${PROJECT_BINARY_DIR}/${RESOURCE_FILENAME}"
      "${PROJECT_BINARY_DIR}/${MANIFEST_FILENAME}"
  )
endif()

#-------------------------------------------------------------------------------
# Include Directories
#-------------------------------------------------------------------------------

target_include_directories("${TARGET_NAME}"
  PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>"
)

#-------------------------------------------------------------------------------
# Target Properties
#-------------------------------------------------------------------------------

set_target_properties("${TARGET_NAME}"
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_DIR}"
    POSITION_INDEPENDENT_CODE OFF
)

#-------------------------------------------------------------------------------
# Compile Definitions
#-------------------------------------------------------------------------------

target_compile_definitions("${TARGET_NAME}"
  PRIVATE
    AMX_ANSIONLY

    $<$<PLATFORM_ID:Windows>:
      _MBCS
      _CRT_SECURE_NO_WARNINGS
    >

    $<$<NOT:$<PLATFORM_ID:Windows>>:
      LINUX
      HAVE_STDINT_H
    >
)

#-------------------------------------------------------------------------------
# Link Options
#-------------------------------------------------------------------------------

target_link_options("${TARGET_NAME}"
  PRIVATE
    $<$<PLATFORM_ID:Windows>:
      /SUBSYSTEM:CONSOLE
    >
)

#-------------------------------------------------------------------------------
# Link Libraries
#-------------------------------------------------------------------------------

target_link_libraries("${TARGET_NAME}"
  PRIVATE
    "ZLib::zlib"
    "PCLib::amxxpc32"

    $<$<NOT:$<PLATFORM_ID:Windows>>:
      "${CMAKE_DL_LIBS}"
    >
)
