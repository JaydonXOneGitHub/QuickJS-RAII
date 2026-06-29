#include "runtime.hpp"

using namespace QuickJS;

Runtime::Runtime() {
    this->rt = JS_NewRuntime();
}

Runtime::~Runtime() {
    JS_RunGC(rt);
    JS_FreeRuntime(this->rt);
}

JSRuntime* Runtime::getRuntime() const {
    return this->rt;
}