#pragma once

#include <functional>
#include <vector>
#include <string>

namespace QuickJS {
    class Context;
    class Value;
    class ModuleDef;

    namespace WrappedCallbacks {
        typedef std::function<QuickJS::Value(
            QuickJS::Context&,
            QuickJS::Value&,
            const std::vector<QuickJS::Value>&
        )> Value;

        typedef std::function<QuickJS::Value(
            QuickJS::Context&,
            QuickJS::ModuleDef&
        )> ModuleInit;

        typedef std::function<std::string(
            QuickJS::Context&,
            const std::string&,
            const std::string&,
            void*
        )> ModuleNormalizer;

        typedef std::function<QuickJS::ModuleDef(
            QuickJS::Context&, 
            const std::string&, 
            void*
        )> ModuleLoader;
    }
}