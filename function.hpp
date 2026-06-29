#pragma once

#include "quickjs.h"
#include "clonable.hpp"
#include <initializer_list>

namespace QuickJS {
    class Value;

    class Function final : public Clonable<Function> {
    private:
        JSContext* ctx;
        JSValue func;

    public:
        Function(JSContext* ctx, JSValue func);
        Function(const Function&) = delete;
        Function(Function&& other) noexcept;
        Function& operator=(Function& other) = delete;
        Function& operator=(Function&& other) noexcept;

        ~Function();

        Function clone() const override;

        Value backToJSValue() const;

        bool isValid() const;

        Value call(const Value& thisObject, const std::initializer_list<Value>& args);

        Value call(const Value& thisObject);

        template<typename... Args>
        Value call(const Value& thisObject, Args... args) {
            return this->call(thisObject, { args... });
        }
    };
}