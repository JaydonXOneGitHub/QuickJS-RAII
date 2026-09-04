#include "promise.hpp"
#include "function.hpp"
#include "object.hpp"
#include <stdlib.h>

using namespace QuickJS;

Promise::Promise(JSContext* ctx) {
    JSValue resolvingFuncs[2];

    JSValue promise = JS_NewPromiseCapability(ctx, resolvingFuncs);

    this->promise = promise;
    this->resolveFunc = resolvingFuncs[0];
    this->rejectFunc = resolvingFuncs[1];
    this->ctx = ctx;
}

Promise::Promise(Promise&& other) noexcept {
    this->promise = other.promise;
    this->resolveFunc = other.resolveFunc;
    this->rejectFunc = other.rejectFunc;
    this->ctx = other.ctx;

    other.promise = JS_UNDEFINED;
    other.resolveFunc = JS_UNDEFINED;
    other.rejectFunc = JS_UNDEFINED;
    other.ctx = nullptr;
}

Promise& Promise::operator=(Promise&& other) noexcept {
    if (this != &other) {
        this->destroy();

        this->promise = other.promise;
        this->resolveFunc = other.resolveFunc;
        this->rejectFunc = other.rejectFunc;
        this->ctx = other.ctx;

        other.promise = JS_UNDEFINED;
        other.resolveFunc = JS_UNDEFINED;
        other.rejectFunc = JS_UNDEFINED;
        other.ctx = nullptr;
    }
    return *this;
}

void Promise::destroy() {
    if (!this->ctx) {
        return;
    }

    JS_FreeValue(this->ctx, this->promise);
    JS_FreeValue(this->ctx, this->resolveFunc);
    JS_FreeValue(this->ctx, this->rejectFunc);

    this->promise = JS_UNDEFINED;
    this->resolveFunc = JS_UNDEFINED;
    this->rejectFunc = JS_UNDEFINED;
    this->ctx = nullptr;
}

Promise::~Promise() {
    this->destroy();
}

Promise Promise::clone() const {
    Promise p = Promise();
    p.ctx = this->ctx;
    p.promise = JS_DupValue(this->ctx, this->promise);
    p.resolveFunc = JS_DupValue(this->ctx, this->resolveFunc);
    p.rejectFunc = JS_DupValue(this->ctx, this->rejectFunc);
    return p;
}

void Promise::resolve(const QuickJS::Value& value) {
    JSValue arg = value.getRaw();
    JSValue ret = JS_Call(this->ctx, this->resolveFunc, JS_UNDEFINED, 1, &arg);
    JS_FreeValue(this->ctx, ret);
}

void Promise::reject(const QuickJS::Value& value) {
    JSValue arg = value.getRaw();
    JSValue ret = JS_Call(this->ctx, this->rejectFunc, JS_UNDEFINED, 1, &arg);
    JS_FreeValue(this->ctx, ret);
}

std::string Promise::toString() const {
    QuickJS::Value v = QuickJS::Value(this->ctx, this->promise, false);

    QuickJS::Object o = v.asObject();

    QuickJS::Value toString = o.get("toString");

    QuickJS::Function toStringFunc = toString.asFunction();

    QuickJS::Value str = toStringFunc.call(v);

    return str.asString();
}

QuickJS::Value Promise::toValue() const {
    return QuickJS::Value(this->ctx, JS_DupValue(this->ctx, this->promise));
}