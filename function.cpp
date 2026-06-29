#include "function.hpp"
#include "value.hpp"
#include "exception.hpp"
#include <vector>

#include <iostream>

using namespace QuickJS;

Function::Function(JSContext* ctx, JSValue func) {
    this->ctx = ctx;

    this->func = JS_DupValue(ctx, func);

    JS_FreeValue(this->ctx, func);
}

Function::Function(Function&& other) noexcept {
    this->ctx = other.ctx;

    this->func = other.func;

    other.ctx = NULL;

    other.func = JS_UNDEFINED;
}

Function& Function::operator=(Function&& other) noexcept {
    if (this != &other) {
        if (this->ctx) { 
            JS_FreeValue(this->ctx, this->func); 
        }

        this->ctx = other.ctx;

        this->func = other.func;

        other.ctx = NULL;

        other.func = JS_UNDEFINED;
    }
    return *this;
}

Function::~Function() {
    if (this->isValid()) {
        JS_FreeValue(this->ctx, this->func);
    }
}

Function Function::clone() const {
    return Function(this->ctx, JS_DupValue(this->ctx, this->func));
}

bool Function::isValid() const {
    return (this->ctx != NULL) || (!JS_IsNull(this->func) && !JS_IsUndefined(this->func));
}

Value Function::backToJSValue() const {
    if (!this->isValid()) {
        throw Exception("Function is no longer valid!");
    }
    return Value(this->ctx, JS_DupValue(this->ctx, this->func));
}

Value Function::call(const Value& thisObject, const std::initializer_list<Value>& args) {
    if (!this->isValid()) {
        throw Exception("Function is no longer valid!");
    }

    std::vector<JSValue> finalArgs = std::vector<JSValue>();

    finalArgs.reserve(args.size());

    for (const Value& v : args) {
        finalArgs.push_back(v.getRaw());
    }

    JSValue v = JS_Call(
        this->ctx, this->func, 
        thisObject.getRaw(), finalArgs.size(), 
        finalArgs.data()
    );

    return Value(this->ctx, v);
}

Value Function::call(const Value& thisObject) {
    if (!this->isValid()) {
        throw Exception("Function is no longer valid!");
    }
    
    JSValue v = JS_Call(
        this->ctx, this->func, 
        thisObject.getRaw(), 0, 
        NULL
    );

    return Value(this->ctx, v);
}