#pragma once

#include "quickjs.h"
#include "clonable.hpp"
#include <string>
#include "classbinding.hpp"
#include "pointer.hpp"

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
        Pointer<T> getOpaque() const {
            std::cout << ClassBinding<T>::classID << '\n';
            T* ptr = static_cast<T*>(JS_GetOpaque(this->object, ClassBinding<T>::classID));
            return Pointer(ptr);
        }

        Value backToJSValue() const;

        Value get(const char* propName) const;
        void set(const char* propName, const Value& value);

        
        bool isValid() const;
        
        std::string toString() const override;
        
        Object clone() const override;
    };
}