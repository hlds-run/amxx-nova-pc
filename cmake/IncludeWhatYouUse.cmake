# Enables Include-What-You-Use (IWYU) for a list of targets with the specified options.
#
# @param TARGETS
#   The list of target names to enable IWYU for.
#
# @param CHECK_ALSO
#   [optional] Glob pattern(s) of extra files to check. May be specified multiple times.
#
# @param KEEP
#   [optional] Glob pattern(s) of includes to always keep. May be specified multiple times.
#
# @param MAPPING_FILE
#   [optional] Path to a custom IWYU mapping file.
#
# @param NO_DEFAULT_MAPPINGS
#   [optional] Disables IWYU's built-in default mappings.
#
# @param PCH_IN_CODE
#   [optional] Treats first include as PCH.
#
# @param PREFIX_HEADER_INCLUDES
#   [optional] Behavior for prefix headers (add|keep|remove).
#
# @param TRANSITIVE_INCLUDES_ONLY
#   [optional] Restrict suggestions to already transitively visible includes.
#
# @param MAX_LINE_LENGTH
#   [optional] Max line length for comments/alignment (default 80).
#
# @param COMMENT_STYLE
#   [optional] Style for "why" comments (none|short|long).
#
# @param NO_COMMENTS
#   [optional] Suppress "why" comments entirely.
#
# @param UPDATE_COMMENTS
#   [optional] Force updating "why" comments even if no edits required.
#
# @param NO_FWD_DECLS
#   [optional] Disable forward declarations.
#
# @param VERBOSE
#   [optional] Verbosity level (0..N).
#
# @param QUOTED_INCLUDES_FIRST
#   [optional] Sort quoted includes before angle-bracket ones.
#
# @param CXX17NS
#   [optional] Use C++17 nested namespaces for forward decls.
#
# @param ERROR
#   [optional] Exit code for violations (default 1).
#
# @param ERROR_ALWAYS
#   [optional] Always exit with code for violations.
#
# @param REGEX
#   [optional] Regex dialect for IWYU (llvm|ecmascript).
#
# @param EXPERIMENTAL
#   [optional] List of experimental flags.
function(enable_include_what_you_use)
  cmake_parse_arguments(ARG
    "NO_DEFAULT_MAPPINGS;PCH_IN_CODE;TRANSITIVE_INCLUDES_ONLY;NO_COMMENTS;UPDATE_COMMENTS;NO_FWD_DECLS;QUOTED_INCLUDES_FIRST;CXX17NS"
    "MAPPING_FILE;PREFIX_HEADER_INCLUDES;MAX_LINE_LENGTH;COMMENT_STYLE;VERBOSE;ERROR;ERROR_ALWAYS;REGEX"
    "TARGETS;CHECK_ALSO;KEEP;EXPERIMENTAL"
    ${ARGN}
  )

  if(NOT ARG_TARGETS)
    message(FATAL_ERROR "TARGETS argument is required.")
  endif()

  # Build base IWYU options (must all be passed with -Xiwyu)
  set(iwyu_opts "")

  foreach(glob IN LISTS ARG_CHECK_ALSO)
    list(APPEND iwyu_opts "-Xiwyu" "--check_also=${glob}")
  endforeach()

  foreach(glob IN LISTS ARG_KEEP)
    list(APPEND iwyu_opts "-Xiwyu" "--keep=${glob}")
  endforeach()

  if(DEFINED ARG_MAPPING_FILE)
    list(APPEND iwyu_opts "-Xiwyu" "--mapping_file=${ARG_MAPPING_FILE}")
  endif()

  if(ARG_NO_DEFAULT_MAPPINGS)
    list(APPEND iwyu_opts "-Xiwyu" "--no_default_mappings")
  endif()

  if(ARG_PCH_IN_CODE)
    list(APPEND iwyu_opts "-Xiwyu" "--pch_in_code")
  endif()

  if(DEFINED ARG_PREFIX_HEADER_INCLUDES)
    list(APPEND iwyu_opts "-Xiwyu" "--prefix_header_includes=${ARG_PREFIX_HEADER_INCLUDES}")
  endif()

  if(ARG_TRANSITIVE_INCLUDES_ONLY)
    list(APPEND iwyu_opts "-Xiwyu" "--transitive_includes_only")
  endif()

  if(DEFINED ARG_MAX_LINE_LENGTH)
    list(APPEND iwyu_opts "-Xiwyu" "--max_line_length=${ARG_MAX_LINE_LENGTH}")
  endif()

  if(DEFINED ARG_COMMENT_STYLE)
    list(APPEND iwyu_opts "-Xiwyu" "--comment_style=${ARG_COMMENT_STYLE}")
  endif()

  if(ARG_NO_COMMENTS)
    list(APPEND iwyu_opts "-Xiwyu" "--no_comments")
  endif()

  if(ARG_UPDATE_COMMENTS)
    list(APPEND iwyu_opts "-Xiwyu" "--update_comments")
  endif()

  if(ARG_NO_FWD_DECLS)
    list(APPEND iwyu_opts "-Xiwyu" "--no_fwd_decls")
  endif()

  if(DEFINED ARG_VERBOSE)
    list(APPEND iwyu_opts "-Xiwyu" "--verbose=${ARG_VERBOSE}")
  endif()

  if(ARG_QUOTED_INCLUDES_FIRST)
    list(APPEND iwyu_opts "-Xiwyu" "--quoted_includes_first")
  endif()

  if(ARG_CXX17NS)
    list(APPEND iwyu_opts "-Xiwyu" "--cxx17ns")
  endif()

  if(DEFINED ARG_ERROR)
    list(APPEND iwyu_opts "-Xiwyu" "--error=${ARG_ERROR}")
  endif()

  if(DEFINED ARG_ERROR_ALWAYS)
    list(APPEND iwyu_opts "-Xiwyu" "--error_always=${ARG_ERROR_ALWAYS}")
  endif()

  if(DEFINED ARG_REGEX)
    list(APPEND iwyu_opts "-Xiwyu" "--regex=${ARG_REGEX}")
  endif()

  foreach(flag IN LISTS ARG_EXPERIMENTAL)
    list(APPEND iwyu_opts "-Xiwyu" "--experimental=${flag}")
  endforeach()

  # Find IWYU executable
  find_program(iwyu_exe
    NAMES "include-what-you-use" "iwyu"
    HINTS "$ENV{PROGRAMFILES}/LLVM/bin" "$ENV{PROGRAMFILES}/IWYU" "$ENV{PROGRAMFILES}/Include What You Use"
  )

  if(NOT iwyu_exe)
    message(WARNING "Include-What-You-Use not found")
    return()
  endif()

  foreach(target_name IN LISTS ARG_TARGETS)
    message(STATUS "Enabling Include-What-You-Use for target \"${target_name}\"")

    if(iwyu_opts)
      set(iwyu_cmd "${iwyu_exe};${iwyu_opts}")
    else()
      set(iwyu_cmd "${iwyu_exe}")
    endif()

    set_target_properties("${target_name}" PROPERTIES
      C_INCLUDE_WHAT_YOU_USE "${iwyu_cmd}"
      CXX_INCLUDE_WHAT_YOU_USE "${iwyu_cmd}"
    )
  endforeach()
endfunction()
