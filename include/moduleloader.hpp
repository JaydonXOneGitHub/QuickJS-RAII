#pragma once

#include "quickjs.h"

namespace QuickJS {
    class ModuleLoader {
    private:
        JSContext* ctx;
        void* opaque;
        JSModuleNormalizeFunc* normalizer;
        JSModuleLoaderFunc* loader;

    public:
        ModuleLoader(JSContext* ctx);

        ModuleLoader& setOpaque(void* opaque);
        ModuleLoader& setLoader(JSModuleLoaderFunc* loader);
        ModuleLoader& setNormalizer(JSModuleNormalizeFunc* normalizer);

        void submit();
    };
}