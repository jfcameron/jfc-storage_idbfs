# © Joseph Cameron - MIT License

get_target_property(_jfc_storage_dir jfcstorage SOURCE_DIR)

jfc_add_tests(
    C++_STANDARD 20

    TEST_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/persistent_directory_test.cpp"

    INCLUDE_DIRECTORIES
        "${_jfc_storage_dir}/test/conformance"

    LIBRARIES
        jfcstorage_persistent
        jfcstorage_filesystem
)

set_property(TARGET ${JFC_TEST_TARGET} PROPERTY CXX_EXTENSIONS OFF)

# a binary of its own: its FS.syncfs stands in for IndexedDB for the whole run, and the tests above want the real one
jfc_add_tests(
    C++_STANDARD 20

    TEST_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/persistent_conformance.cpp"

    LIBRARIES
        jfcstorage_persistent
        jfcstorage_conformance
)

set_property(TARGET ${JFC_TEST_TARGET} PROPERTY CXX_EXTENSIONS OFF)
