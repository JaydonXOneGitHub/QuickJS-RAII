#include "value.hpp"
#include <iostream>

using namespace QuickJS;

Value::Value(JSContext* ctx, JSValue value, bool copy) {
    this->ctx = ctx;
    this->copy = copy;
    
    if (copy) {
        this->value = JS_DupValue(ctx, value);
        JS_FreeValue(ctx, value);
    }
    else {
        this->value = value;
    }
}

Value::~Value() {
    if ((this->isNull() || this->isUndefined())) {
        return;
    }

    if (this->ctx == NULL) {
        return;
    }

    if (!this->copy) {
        return;
    }
    
    JS_FreeValue(this->ctx, this->value);
}

Value::Value(Value&& other) noexcept {
    this->ctx = other.ctx;

    this->value = other.value;

    other.ctx = NULL;

    other.value = JS_UNDEFINED;

    other.copy = false;
}
    
Value& Value::operator=(Value&& other) noexcept {
    if (this != &other) {
        if (this->ctx) { 
            JS_FreeValue(this->ctx, this->value); 
        }

        this->ctx = other.ctx;

        this->value = other.value;

        other.ctx = NULL;

        other.value = JS_UNDEFINED;

        other.copy = false;
    }
    return *this;
}

bool Value::isBigInt() const {
    return (bool)JS_IsBigInt(this->ctx, this->value);
}

int64_t Value::asBigInt() const {
    if (!this->isBigInt()) {
        throw Exception("Not of type \"BigInt\"!");
    }

    int64_t bigInt = 0;

    int ret = JS_ToBigInt64(this->ctx, &bigInt, this->value);
    
    if (ret < 0) {
        throw Exception("BigInt conversion failed!");
    }

    return bigInt;
}

bool Value::isNumber() const {
    return (bool)JS_IsNumber(this->value);
}

double Value::asNumber() const {
    if (!this->isNumber()) {
        throw Exception("Not of type \"number\"!");
    }

    double number = 0;

    int ret = JS_ToFloat64(this->ctx, &number, this->value);
    
    if (ret < 0) {
        throw Exception("Number conversion failed!");
    }

    return number;
}

bool Value::isBoolean() const {
    return (bool)JS_IsBool(this->value);
}

bool Value::asBoolean() const {
    if (!this->isBoolean()) {
        throw Exception("Not of type \"boolean\"!");
    }

    int ret = JS_ToBool(this->ctx, this->value);

    if (ret == -1) {
        JSValue error = JS_GetException(this->ctx);

        if (!JS_IsException(error)) {
            throw Exception("Exception conversion failed!");;
        }
        
        JSValue message = JS_GetPropertyStr(this->ctx, error, "message");

        if (JS_IsString(message)) {
            const char *msg = JS_ToCString(this->ctx, message);

            Exception e = Exception(msg);

            JS_FreeCString(this->ctx, msg);

            throw e;
        }
    }

    return (ret != 0);
}

bool Value::isString() const {
    return (bool)JS_IsString(this->value);
}

std::string Value::asString() const {
    if (!this->isString()) {
        throw Exception("Not of type \"string\"!");
    }

    const char* str = JS_ToCString(this->ctx, this->value);
    
    if (!str) {
        throw Exception("String conversion failed!");
    }

    std::string result = std::string(str);

    JS_FreeCString(this->ctx, str);

    return result;
}

bool Value::isArray() const {
    return (bool)JS_IsArray(this->ctx, this->value);
}

Array Value::asArray() const {
    if (!this->isArray()) {
        throw Exception("Not of type \"Array\"!");
    }

    JSValue len_val = JS_GetPropertyStr(this->ctx, this->value, "length");

    if (!JS_IsNumber(len_val)) {
        JS_FreeValue(this->ctx, len_val);
        throw Exception("Length somehow not of type \"number\"!");
    }

    return Array(this->ctx, JS_DupValue(this->ctx, this->value), len_val);
}

bool Value::isObject() const {
    return (bool)JS_IsObject(this->value);
}

Object Value::asObject() const {
    if (!this->isObject()) {
        throw Exception("Not of type \"Object\"!");
    }

    return Object(this->ctx, JS_DupValue(this->ctx, this->value));
}

bool Value::isFunction() const {
    return (bool)JS_IsFunction(this->ctx, this->value);
}

Function Value::asFunction() const {
    if (!this->isFunction()) {
        throw Exception("Not of type \"function\"!");
    }

    return Function(this->ctx, JS_DupValue(this->ctx, this->value));
}

bool Value::isException() const {
    return (bool)JS_IsError(this->ctx, this->value);
}

Exception Value::asException() const {
    if (!this->isException()) {
        return Exception("Exception conversion failed!");
    }

    JSValue error = JS_GetException(this->ctx);

    if (!JS_IsException(error)) {
        return Exception("Exception conversion failed!");;
    }
    
    JSValue message = JS_GetPropertyStr(this->ctx, error, "message");

    if (JS_IsString(message)) {
        const char *msg = JS_ToCString(this->ctx, message);

        Exception e = Exception(msg);

        JS_FreeCString(this->ctx, msg);

        return e;
    }

    return Exception("Exception conversion failed!");
}

bool Value::isNull() const {
    return (bool)JS_IsNull(this->value);
}

bool Value::isUndefined() const {
    return (bool)JS_IsUndefined(this->value);
}

bool Value::isNaN() const {
    return (bool)JS_VALUE_IS_NAN(this->value);
}

Value Value::clone() const {
    return Value(this->ctx, this->value);
}

JSValue Value::getRaw() const {
    return this->value;
}

JSValue Value::getRawDuplicate() const {
    return JS_DupValue(this->ctx, this->value);
}