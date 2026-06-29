#include "object.hpp"
#include "value.hpp"

#include <iostream>

using namespace QuickJS;

Object::Object(JSContext* ctx, JSValue object) {
    this->ctx = ctx;
    this->object = JS_DupValue(ctx, object);
    JS_FreeValue(this->ctx, object);
}

Object::Object(Object&& other) noexcept {
    this->ctx = other.ctx;

    this->object = other.object;

    other.ctx = NULL;

    other.object = JS_UNDEFINED;
}

Object& Object::operator=(Object&& other) noexcept {
    if (this != &other) {
        if (this->ctx) { 
            JS_FreeValue(this->ctx, this->object); 
        }

        this->ctx = other.ctx;

        this->object = other.object;

        other.ctx = NULL;

        other.object = JS_UNDEFINED;
    }
    return *this;
}

Object::~Object() {
    if (this->isValid()) {
        JS_FreeValue(this->ctx, this->object);
    }
}

bool Object::isValid() const {
    return (this->ctx != NULL) || (!JS_IsNull(this->object) && !JS_IsUndefined(this->object));
}

Value Object::backToJSValue() const {
    return Value(this->ctx, JS_DupValue(this->ctx, this->object));
}

Value Object::get(const char* propName) const {
    if (!this->isValid()) {
        throw Exception("Object is no longer valid!");
    }
    JSValue v = JS_GetPropertyStr(this->ctx, this->object, propName);
    Value value = Value(this->ctx, v);
    return value;
}

void Object::set(const char* propName, const Value& value) {
    if (!this->isValid()) {
        throw Exception("Object is no longer valid!");
    }

    int ret = JS_SetPropertyStr(this->ctx, this->object, propName, value.getRawDuplicate());

    if (ret < 0) {
        throw Exception("Object property assignment failed!");
    }
}

Object Object::clone() const {
    return Object(this->ctx, JS_DupValue(this->ctx, this->object));
}