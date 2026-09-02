#include "moduleloader.hpp"

using namespace QuickJS;

ModuleLoader::ModuleLoader(JSContext* ctx) {
    this->ctx = ctx;
}

ModuleLoader& ModuleLoader::setOpaque(void* opaque) {
    this->opaque = opaque;
    return *this;
}

ModuleLoader& ModuleLoader::setLoader(JSModuleLoaderFunc* loader) {
    this->loader = loader;
    return *this;
}

ModuleLoader& ModuleLoader::setNormalizer(JSModuleNormalizeFunc* normalizer) {
    this->normalizer = normalizer;
    return *this;
}

void ModuleLoader::submit() {
    JS_SetModuleLoaderFunc(
        JS_GetRuntime(this->ctx), 
        this->normalizer, 
        this->loader, 
        this->opaque
    );
}