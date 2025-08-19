# Check and create the .git/hooks directory if it doesn't exist
set(GIT_HOOKS_DIR "${CMAKE_SOURCE_DIR}/.git/hooks")
file(MAKE_DIRECTORY "${GIT_HOOKS_DIR}")

# Set the source and destination paths for the pre-commit hook.
set(HOOK_SRC "${CMAKE_SOURCE_DIR}/tools/pre-commit.sh")
set(HOOK_DST "${GIT_HOOKS_DIR}/pre-commit")

# Make clang-* binaries executable on Linux/Darwin
if(NOT WIN32)
  # Determine the tools directory based on the OS
  if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set(TOOLS_DIR "tools/macos")
  else()
    set(TOOLS_DIR "tools/linux")
  endif()

  # Find all clang-* binaries in the appropriate tools folder
  file(GLOB CLANG_BINS "${CMAKE_SOURCE_DIR}/${TOOLS_DIR}/clang-*")

  foreach(BIN_FILE ${CLANG_BINS})
    if(NOT EXISTS "${BIN_FILE}")
      continue()
    endif()

    message(STATUS "Making clang-* binary executable: ${BIN_FILE}")

    execute_process(
      COMMAND chmod +x "${BIN_FILE}"
      RESULT_VARIABLE CHMOD_RESULT
      ERROR_QUIET
    )

    if(NOT CHMOD_RESULT EQUAL 0)
      message(FATAL_ERROR "Failed to make ${BIN_FILE} executable.")
    endif()
  endforeach()
endif()

# Check if the source hook file exists
if(EXISTS "${HOOK_SRC}")
  message(STATUS "Installing git hook: ${HOOK_SRC} -> ${HOOK_DST}")

  # Copy the file to the hooks directory
  file(COPY "${HOOK_SRC}" DESTINATION "${GIT_HOOKS_DIR}")

  # Get the original filename after copying
  get_filename_component(HOOK_NAME_COPIED "${HOOK_SRC}" NAME)

  # Rename the copied file to its final destination name
  file(RENAME "${GIT_HOOKS_DIR}/${HOOK_NAME_COPIED}" "${HOOK_DST}")

  # Make the file executable on Linux/macOS
  if(NOT WIN32)
    execute_process(
      COMMAND chmod +x "${HOOK_DST}"
      RESULT_VARIABLE CHMOD_RESULT
      ERROR_QUIET
    )

    if(NOT CHMOD_RESULT EQUAL 0)
      message(FATAL_ERROR "Failed to make ${HOOK_DST} executable.")
    endif()
  endif()
else()
  message(WARNING "Git hook not found at: ${HOOK_SRC}. Installation skipped.")
endif()
