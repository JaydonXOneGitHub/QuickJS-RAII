#pragma once

#include "quickjs.h"
#include "runtime.hpp"
#include "value.hpp"
#include "classbinding.hpp"

namespace QuickJS {
    class Context {
    private:
        JSContext* ctx;
        void* ctxOpaque;
        bool isOwned;

    public:
        Context(Context&) = delete;
        Context(JSContext* ctx);
        Context& operator=(Context& other) = delete;
        Context(const Runtime& runtime);
        ~Context();

        void freeContextOpaque();

        template<typename T, typename... Args>
        T* registerContextOpaque(Args... args) {
            this->freeContextOpaque();
            T* value = new T(args...);
            this->ctxOpaque = value;
            JS_SetContextOpaque(this->ctx, this->ctxOpaque);
            return value;
        }

        template<typename T>
        T* getContextOpaque() const {
            return static_cast<T*>(JS_GetContextOpaque(this->ctx));
        }

        void registerClass(void(*jsClassInstiantiator)(JSRuntime* rt, JSContext* ctx));

        template<typename T>
        void registerClass() {
            this->registerClass(&T::registerClass);
        }

        JSContext* getContext();

        Value getGlobalProperty(const char* propName) const;
        void setGlobalProperty(const char* propName, const Value& value);

        Value evalCode(const std::string& source);

        Value getGlobalObject() const;
        JSValue getGlobalObjectRaw() const;
        void setGlobalPropertyRaw(const char* propName, JSValue value);

        Value createNumber(double value);
        Value createBigInt64(int64_t value);
        Value createFunction(JSCFunction* func, const char* funcName, int argCount = 0);
        Value createString(const std::string& str);
        Value createBoolean(bool boolean);
        Value createArray() const;
        Value createObject() const;
        Value createNull() const;
        Value createUndefined() const;

        JSValue createNumberRaw(double value);
        JSValue createBigInt64Raw(int64_t value);
        JSValue createFunctionRaw(JSCFunction* func, const char* funcName, int argCount = 0);
        JSValue createStringRaw(const std::string& str);
        JSValue createBooleanRaw(bool boolean);
        JSValue createArrayRaw() const;
        JSValue createObjectRaw() const;
        JSValue createNullRaw() const;
        JSValue createUndefinedRaw() const;

        template<typename T, typename... Args>
        Value createNativeObject(Args... args) {
            T* ptr = new T(args...);
        }

        template<typename T>
        ClassBinding<T> createClassBinding(const char* className) const {
            return ClassBinding<T>(this->ctx, className);
        }
    };
}