#pragma once

#include "quickjs.h"
#include "array.hpp"
#include "object.hpp"
#include "function.hpp"
#include "exception.hpp"
#include "clonable.hpp"
#include "tag.hpp"
#include <string>

namespace QuickJS {
    using Number = double;
    using String = std::string;
    using Boolean = bool;
    using BigInt = int64_t;

    class Value final : public Clonable<Value> {
    private:
        JSValue value;
        JSContext* ctx;
        bool copy;

    public:
        
    public:
        Value(JSContext* ctx, JSValue value, bool copy = true);
        Value(const Value& other) = delete;
        Value(Value&& other) noexcept;
        ~Value();

        Value& operator=(Value& other) = delete;
        Value& operator=(Value&& other) noexcept;

        bool isBigInt() const;
        bool isNumber() const;
        bool isBoolean() const;
        bool isString() const;
        bool isArray() const;
        bool isObject() const;
        bool isFunction() const;
        bool isException() const;
        bool isNull() const;
        bool isUndefined() const;
        bool isNaN() const;

        template <> 
        bool Value::is<double>() const;
        template <> 
        bool Value::is<std::string>() const;
        template <> 
        bool Value::is<bool>() const;
        template <> 
        bool Value::is<int64_t>() const;
        template <> 
        bool Value::is<Object>() const;
        template <> 
        bool Value::is<Function>() const;
        template <> 
        bool Value::is<Exception>() const;

        int64_t asBigInt() const;
        double asNumber() const;
        bool asBoolean() const;
        std::string asString() const;
        Array asArray() const;
        Object asObject() const;
        Function asFunction() const;
        Exception asException() const;

        template <> 
        double Value::as<double>() const;
        template <> 
        std::string Value::as<std::string>() const;
        template <> 
        bool Value::as<bool>() const;
        template <> 
        int64_t Value::as<int64_t>() const;
        template <> 
        Object Value::as<Object>() const;
        template <> 
        Function Value::as<Function>() const;
        template <> 
        Exception Value::as<Exception>() const;

        Tag getTag() const;

        std::string toString() const override;

        Value clone() const override;

        // Gets duplicated raw JS value - if not consumed, free this afterwards.
        JSValue getRawDuplicate() const;
        // Gets raw JS value
        JSValue getRaw() const;
    };
}