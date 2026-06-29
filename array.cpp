#include "array.hpp"
#include "value.hpp"
#include <iostream>

using namespace QuickJS;

Array::Array(JSContext* ctx, JSValue array, JSValue length) {
    this->ctx = ctx;
    this->array = JS_DupValue(ctx, array);
    this->length = JS_DupValue(ctx, length);
    JS_FreeValue(this->ctx, length);
    JS_FreeValue(this->ctx, array);
}

Array::Array(Array&& other) noexcept {
    this->ctx = other.ctx;
    this->array = other.array;
    this->length = other.length;
    
    other.ctx = NULL;
    other.array = JS_UNDEFINED;
    other.length = JS_UNDEFINED;
}
    
Array& Array::operator=(Array&& other) noexcept {
    if (this != &other) {
        if (this->ctx) {
            JS_FreeValue(this->ctx, this->array);
            JS_FreeValue(this->ctx, this->length);
        }

        this->ctx = other.ctx;
        this->array = other.array;
        this->length = other.length;

        other.ctx = NULL;
        other.array = JS_UNDEFINED;
        other.length = JS_UNDEFINED;
    }
    return *this;
}

Array::~Array() {
    //std::cout << "Array::~Array() destroying" << std::endl;

    if (this->isValid()) {
        //std::cout << "  -> freeing array and length" << std::endl;
        JS_FreeValue(this->ctx, this->array);
        JS_FreeValue(this->ctx, this->length);
    }
}

Value Array::backToJSValue() const {
    if (!this->isValid()) {
        throw Exception("Array is no longer valid!");
    }

    return Value(this->ctx, JS_DupValue(this->ctx, this->array));
}

Value Array::get(uint32_t index) const {
    if (!this->isValid()) {
        throw Exception("Array is no longer valid!");
    }

    JSValue v = JS_GetPropertyUint32(this->ctx, this->array, index);
    Value value = Value(this->ctx, v);
    //JS_FreeValue(this->ctx, v);
    return value;
}

void Array::set(uint32_t index, const Value& value) {
    int ret = JS_SetPropertyUint32(this->ctx, this->array, index, value.getRawDuplicate());

    if (ret < 0) {
        throw Exception("Array element insertion failed!");
    }
}

uint32_t Array::getLength() const {
    Value len = Value(this->ctx, this->length);
    return (uint32_t)len.asNumber();
}

bool Array::isValid() const {
    return (this->ctx != NULL) || (
        (!JS_IsNull(this->array) && !JS_IsUndefined(this->array)) &&
        (!JS_IsNull(this->length) && !JS_IsUndefined(this->length))
    );
}

Array Array::clone() const {
    return Array(this->ctx, this->array, this->length);
}