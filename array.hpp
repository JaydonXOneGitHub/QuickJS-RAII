#pragma once

#include "quickjs.h"
#include "clonable.hpp"

namespace QuickJS {
    class Value;

    class Array final : public Clonable<Array> {
    private:
        JSContext* ctx;
        JSValue array;
        JSValue length;

    public:
        Array(JSContext* ctx, JSValue array, JSValue length);
        Array(const Array&) = delete;
        Array(Array&& other) noexcept;
        Array& operator=(Array&& other) noexcept;

        ~Array();

        Value backToJSValue() const;

        Value get(uint32_t index) const;
        void set(uint32_t index, const Value& value);

        uint32_t getLength() const;

        bool isValid() const;

        Array clone() const override;
    };
}