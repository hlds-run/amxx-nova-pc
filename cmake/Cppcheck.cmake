# Enables Cppcheck for a list of targets with all configurable options from the Cppcheck reference.
#
# @param TARGETS
#   The list of target names to enable Cppcheck for.
#
# @param ADDON
#   [optional] List of addons to execute (e.g., "misra").
#
# @param ADDON_PYTHON
#   [optional] Python interpreter for addons.
#
# @param CPPCHECK_BUILD_DIR
#   [optional] Base Cppcheck work folder (per-target subdirs will be created).
#
# @param CHECK_CONFIG
#   [optional] Check Cppcheck configuration (boolean).
#
# @param CHECK_LEVEL
#   [optional] Analysis level: reduced, normal, exhaustive.
#
# @param CHECK_LIBRARY
#   [optional] Show library incomplete info (boolean).
#
# @param CHECKERS_REPORT
#   [optional] File for active checkers report.
#
# @param CLANG
#   [optional] Path to Clang executable.
#
# @param CONFIG_EXCLUDE
#   [optional] List of paths to exclude from config checking.
#
# @param CONFIG_EXCLUDES_FILE
#   [optional] File with config excludes.
#
# @param DISABLE
#   [optional] List of checks to disable.
#
# @param DEFINE
#   [optional] List of preprocessor defines (e.g., "DEBUG=1").
#
# @param PREPROCESS_ONLY
#   [optional] Print preprocessor output (boolean, -E).
#
# @param ENABLE
#   [optional] List of checks to enable (e.g., "all").
#
# @param ERROR_EXITCODE
#   [optional] Exit code on errors.
#
# @param ERRORLIST
#   [optional] Print error list in XML (boolean).
#
# @param EXITCODE_SUPPRESSIONS
#   [optional] File for suppressions not affecting exit code.
#
# @param FILE_FILTER
#   [optional] List of file filters.
#
# @param FILE_LIST
#   [optional] File with list of files to check.
#
# @param FORCE
#   [optional] Force checking of all configurations in files (boolean).
#
# @param FSIGNED_CHAR
#   [optional] Treat char as signed (boolean).
#
# @param FUNSIGNED_CHAR
#   [optional] Treat char as unsigned (boolean).
#
# @param INCLUDE_PATH
#   [optional] List of include paths (-I).
#
# @param INCLUDES_FILE
#   [optional] File with include paths.
#
# @param INCLUDE
#   [optional] List of files to force include.
#
# @param IGNORE
#   [optional] List of files/directories to ignore (-i).
#
# @param INCONCLUSIVE
#   [optional] Allow inconclusive results (boolean).
#
# @param INLINE_SUPPR
#   [optional] Enable inline suppressions (boolean).
#
# @param JOBS
#   [optional] Number of threads (-j).
#
# @param LOAD_AVERAGE
#   [optional] Load average limit (-l).
#
# @param LANGUAGE
#   [optional] Forces cppcheck to check all files as the given language (c, c++).
#
# @param LIBRARY
#   [optional] List of library configs.
#
# @param MAX_CONFIGS
#   [optional] Max configurations.
#
# @param MAX_CTU_DEPTH
#   [optional] Max whole program depth.
#
# @param OUTPUT_FILE
#   [optional] File for results.
#
# @param OUTPUT_FORMAT
#   [optional] Format: text, sarif, xml.
#
# @param PLATFORM
#   [optional] Platform type.
#
# @param PLIST_OUTPUT
#   [optional] Path for Clang-plist files.
#
# @param PROJECT
#   [optional] Project file (Visual Studio Solution, compile_commands.json).
#
# @param PROJECT_CONFIGURATION
#   [optional] Config for Visual Studio projects.
#
# @param QUIET
#   [optional] Do not show progress reports (boolean).
#
# @param RELATIVE_PATHS
#   [optional] Base paths for relative output.
#
# @param REPORT_PROGRESS
#   [optional] Report progress (boolean).
#
# @param REPORT_TYPE
#   [optional] Type for guideline fields (e.g., misra-c-2012).
#
# @param RULE
#   [optional] Regular expression rule.
#
# @param RULE_FILE
#   [optional] File with rules.
#
# @param SAFETY
#   [optional] Safety-certified mode (boolean).
#
# @param SHOWTIME
#   [optional] Timing mode: none, file, summary, etc.
#
# @param STD
#   [optional] Standard (overridden by target properties if not set).
#
# @param SUPPRESS
#   [optional] List of suppression specs.
#
# @param SUPPRESSIONS_LIST
#   [optional] File with suppressions.
#
# @param SUPPRESS_XML
#   [optional] XML file for suppressions.
#
# @param TEMPLATE
#   [optional] Error message template.
#
# @param TEMPLATE_LOCATION
#   [optional] Location template.
#
# @param UNDEFINE
#   [optional] List of preprocessor undefines (-U).
#
# @param VERBOSE
#   [optional] Verbose output (boolean).
#
# @param XML
#   [optional] XML output (boolean).
function(enable_cppcheck)
  # Parse the arguments passed to the function
  cmake_parse_arguments(
    "ARG"
    "CHECK_CONFIG;CHECK_LIBRARY;PREPROCESS_ONLY;FORCE;FSIGNED_CHAR;FUNSIGNED_CHAR;INCONCLUSIVE;INLINE_SUPPR;QUIET;REPORT_PROGRESS;SAFETY;VERBOSE;XML;ERRORLIST"
    "ADDON_PYTHON;CPPCHECK_BUILD_DIR;CHECK_LEVEL;CHECKERS_REPORT;CLANG;CONFIG_EXCLUDES_FILE;ERROR_EXITCODE;EXITCODE_SUPPRESSIONS;FILE_LIST;INCLUDES_FILE;JOBS;LOAD_AVERAGE;LANGUAGE;MAX_CONFIGS;MAX_CTU_DEPTH;OUTPUT_FILE;OUTPUT_FORMAT;PLATFORM;PLIST_OUTPUT;PROJECT;PROJECT_CONFIGURATION;RELATIVE_PATHS;REPORT_TYPE;RULE;RULE_FILE;SHOWTIME;STD;TEMPLATE;TEMPLATE_LOCATION;SUPPRESSIONS_LIST;SUPPRESS_XML"
    "TARGETS;ADDON;CONFIG_EXCLUDE;DISABLE;DEFINE;ENABLE;FILE_FILTER;INCLUDE_PATH;INCLUDES;IGNORE;LIBRARY;SUPPRESS;UNDEFINE"
    ${ARGN}
  )

  # If TARGETS is not set, print an error message and stop processing
  if(NOT ARG_TARGETS)
    message(FATAL_ERROR "TARGETS argument is required.")
  endif()

  # Set number of jobs if not specified
  if(NOT ARG_JOBS)
    if(NOT ARG_JOBS)
      set(ARG_JOBS ${NUM_CORES})
    endif()
  endif()

  # If PLATFORM is not set, determine based on system
  if(NOT ARG_PLATFORM)
    if(WIN32)
      set(ARG_PLATFORM "win32A")
    else()
      set(ARG_PLATFORM "unix32")
    endif()
  endif()

  # If ENABLE is not set, default to "all"
  if(NOT ARG_ENABLE)
    set(ARG_ENABLE "all")
  endif()

  # If LIBRARY is not set, default to common libraries
  if(NOT ARG_LIBRARY)
    set(ARG_LIBRARY "boost,gnu,googletest,libcurl,posix,qt,sdl,std,windows,zlib")
  endif()

  # If MAX_CTU_DEPTH is not set, default to 10
  if(NOT ARG_MAX_CTU_DEPTH)
    set(ARG_MAX_CTU_DEPTH "10")
  endif()

  # If MAX_CONFIGS is not set, default to 50
  if(NOT ARG_MAX_CONFIGS)
    set(ARG_MAX_CONFIGS "50")
  endif()

  # If QUIET is not set, default to ON
  if(NOT DEFINED ARG_QUIET)
    set(ARG_QUIET ON)
  endif()

  # If INLINE_SUPPR is not set, default to ON
  if(NOT DEFINED ARG_INLINE_SUPPR)
    set(ARG_INLINE_SUPPR ON)
  endif()

  # Find the cppcheck executable
  find_program(cppcheck_exe
    NAMES "cppcheck"
    HINTS "$ENV{PROGRAMFILES}/Cppcheck"
  )

  # If cppcheck is not found, print a warning and return
  if(NOT cppcheck_exe)
    message(WARNING "Cppcheck not found")
    return()
  endif()

  # Start building the cppcheck command list
  set(cppcheck_base
    "${cppcheck_exe}"
    "-j${ARG_JOBS}" # Number of threads
  )

  # Add single-value options if provided
  if(ARG_ADDON_PYTHON)
    list(APPEND cppcheck_base "--addon-python=${ARG_ADDON_PYTHON}")
  endif()
  if(ARG_CPPCHECK_BUILD_DIR)
    set(build_dir_base "${ARG_CPPCHECK_BUILD_DIR}")
  else()
    set(build_dir_base "${PROJECT_BINARY_DIR}/Cppcheck")
  endif()
  if(ARG_CHECK_LEVEL)
    list(APPEND cppcheck_base "--check-level=${ARG_CHECK_LEVEL}")
  endif()
  if(ARG_CHECKERS_REPORT)
    list(APPEND cppcheck_base "--checkers-report=${ARG_CHECKERS_REPORT}")
  endif()
  if(ARG_CLANG)
    list(APPEND cppcheck_base "--clang=${ARG_CLANG}")
  endif()
  if(ARG_CONFIG_EXCLUDES_FILE)
    list(APPEND cppcheck_base "--config-excludes-file=${ARG_CONFIG_EXCLUDES_FILE}")
  endif()
  if(ARG_ERROR_EXITCODE)
    list(APPEND cppcheck_base "--error-exitcode=${ARG_ERROR_EXITCODE}")
  endif()
  if(ARG_EXITCODE_SUPPRESSIONS)
    list(APPEND cppcheck_base "--exitcode-suppressions=${ARG_EXITCODE_SUPPRESSIONS}")
  endif()
  if(ARG_FILE_LIST)
    list(APPEND cppcheck_base "--file-list=${ARG_FILE_LIST}")
  endif()
  if(ARG_INCLUDES_FILE)
    list(APPEND cppcheck_base "--includes-file=${ARG_INCLUDES_FILE}")
  endif()
  if(ARG_LOAD_AVERAGE)
    list(APPEND cppcheck_base "-l ${ARG_LOAD_AVERAGE}")
  endif()
  if(ARG_LANGUAGE)
    list(APPEND cppcheck_base "--language=${ARG_LANGUAGE}")
  endif()
  if(ARG_MAX_CONFIGS)
    list(APPEND cppcheck_base "--max-configs=${ARG_MAX_CONFIGS}")
  endif()
  if(ARG_MAX_CTU_DEPTH)
    list(APPEND cppcheck_base "--max-ctu-depth=${ARG_MAX_CTU_DEPTH}")
  endif()
  if(ARG_OUTPUT_FILE)
    list(APPEND cppcheck_base "--output-file=${ARG_OUTPUT_FILE}")
  endif()
  if(ARG_OUTPUT_FORMAT)
    list(APPEND cppcheck_base "--output-format=${ARG_OUTPUT_FORMAT}")
  endif()
  if(ARG_PLATFORM)
    list(APPEND cppcheck_base "--platform=${ARG_PLATFORM}")
  endif()
  if(ARG_PLIST_OUTPUT)
    list(APPEND cppcheck_base "--plist-output=${ARG_PLIST_OUTPUT}")
  endif()
  if(ARG_PROJECT)
    list(APPEND cppcheck_base "--project=${ARG_PROJECT}")
  endif()
  if(ARG_PROJECT_CONFIGURATION)
    list(APPEND cppcheck_base "--project-configuration=${ARG_PROJECT_CONFIGURATION}")
  endif()
  if(ARG_RELATIVE_PATHS)
    list(APPEND cppcheck_base "--relative-paths=${ARG_RELATIVE_PATHS}")
  endif()
  if(ARG_REPORT_TYPE)
    list(APPEND cppcheck_base "--report-type=${ARG_REPORT_TYPE}")
  endif()
  if(ARG_RULE)
    list(APPEND cppcheck_base "--rule=${ARG_RULE}")
  endif()
  if(ARG_RULE_FILE)
    list(APPEND cppcheck_base "--rule-file=${ARG_RULE_FILE}")
  endif()
  if(ARG_SHOWTIME)
    list(APPEND cppcheck_base "--showtime=${ARG_SHOWTIME}")
  endif()
  if(ARG_STD)
    set(custom_std "${ARG_STD}")
  endif()
  if(ARG_SUPPRESSIONS_LIST)
    list(APPEND cppcheck_base "--suppressions-list=${ARG_SUPPRESSIONS_LIST}")
  endif()
  if(ARG_SUPPRESS_XML)
    list(APPEND cppcheck_base "--suppress-xml=${ARG_SUPPRESS_XML}")
  endif()
  if(ARG_TEMPLATE)
    list(APPEND cppcheck_base "--template=${ARG_TEMPLATE}")
  endif()
  if(ARG_TEMPLATE_LOCATION)
    list(APPEND cppcheck_base "--template-location=${ARG_TEMPLATE_LOCATION}")
  endif()

  # Add boolean flags if enabled
  if(ARG_CHECK_CONFIG)
    list(APPEND cppcheck_base "--check-config")
  endif()
  if(ARG_CHECK_LIBRARY)
    list(APPEND cppcheck_base "--check-library")
  endif()
  if(ARG_PREPROCESS_ONLY)
    list(APPEND cppcheck_base "-E")
  endif()
  if(ARG_FORCE)
    list(APPEND cppcheck_base "--force")
  endif()
  if(ARG_FSIGNED_CHAR)
    list(APPEND cppcheck_base "--fsigned-char")
  endif()
  if(ARG_FUNSIGNED_CHAR)
    list(APPEND cppcheck_base "--funsigned-char")
  endif()
  if(ARG_INCONCLUSIVE)
    list(APPEND cppcheck_base "--inconclusive")
  endif()
  if(ARG_INLINE_SUPPR)
    list(APPEND cppcheck_base "--inline-suppr")
  endif()
  if(ARG_QUIET)
    list(APPEND cppcheck_base "--quiet")
  endif()
  if(ARG_REPORT_PROGRESS)
    list(APPEND cppcheck_base "--report-progress")
  endif()
  if(ARG_SAFETY)
    list(APPEND cppcheck_base "--safety")
  endif()
  if(ARG_VERBOSE)
    list(APPEND cppcheck_base "--verbose")
  endif()
  if(ARG_XML)
    list(APPEND cppcheck_base "--xml")
  endif()
  if(ARG_ERRORLIST)
    list(APPEND cppcheck_base "--errorlist")
  endif()

  # Add multi-value options
  foreach(addon IN LISTS ARG_ADDON)
    list(APPEND cppcheck_base "--addon=${addon}")
  endforeach()
  foreach(exclude IN LISTS ARG_CONFIG_EXCLUDE)
    list(APPEND cppcheck_base "--config-exclude=${exclude}")
  endforeach()
  foreach(disable IN LISTS ARG_DISABLE)
    list(APPEND cppcheck_base "--disable=${disable}")
  endforeach()
  foreach(define IN LISTS ARG_DEFINE)
    list(APPEND cppcheck_base "-D${define}")
  endforeach()
  foreach(enable IN LISTS ARG_ENABLE)
    list(APPEND cppcheck_base "--enable=${enable}")
  endforeach()
  foreach(filter IN LISTS ARG_FILE_FILTER)
    list(APPEND cppcheck_base "--file-filter=${filter}")
  endforeach()
  foreach(inc_path IN LISTS ARG_INCLUDE_PATH)
    list(APPEND cppcheck_base "-I ${inc_path}")
  endforeach()
  foreach(include IN LISTS ARG_INCLUDE)
    list(APPEND cppcheck_base "--include=${include}")
  endforeach()
  foreach(ignore IN LISTS ARG_IGNORE)
    list(APPEND cppcheck_base "-i ${ignore}")
  endforeach()
  foreach(library IN LISTS ARG_LIBRARY)
    list(APPEND cppcheck_base "--library=${library}")
  endforeach()
  foreach(suppress IN LISTS ARG_SUPPRESS)
    list(APPEND cppcheck_base "--suppress=${suppress}")
  endforeach()
  foreach(undef IN LISTS ARG_UNDEFINE)
    list(APPEND cppcheck_base "-U${undef}")
  endforeach()

  # Add project-wide suppression file if exists
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.cppcheck")
    list(APPEND cppcheck_base "--suppressions-list=${CMAKE_CURRENT_SOURCE_DIR}/.cppcheck")
  elseif(EXISTS "${CMAKE_SOURCE_DIR}/.cppcheck")
    list(APPEND cppcheck_base "--suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck")
  endif()

  # Add project source dir if exists
  set(src_dir "${PROJECT_SOURCE_DIR}/src")
  if(EXISTS "${src_dir}")
    list(APPEND cppcheck_base "-I${src_dir}")
  endif()

  # Loop over the list of targets
  foreach(target_name IN LISTS ARG_TARGETS)
    message(STATUS "Enabling Cppcheck for target \"${target_name}\"")

    # Set per-target build dir
    set(cppcheck_build_dir "${build_dir_base}/${target_name}")
    file(MAKE_DIRECTORY "${cppcheck_build_dir}")

    # Copy base command and add per-target options
    set(cppcheck_exe_target ${cppcheck_base})
    list(APPEND cppcheck_exe_target "--cppcheck-build-dir=${cppcheck_build_dir}")

    # Get target-specific include directories
    get_property(target_includes TARGET "${target_name}" PROPERTY INCLUDE_DIRECTORIES)
    if(target_includes)
      foreach(inc_dir IN LISTS target_includes)
        if(EXISTS "${inc_dir}")
          list(APPEND cppcheck_exe_target "-I${inc_dir}")
        endif()
      endforeach()
    endif()

    # Set C and C++ commands
    set(cppcheck_exe_target_c ${cppcheck_exe_target})
    set(cppcheck_exe_target_cpp ${cppcheck_exe_target})

    # Set standards from target properties or custom
    if(ARG_STD)
      list(APPEND cppcheck_exe_target_c "--std=${ARG_STD}")
      list(APPEND cppcheck_exe_target_cpp "--std=${ARG_STD}")
    else()
      if(DEFINED CMAKE_C_STANDARD)
        list(APPEND cppcheck_exe_target_c "--std=c${CMAKE_C_STANDARD}")
      endif()
      if(DEFINED CMAKE_CXX_STANDARD)
        list(APPEND cppcheck_exe_target_cpp "--std=c++${CMAKE_CXX_STANDARD}")
      endif()
    endif()

    # Set properties for the target
    set_target_properties("${target_name}" PROPERTIES
      C_CPPCHECK "${cppcheck_exe_target_c}"
      CXX_CPPCHECK "${cppcheck_exe_target_cpp}"
    )
  endforeach()
endfunction()
