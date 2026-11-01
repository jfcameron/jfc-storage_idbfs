# © Joseph Cameron - MIT License

if (NOT TARGET jfcstorage_filesystem)
    set(JFCSTORAGE_FILESYSTEM_INSTALL ${JFCSTORAGE_IDBFS_INSTALL})

    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/jfc-storage_filesystem")
endif()
