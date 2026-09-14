# Adapted from ~/Documents/interview/cmake/nix-toolchain.cmake.
include_guard(GLOBAL)
find_program(CMAKE_CXX_COMPILER clang++ REQUIRED)

# clang-scan-deps does not execute Nix's compiler wrapper. Make the wrapper's
# header search order explicit for both scanning and compilation.
execute_process(COMMAND "${CMAKE_CXX_COMPILER}" -x c++ -E -v /dev/null
  OUTPUT_QUIET ERROR_VARIABLE _verbose COMMAND_ERROR_IS_FATAL ANY)
string(REGEX REPLACE
  ".*#include <\\.\\.\\.> search starts here:\n(.*)\nEnd of search list\\..*"
  "\\1" _search_block "${_verbose}")
if(_search_block STREQUAL _verbose)
  message(FATAL_ERROR "Cannot discover Nix compiler include paths.")
endif()
string(REPLACE "\n" ";" _dirs "${_search_block}")
foreach(_dir IN LISTS _dirs)
  string(STRIP "${_dir}" _dir)
  if(IS_DIRECTORY "${_dir}")
    string(APPEND CMAKE_CXX_FLAGS_INIT " -isystem \"${_dir}\"")
  endif()
endforeach()

# glibc fortify overloads have internal linkage and cannot be exported by std.
# Apply the same module-compatible hardening as the interview sample.
set(CMAKE_CXX_COMPILER_LAUNCHER
  "env;NIX_HARDENING_ENABLE=bindnow format pic relro stackprotector strictoverflow")

if(NOT CMAKE_CXX_STDLIB_MODULES_JSON)
  set(CMAKE_CXX_STDLIB_MODULES_JSON "$ENV{NIX_CXX_STDLIB_MODULES_JSON}"
    CACHE FILEPATH "libc++ module manifest")
endif()
if(NOT EXISTS "${CMAKE_CXX_STDLIB_MODULES_JSON}")
  message(FATAL_ERROR "Missing libc++ module manifest. Configure inside nix develop.")
endif()
list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES CMAKE_CXX_STDLIB_MODULES_JSON)
