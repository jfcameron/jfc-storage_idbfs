# jfc-storage_idbfs
The [jfc-storage](https://github.com/jfcameron/jfc-storage) store interface, kept in a browser's IndexedDB through Emscripten's IDBFS.

API documentation: https://jfcameron.github.io/jfc-storage_idbfs/

## Getting Started
Install cmake 3.21 or greater, Emscripten and node
```
# initialize the tree
git submodule update --init --recursive

# generate the script to build the library and its tests
emcmake cmake --preset emscripten

# build the library and tests
cmake --build --preset emscripten

# run the tests
ctest --preset emscripten
```
See `CMakePresets.json` for the presets. See `.github/workflows/ci.yml` to see exactly how CI runners build it.

To use it from another project, add it with `add_subdirectory` and link `jfcstorage_persistent`. The installed package is `jfc-storage_idbfs`, target `jfc::storage_persistent`. A project that has already added jfc-storage or jfc-storage_filesystem gets one copy of each. It builds for the browser only: a native project uses jfc-storage_filesystem alone.

## Overview
A page's filesystem is in memory and is lost when the page closes. `persistent_directory` mounts a directory on IndexedDB, loads what was kept there, and saves what has changed when asked to synchronise. Both are asynchronous, and report how they went to a callback. The user directories of [jfc-storage_filesystem](https://github.com/jfcameron/jfc-storage_filesystem) are under `/persistent`, so a page mounts that once, before anything reads from it.

`make_persistent_store` makes a filesystem store in a mounted directory that synchronises after every change, and tells its callback how each synchronisation went. Changes made while one is under way are saved together by the next. It refuses to change anything before the mount's first load is through, since the load would replace what it had written.
