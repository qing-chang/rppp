############################################################################
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    #Clang
    set(CXX_MODULES_CHECK -fmodules-ts)
    set(CXX_MODULES_FLAGS -fmodules-ts)
    set(CXX_MODULES_EXT pcm)
    set(CXX_MODULES_CREATE_FLAGS -fmodules-ts -x c++-module --precompile)
    set(CXX_MODULES_USE_FLAG -fmodule-file=)
    set(CXX_MODULES_OUTPUT_FLAG -o)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #GCC
    message(FATAL_ERROR "GCC is not supported yet")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    #MSVC
    set(CXX_MODULES_CHECK /experimental:module)
    set(CXX_MODULES_FLAGS /experimental:module /module:interface)
    set(CXX_MODULES_EXT ifc)
    set(CXX_MODULES_CREATE_FLAGS -c)
    set(CXX_MODULES_USE_FLAG /module:reference )
    set(CXX_MODULES_OUTPUT_FLAG /module:output)
else ()
    message(FATAL_ERROR "Unsupported compiler")
endif ()
############################################################################
#Compiler flags support tests
include(CheckCXXCompilerFlag)
include(CMakePushCheckState)
#Check if used compiler version supports modules
check_cxx_compiler_flag(${CXX_MODULES_CHECK} CXX_MODULES)
############################################################################
##Check if current compiler supports C++ modules. If compiler doesn't support
##modules it fails with fatal error.
function (_check_cxx_modules_support)
    if (NOT CXX_MODULES)
        message(FATAL_ERROR "Compiler doesn't support C++ modules (TS)")
    endif ()
endfunction ()
############################################################################
##Enable C++ modules for project.
##This function adds appropriate compiler flags to the target.
function (target_enable_cxx_modules TARGET)
    _check_cxx_modules_support()

    #Add modules flag
    target_compile_options(${TARGET} PRIVATE ${CXX_MODULES_FLAGS})
endfunction ()
############################################################################
##Create an executable with C++ support
function (add_module_executable TARGET)
    _check_cxx_modules_support()

    add_executable(${TARGET} ${ARGN})

    #Enable modules for target
    target_enable_cxx_modules(${TARGET})
endfunction ()
############################################################################
##Create C++ module library.
##Sets target property CXX_MODULES_INTERFACE_FILES and CXX_MODULES_INTERFACE_TARGETS
function (add_module_library TARGET)
    _check_cxx_modules_support()

    #Get sources
    set(_sources)

    #Filter source files
    foreach (_arg ${ARGN})
        list(FIND "STATIC;SHARED;MODULE;EXCLUDE_FROM_ALL;OBJECT;UNKNOWN;IMPORTED" ${_arg} _skip)

        if (${_skip} GREATER_EQUAL 0)
            continue ()
        endif ()

        if (${_arg} MATCHES "ALIAS")
            message(FATAL_ERROR "Alias library is not supported")
        endif ()

        #TODO: limit sources extensions?

        list(APPEND _sources ${_arg})
    endforeach ()

    #Allow to use CXX compiler on C++ module files
    set_source_files_properties(${_sources} PROPERTIES LANGUAGE CXX)

    #Create normal library
    add_library(${TARGET} ${ARGN})

    #Enable modules for target
    target_enable_cxx_modules(${TARGET})

    set(_interface_files)
    set(_interface_targets)

    #Create targets for interface files
    foreach (_source ${_sources})
        set(_o_file ${_source}.${CXX_MODULES_EXT})
        set(_i_file ${CMAKE_CURRENT_SOURCE_DIR}/${_source})

        #TODO: CXX flags might be different
        set(_cmd ${CMAKE_CXX_COMPILER} "$<JOIN:$<TARGET_PROPERTY:${TARGET},COMPILE_OPTIONS>,\t>" ${CXX_MODULES_CREATE_FLAGS} ${_i_file} ${CXX_MODULES_OUTPUT_FLAG} ${_o_file})

        get_filename_component(_o_file_dir ${_o_file} DIRECTORY)

        if (_o_file_dir)
            file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${_o_file_dir})
        endif()

        #Create interface build target
        add_custom_command(
            OUTPUT ${_o_file}
            COMMAND ${_cmd}
            DEPENDS ${_i_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )

        #Replace directory separators with something else
        string(REPLACE "/" "__" _o_file_target ${_o_file})
        string(PREPEND _o_file_target "module_")

        #Create interface build target
        add_custom_target(${_o_file_target}
            COMMAND ${_cmd}
            DEPENDS ${_o_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )

        list(APPEND _interface_files ${_o_file})
        list(APPEND _interface_targets ${_o_file_target})
    endforeach ()

    #Store property with interface files
    set_target_properties(${TARGET}
        PROPERTIES CXX_MODULES_INTERFACE_FILES "${_interface_files}"
    )

    set_target_properties(${TARGET}
        PROPERTIES CXX_MODULES_INTERFACE_TARGETS "${_interface_targets}"
    )
endfunction ()
############################################################################
##Link a (C++ module) library to (C++ module) target.
##Use target property CXX_MODULES_INTERFACE_FILES and CXX_MODULES_INTERFACE_TARGETS
function (target_link_module_libraries TARGET)
    _check_cxx_modules_support()

    #Enable modules for target
    target_enable_cxx_modules(${TARGET})

    foreach (_arg ${ARGN})
        list(FIND "PUBLIC;PRIVATE;INTERFACE" ${_arg} _skip)

        if (${_skip} GREATER_EQUAL 0)
            continue ()
        endif ()

        #Get interface files from library
        get_target_property(_interface_targets ${_arg} CXX_MODULES_INTERFACE_TARGETS)

        foreach (_target ${_interface_targets})
            add_dependencies(${TARGET} ${_target})
        endforeach ()

        #Get interface files from library
        get_target_property(_interface_files ${_arg} CXX_MODULES_INTERFACE_FILES)

        foreach (_file ${_interface_files})
            #TODO: might be different on different compilers
            target_compile_options(${TARGET} PRIVATE ${CXX_MODULES_USE_FLAG}${CMAKE_CURRENT_BINARY_DIR}/${_file})
        endforeach ()
    endforeach ()

    #Normal link
    target_link_libraries(${TARGET} ${ARGN})
endfunction ()