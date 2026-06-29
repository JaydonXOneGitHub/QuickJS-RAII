#pragma once

#include "quickjs.h"
#include "clonable.hpp"
#include <string>
#include "classbinding.hpp"

namespace QuickJS {
    class Value;

    class Object final : public Clonable<Object> {
    private:
        JSContext* ctx;
        JSValue object;

    public:
        Object(JSContext* ctx, JSValue object);
        Object(const Object&) = delete;
        Object(Object&& other) noexcept;
        Object& operator=(Object& other) = delete;
        Object& operator=(Object&& other) noexcept;

        ~Object();

        template<typename T>
        T* getOpaque() const {
            std::cout << ClassBinding<T>::classID << '\n';
            return static_cast<T*>(JS_GetOpaque(this->object, ClassBinding<T>::classID));
        }

        Value backToJSValue() const;

        Value get(const char* propName) const;
        void set(const char* propName, const Value& value);

        bool isValid() const;

        Object clone() const override;
    };
}