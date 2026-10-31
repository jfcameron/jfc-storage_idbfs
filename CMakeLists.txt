# © Joseph Cameron - MIT License

cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

if (TARGET jfcstorage_persistent)
    return()
endif()

project(jfc-storage_idbfs
    VERSION 1.0.0
    DESCRIPTION "the jfc-storage store interface, kept in a browser's IndexedDB through IDBFS"
    LANGUAGES CXX)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/jfc-cmake/jfclib.cmake")

include(GNUInstallDirs)

option(JFCSTORAGE_IDBFS_BUILD_TESTS "Build unit tests" ${PROJECT_IS_TOP_LEVEL})
option(JFCSTORAGE_IDBFS_BUILD_DOCS "Build documentation" OFF)
option(JFCSTORAGE_IDBFS_INSTALL "Generate install rules" ${PROJECT_IS_TOP_LEVEL})

if (JFCSTORAGE_IDBFS_BUILD_DOCS)
    jfc_generate_documentation_doxygen(
        README_PATH      README.md
        OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/docs

        INCLUDE_DIRECTORIES
            include/
    )

    if (NOT EMSCRIPTEN)
        return()
    endif()
endif()

if (NOT EMSCRIPTEN)
    message(FATAL_ERROR "jfc-storage_idbfs builds for the browser only: configure with the emscripten preset")
endif()

add_subdirectory(extern)

include(CheckCXXCompilerFlag)

set(_jfcstorage_strict_flags)

foreach (_strict_flag "-Werror=c++20-extensions" "-Werror=c++23-extensions")
    string(MAKE_C_IDENTIFIER "HAS${_strict_flag}" _strict_flag_symbol)

    check_cxx_compiler_flag("${_strict_flag}" ${_strict_flag_symbol})

    if (${_strict_flag_symbol})
        list(APPEND _jfcstorage_strict_flags $<$<COMPILE_LANGUAGE:CXX>:${_strict_flag}>)
    endif()
endforeach()

add_library(jfcstorage_persistent STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/persistent_directory.cpp")

add_library(jfc::storage_persistent ALIAS jfcstorage_persistent)

set_target_properties(jfcstorage_persistent PROPERTIES
    EXPORT_NAME storage_persistent
    PREFIX "lib"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

target_compile_features(jfcstorage_persistent PUBLIC cxx_std_20)

target_compile_options(jfcstorage_persistent PRIVATE ${_jfcstorage_strict_flags})

target_include_directories(jfcstorage_persistent PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(jfcstorage_persistent PUBLIC jfcstorage_interface PRIVATE jfcstorage_filesystem)

target_link_options(jfcstorage_persistent PUBLIC "-lidbfs.js")

if (JFCSTORAGE_IDBFS_INSTALL)
    include(CMakePackageConfigHelpers)

    install(TARGETS jfcstorage_persistent
        EXPORT jfc-storage_idbfs-targets
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(EXPORT jfc-storage_idbfs-targets
        FILE jfc-storage_idbfs-targets.cmake
        NAMESPACE jfc::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/jfc-storage_idbfs")

    write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/jfc-storage_idbfs-config-version.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

    file(WRITE "${PROJECT_BINARY_DIR}/jfc-storage_idbfs-config.cmake"
"include(CMakeFindDependencyMacro)
find_dependency(jfcstorage 2)
find_dependency(jfc-storage_filesystem 1)
include(\"\${CMAKE_CURRENT_LIST_DIR}/jfc-storage_idbfs-targets.cmake\")
")

    install(FILES
        "${PROJECT_BINARY_DIR}/jfc-storage_idbfs-config.cmake"
        "${PROJECT_BINARY_DIR}/jfc-storage_idbfs-config-version.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/jfc-storage_idbfs")
endif()

if (JFCSTORAGE_IDBFS_BUILD_TESTS)
    add_subdirectory(test)
endif()
