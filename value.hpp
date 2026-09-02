#pragma once

#include "quickjs.h"
#include "array.hpp"
#include "object.hpp"
#include "function.hpp"
#include "exception.hpp"
#include "clonable.hpp"
#include "tag.hpp"
#include <string>
#include "moduledef.hpp"

namespace QuickJS {
    class ModuleDef;

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

        int64_t asBigInt() const;
        double asNumber() const;
        bool asBoolean() const;
        std::string asString() const;
        Array asArray() const;
        Object asObject() const;
        Function asFunction() const;
        Exception asException() const;

        ModuleDef getModule() const;

        Tag getTag() const;

        std::string toString() const override;

        Value clone() const override;

        // Gets duplicated raw JS value - if not consumed, free this afterwards.
        JSValue getRawDuplicate() const;
        // Gets raw JS value
        JSValue getRaw() const;
    };
}