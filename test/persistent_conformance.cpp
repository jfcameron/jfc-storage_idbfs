// © Joseph Cameron - MIT License

#include "conformance.h"

#include <jfc/storage/persistent_directory.h>
#include <jfc/storage/persistent_store.h>

#include <emscripten/em_asm.h>

#include <string>

namespace jfc::storage::conformance {
    store_ptr_type make_store() {
        static int made = 0;

        EM_ASM({ FS.syncfs = function(populate, callback) { callback(null); }; });

        const std::filesystem::path root = "/conformance_" + std::to_string(made++);

        persistent_directory::mount(root, [](bool) {});

        return make_persistent_store(root);
    }

    std::string_view implementation() { return "persistent"; }
}
