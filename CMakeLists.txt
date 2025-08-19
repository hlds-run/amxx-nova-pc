cmake_minimum_required(VERSION 3.21...3.31)

#-------------------------------------------------------------------------------
# Project Definition
#-------------------------------------------------------------------------------

project(        "AMXX Nova Pawn Compiler"
  VERSION       "1.0.0"
  DESCRIPTION   "AMXX Nova pawn compiler"
  HOMEPAGE_URL  "https://github.com/hlds-run/amxx-nova-pc"
  LANGUAGES     "C" "CXX"
)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------

option(USE_LINKER_GOLD      "Use the Gold linker when compiling with GCC"                       ON )
option(USE_LINKER_LLD       "Use the LLD linker when compiling with Clang"                      ON )
option(LINK_STATIC_MSVC_RT  "Link MSVC runtime library statically"                              OFF)
option(LINK_STATIC_GCC      "Link libgcc library statically"                                    OFF)
option(LINK_STATIC_STDCPP   "Link C++ standard library statically"                              OFF)
option(LINK_LIBCPP          "Link libc++ as the C++ standard library instead of libstdc++"      OFF)
option(ENABLE_RTTI          "Enable support for run-time type information"                      OFF)
option(ENABLE_EXCEPTIONS    "Enable support for exception handling"                             ON )
option(ENABLE_ASAN          "Enable AddressSanitizer"                                           OFF)
option(ENABLE_UBSAN         "Enable UndefinedBehaviorSanitizer"                                 OFF)
option(ENABLE_LINK_TRACE    "Enable linker trace flag (detailed output of the linking process)" OFF)

#-------------------------------------------------------------------------------
# Output Directories
#-------------------------------------------------------------------------------

if(NOT BIN_OUTPUT_DIR)
  set(BIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_CXX_COMPILER_ID}-$<CONFIG>")
endif()

#-------------------------------------------------------------------------------
# System Environment
#-------------------------------------------------------------------------------

include("ProcessorCount")
ProcessorCount(NUM_CORES)

if(NUM_CORES EQUAL 0)
  set(NUM_CORES 4)
endif()

if(PROCESSOR_CORES GREATER 8)
  set(PROCESSOR_CORES 8)
endif()

string(TIMESTAMP CURRENT_YEAR "%Y")

#-------------------------------------------------------------------------------
# CMake Policies
#-------------------------------------------------------------------------------

# Honor visibility properties for all target types
set(CMAKE_POLICY_DEFAULT_CMP0063 NEW)

# INTERPROCEDURAL_OPTIMIZATION is enforced when enabled
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

# target_sources() command converts relative paths to absolute
set(CMAKE_POLICY_DEFAULT_CMP0076 NEW)

# option() honors normal variables
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Control generation of Position Independent Executable (PIE)
set(CMAKE_POLICY_DEFAULT_CMP0083 NEW)

# MSVC runtime library flags are selected by an abstraction (CMAKE_MSVC_RUNTIME_LIBRARY)
set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)

# MSVC warning flags are not in CMAKE_<LANG>_FLAGS by default
set(CMAKE_POLICY_DEFAULT_CMP0092 NEW)

# Link properties are transitive over private dependencies of static libraries
set(CMAKE_POLICY_DEFAULT_CMP0099 NEW)

# Makefile generators do not repeat custom commands from target dependencies
set(CMAKE_POLICY_DEFAULT_CMP0113 NEW)

# ExternalProject step targets fully adopt their steps
set(CMAKE_POLICY_DEFAULT_CMP0114 NEW)

# Ninja generators transform DEPFILEs from add_custom_command()
set(CMAKE_POLICY_DEFAULT_CMP0116 NEW)

# MSVC RTTI flag /GR is not added to CMAKE_CXX_FLAGS by default
set(CMAKE_POLICY_DEFAULT_CMP0117 NEW)

# LANGUAGE source file property explicitly compiles as language
set(CMAKE_POLICY_DEFAULT_CMP0119 NEW)

#-------------------------------------------------------------------------------
# Build Configuration
#-------------------------------------------------------------------------------

# Building shared libraries instead of static by default
set(BUILD_SHARED_LIBS OFF)

# First search using Config mode before falling back to Module mode
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

# Set the possible build types
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release" "RelWithDebInfo")

# Append the relative path to the RPATH list
list(APPEND CMAKE_BUILD_RPATH "$ORIGIN/.")
list(REMOVE_DUPLICATES CMAKE_BUILD_RPATH)

# Set the C language standard
set(CMAKE_C_STANDARD 11)

# Enforce the use of the C language standard
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set the visibility of symbols in C object files
set(CMAKE_C_VISIBILITY_PRESET hidden)

# Set the C++ language standard
set(CMAKE_CXX_STANDARD 23)

# Enforce the use of the C++ language standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the visibility of symbols in C++ object files
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Interprocedural optimization for specified build types
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)

# Dependency optimization for faster builds
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

# Position independent code generation
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Hiding of inline functions in shared libraries
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Export the compile commands to a JSON file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable error reporting for deprecated features
set(CMAKE_ERROR_DEPRECATED ON)

#-------------------------------------------------------------------------------
# CMake Modules and Dependencies
#-------------------------------------------------------------------------------

include("FetchContent")
include("cmake/CompileOptions.cmake")
include("cmake/SetupGitHooks.cmake")

if(UNIX)
  find_package(Threads REQUIRED)
endif()

#-------------------------------------------------------------------------------
# Subdirectories
#-------------------------------------------------------------------------------

add_subdirectory("external/zlib")
add_subdirectory("libs/amxxpc32")
add_subdirectory("apps/amxxpc")
