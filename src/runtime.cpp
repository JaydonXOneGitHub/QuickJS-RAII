#include "runtime.hpp"

using namespace QuickJS;

Runtime::Runtime() {
    this->rt = JS_NewRuntime();
}

Runtime::~Runtime() {
    this->runGarbageCollector();
    JS_FreeRuntime(this->rt);
}

JSRuntime* Runtime::getRuntime() const {
    return this->rt;
}

void Runtime::runGarbageCollector() const {
    JS_RunGC(this->rt);
}