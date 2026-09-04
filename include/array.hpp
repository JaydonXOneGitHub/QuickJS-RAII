#pragma once

#include "quickjs.h"
#include "clonable.hpp"
#include <initializer_list>

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
        Value pop();
        Value shift();
        Value reverse();
        Value clear();
        bool isEmpty() const;
        uint32_t getLength() const;
        Value push(const std::initializer_list<Value>& values);
        Value unshift(const std::initializer_list<Value>& values);

        template<typename... Args>
        Value push(Args... args) {
            return this->push({ args... });
        }

        template<typename... Args>
        Value unshift(Args... args) {
            return this->unshift({ args... });
        }

        bool isValid() const;

        std::string toString() const override;

        Array clone() const override;
    };
}