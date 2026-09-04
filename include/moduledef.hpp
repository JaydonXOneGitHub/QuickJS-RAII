#pragma once

#include "quickjs.h"
#include <string>
#include "value.hpp"

namespace QuickJS {
    class ModuleDef final {
    private:
        JSContext* ctx;
        JSModuleDef* moduleDef;

    public:
        explicit ModuleDef(JSContext* ctx, JSModuleDef* moduleDef);
        ModuleDef();

        JSModuleDef* getRawModule() const;

        bool isValid() const;

        int addModuleExport(std::string& exportName);
        int exportValue(std::string& exportName, QuickJS::Value value);
    };
}