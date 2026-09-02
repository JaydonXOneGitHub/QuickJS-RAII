#include "moduledef.hpp"

using namespace QuickJS;

ModuleDef::ModuleDef() {
    this->ctx = nullptr;
    this->moduleDef = nullptr;
}

ModuleDef::ModuleDef(JSContext* ctx, JSModuleDef* moduleDef) {
    this->ctx = ctx;
    this->moduleDef = moduleDef;
}

JSModuleDef* ModuleDef::getRawModule() const {
    return this->moduleDef;
}

bool ModuleDef::isValid() const {
    return !(this->ctx == nullptr || this->moduleDef == nullptr);
}

int ModuleDef::addModuleExport(std::string& exportName) {
    if (!this->isValid()) {
        return -505;
    }

    return JS_AddModuleExport(this->ctx, this->moduleDef, exportName.c_str());
}

int ModuleDef::exportValue(std::string& exportName, QuickJS::Value value) {
    if (!this->isValid()) {
        return -505;
    }

    return JS_SetModuleExport(
        this->ctx, this->moduleDef, 
        exportName.c_str(), value.getRawDuplicate()
    );
}