 * `target_enable_cxx_modules` - enable C++ modules support for project. It just set compiler options for C++ modules.
 * `add_module_library` - same as CMake's `add_library` but generate C++ module interface files from given source
   files and store that in `CXX_MODULES_INTERFACE_FILES` target property and set `CXX_MODULES_INTERFACE_TARGETS` which contains
   target names for creating interface files.
 * `add_module_executable` - same as `add_executable` but enable C++ modules support.
 * `target_link_module_libraries` - same as `target_link_libraries` but generate C++ modules interface files import
   flags for target.