#pragma once

#include "quickjs.h"
#include <iostream>

namespace QuickJS {
    template<typename T>
    class ClassBinding final {
    private:
        JSContext* ctx;
        const char* className;
        JSValue prototype;

    public:
        static inline JSClassID classID = 0;

    public:
        ClassBinding(JSContext* ctx, const char* className) {
            this->ctx = ctx;
            this->className = className;

            JS_NewClassID(&ClassBinding<T>::classID);
        
            JSClassDef classDef = {};

            classDef.class_name = this->className;
            classDef.finalizer = &ClassBinding<T>::finalizer;
            
            JS_NewClass(JS_GetRuntime(this->ctx), this->classID, &classDef);

            this->prototype = JS_NewObject(this->ctx);
            JS_SetClassProto(this->ctx, this->classID, this->prototype);
        }

        JSValue getPrototype() {
            return this->prototype;
        }

        void registerConstructor(JSCFunction* constructor_func, int argc) {
            JSValue global = JS_GetGlobalObject(this->ctx);
            JSValue ctor = JS_NewCFunction2(this->ctx, constructor_func, this->className, argc,
                                            JS_CFUNC_constructor, 0);
            JS_SetPropertyStr(this->ctx, global, this->className, ctor);
            JS_FreeValue(this->ctx, global);
        }

        ClassBinding<T>& addMethod(const char* name, JSCFunction* func, int length) {
            JSValue proto = this->getPrototype();

            JS_DefinePropertyValueStr(this->ctx, proto, name, JS_NewCFunction(this->ctx, func, name, length), JS_PROP_CONFIGURABLE);

            return *this;
        }

    private:
        static void finalizer(JSRuntime* rt, JSValue val) {
            T* ptr = static_cast<T*>(JS_GetOpaque(val, ClassBinding<T>::classID));

            if (ptr) {
                delete ptr;
            }
        }
    };
}