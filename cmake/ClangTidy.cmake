# Enables Clang-Tidy for a list of targets with the specified options.
#
# @param TARGETS
#   The list of target names to enable Clang-Tidy for.
#
# @param ALLOW_NO_CHECKS
#   [optional] Permits running Clang-Tidy without any enabled checks, suppressing the "no checks enabled" error.
#
# @param CHECKS
#   [optional] A comma-separated list of check names to enable or disable (with '-' prefix to disable),
#   appended to checks in the .clang-tidy file.
#
# @param CONFIG
#   [optional] Specifies a YAML/JSON configuration string for Clang-Tidy, e.g., "{Checks: '*', CheckOptions: {x: y}}".
#   If empty, Clang-Tidy searches for a .clang-tidy file in parent directories.
#
# @param CONFIG_FILE
#   [optional] Path to a specific .clang-tidy configuration file. Cannot be used with CONFIG.
#
# @param ENABLE_CHECK_PROFILE
#   [optional] Enables timing profiles for each check and prints a report to stderr.
#
# @param ENABLE_MODULE_HEADERS_PARSING
#   [optional] Enables experimental parsing of module headers for C++20 and above,
#   allowing checks to detect macro definitions (may impact performance).
#
# @param EXCLUDE_HEADER_FILTER
#   [optional] A regular expression to exclude diagnostics from specific headers. Must be used with HEADER_FILTER.
#
# @param EXPORT_FIXES
#   [optional] Specifies a YAML file to store suggested code fixes, which can be applied using clang-apply-replacements.
#
# @param EXTRA_ARG
#   [optional] List of additional arguments to append to the compiler command line.
#
# @param EXTRA_ARG_BEFORE
#   [optional] List of additional arguments to prepend to the compiler command line.
#
# @param FIX
#   [optional] Applies suggested fixes to the code, but stops if compilation errors are found (unless FIX_ERRORS is enabled).
#
# @param FIX_ERRORS
#   [optional] Applies suggested fixes even if compilation errors are found, including fixes attached to compiler errors.
#
# @param FIX_NOTES
#   [optional] Applies fixes from diagnostic notes if no direct fix is available for a warning. Implies FIX.
#
# @param FORMAT_STYLE
#   [optional] Defines the code formatting style for applied fixes
#   (e.g., 'none', 'file', 'llvm', 'google', or a JSON string like '{BasedOnStyle: llvm, IndentWidth: 8}').
#
# @param HEADER_FILTER
#   [optional] A regular expression to include diagnostics from specific headers
#   (main file diagnostics are always included).
#
# @param LINE_FILTER
#   [optional] A JSON array specifying files and line ranges to filter warnings,
#   e.g., '[{"name":"file1.cpp","lines":[[1,3],[5,7]]}]'.
#
# @param LIST_CHECKS
#   [optional] Lists all enabled checks and exits. Use with CHECKS='*' to list all available checks.
#
# @param LOAD
#   [optional] List of plugin filenames to load into Clang-Tidy.
#
# @param BUILD_DIR
#   [optional] Path to the build directory containing compile_commands.json for compilation database access.
#
# @param QUIET
#   [optional] Suppresses printing statistics about ignored warnings or warnings treated as errors.
#
# @param STORE_CHECK_PROFILE
#   [optional] Stores per-translation-unit timing profiles as JSON files
#   with the specified prefix instead of printing to stderr.
#
# @param SYSTEM_HEADERS
#   [optional] Enables displaying diagnostics from system headers, overriding the .clang-tidy file setting.
#
# @param USE_COLOR
#   [optional] Enables colored diagnostic output if the terminal supports it, overriding the .clang-tidy file setting.
#
# @param VFSOVERLAY
#   [optional] Specifies a file describing a virtual filesystem to overlay on the real filesystem.
#
# @param WARNINGS_AS_ERRORS
#   [optional] A comma-separated list of check names to treat warnings as errors,
#   appended to the .clang-tidy file setting.
function(enable_clang_tidy)
  # Parse the arguments passed to the function
  cmake_parse_arguments(ARG
    "ALLOW_NO_CHECKS;ENABLE_CHECK_PROFILE;ENABLE_MODULE_HEADERS_PARSING;FIX;FIX_ERRORS;FIX_NOTES;LIST_CHECKS;QUIET;SYSTEM_HEADERS;USE_COLOR"
    "CHECKS;CONFIG;CONFIG_FILE;EXCLUDE_HEADER_FILTER;EXPORT_FIXES;FORMAT_STYLE;HEADER_FILTER;LINE_FILTER;BUILD_DIR;STORE_CHECK_PROFILE;VFSOVERLAY;WARNINGS_AS_ERRORS"
    "TARGETS;EXTRA_ARG;EXTRA_ARG_BEFORE;LOAD"
    ${ARGN}
  )

  # If TARGETS is not set, print an error message and stop processing
  if(NOT ARG_TARGETS)
    message(FATAL_ERROR "TARGETS argument is required.")
  endif()

  # Build the base options list for Clang-Tidy (excluding extra args)
  set(base_options "")

  if(ARG_ALLOW_NO_CHECKS)
    list(APPEND base_options "--allow-no-checks")
  endif()

  if(DEFINED ARG_CHECKS)
    list(APPEND base_options "--checks=${ARG_CHECKS}")
  endif()

  if(DEFINED ARG_CONFIG)
    list(APPEND base_options "--config=${ARG_CONFIG}")
  endif()

  if(DEFINED ARG_CONFIG_FILE)
    list(APPEND base_options "--config-file=${ARG_CONFIG_FILE}")
  endif()

  if(ARG_ENABLE_CHECK_PROFILE)
    list(APPEND base_options "--enable-check-profile")
  endif()

  if(ARG_ENABLE_MODULE_HEADERS_PARSING)
    list(APPEND base_options "--enable-module-headers-parsing")
  endif()

  if(DEFINED ARG_EXCLUDE_HEADER_FILTER)
    list(APPEND base_options "--exclude-header-filter=${ARG_EXCLUDE_HEADER_FILTER}")
  endif()

  if(DEFINED ARG_EXPORT_FIXES)
    list(APPEND base_options "--export-fixes=${ARG_EXPORT_FIXES}")
  endif()

  if(ARG_FIX)
    list(APPEND base_options "--fix")
  endif()

  if(ARG_FIX_ERRORS)
    list(APPEND base_options "--fix-errors")
  endif()

  if(ARG_FIX_NOTES)
    list(APPEND base_options "--fix-notes")
  endif()

  if(DEFINED ARG_FORMAT_STYLE)
    list(APPEND base_options "--format-style=${ARG_FORMAT_STYLE}")
  endif()

  if(DEFINED ARG_HEADER_FILTER)
    list(APPEND base_options "--header-filter=${ARG_HEADER_FILTER}")
  endif()

  if(DEFINED ARG_LINE_FILTER)
    list(APPEND base_options "--line-filter=${ARG_LINE_FILTER}")
  endif()

  if(ARG_LIST_CHECKS)
    list(APPEND base_options "--list-checks")
  endif()

  foreach(plugin IN LISTS ARG_LOAD)
    list(APPEND base_options "--load=${plugin}")
  endforeach()

  if(DEFINED ARG_BUILD_DIR)
    list(APPEND base_options "-p" "${ARG_BUILD_DIR}")
  endif()

  if(ARG_QUIET)
    list(APPEND base_options "--quiet")
  endif()

  if(DEFINED ARG_STORE_CHECK_PROFILE)
    list(APPEND base_options "--store-check-profile=${ARG_STORE_CHECK_PROFILE}")
  endif()

  if(ARG_SYSTEM_HEADERS)
    list(APPEND base_options "--system-headers")
  endif()

  if(ARG_USE_COLOR)
    list(APPEND base_options "--use-color")
  endif()

  if(DEFINED ARG_VFSOVERLAY)
    list(APPEND base_options "--vfsoverlay=${ARG_VFSOVERLAY}")
  endif()

  if(DEFINED ARG_WARNINGS_AS_ERRORS)
    list(APPEND base_options "--warnings-as-errors=${ARG_WARNINGS_AS_ERRORS}")
  endif()

  # Build extra-arg-before options
  set(extra_before_options "")
  foreach(arg IN LISTS ARG_EXTRA_ARG_BEFORE)
    list(APPEND extra_before_options "--extra-arg-before=${arg}")
  endforeach()

  # Build extra-arg options
  set(extra_append_options "")
  foreach(arg IN LISTS ARG_EXTRA_ARG)
    list(APPEND extra_append_options "--extra-arg=${arg}")
  endforeach()

  # Prepare C-specific extra options
  set(c_extra_options "")
  list(APPEND c_extra_options ${extra_before_options})
  if(NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "" AND NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    list(APPEND c_extra_options "--extra-arg=-Wno-unknown-warning-option" "--extra-arg=-Wno-ignored-optimization-argument")
  endif()
  list(APPEND c_extra_options ${extra_append_options})

  # Prepare CXX-specific extra options
  set(cxx_extra_options "")
  list(APPEND cxx_extra_options ${extra_before_options})
  if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "" AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    list(APPEND cxx_extra_options "--extra-arg=-Wno-unknown-warning-option" "--extra-arg=-Wno-ignored-optimization-argument")
  endif()
  list(APPEND cxx_extra_options ${extra_append_options})

  # Find the clang-tidy executable
  find_program(clang_tidy_exe
    NAMES "clang-tidy" "clang-tidy-19" "clang-tidy-20" "clang-tidy-21" "clang-tidy-22" "clang-tidy-23"
    HINTS "$ENV{PROGRAMFILES}/LLVM/bin"
  )

  # If clang-tidy is not found, print a warning and return
  if(NOT clang_tidy_exe)
    message(WARNING "Clang-Tidy not found")
    return()
  endif()

  # Loop over the list of targets
  foreach(target_name IN LISTS ARG_TARGETS)
    message(STATUS "Enabling Clang-Tidy for target \"${target_name}\"")

    # Collect options for C
    set(c_clang_tidy_options "")
    list(APPEND c_clang_tidy_options ${base_options} ${c_extra_options})
    list(FILTER c_clang_tidy_options EXCLUDE REGEX "^$")

    if(c_clang_tidy_options)
      set(c_clang_tidy_cmd "${clang_tidy_exe};${c_clang_tidy_options}")
    else()
      set(c_clang_tidy_cmd "${clang_tidy_exe}")
    endif()

    # Collect options for C++
    set(cxx_clang_tidy_options "")
    list(APPEND cxx_clang_tidy_options ${base_options} ${cxx_extra_options})
    list(FILTER cxx_clang_tidy_options EXCLUDE REGEX "^$")

    if(cxx_clang_tidy_options)
      set(cxx_clang_tidy_cmd "${clang_tidy_exe};${cxx_clang_tidy_options}")
    else()
      set(cxx_clang_tidy_cmd "${clang_tidy_exe}")
    endif()

    # Apply properties
    set_target_properties("${target_name}" PROPERTIES
      C_CLANG_TIDY "${c_clang_tidy_cmd}"
      CXX_CLANG_TIDY "${cxx_clang_tidy_cmd}"
    )
  endforeach()
endfunction()
