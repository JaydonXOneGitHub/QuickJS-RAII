#pragma once

#include "quickjs.h"

namespace QuickJS {
    class Runtime final {
    private:
        JSRuntime* rt;

    public:
        Runtime(Runtime&) = delete;
        Runtime& operator=(Runtime& other) = delete;
        Runtime();
        ~Runtime();

        JSRuntime* getRuntime() const;
    };
}