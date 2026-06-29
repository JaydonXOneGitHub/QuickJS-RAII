#include "context.hpp"

using namespace QuickJS;

Context::Context(const Runtime& runtime) {
    this->ctx = JS_NewContext(runtime.getRuntime());
    this->ctxOpaque = NULL;
    this->isOwned = true;
}

Context::Context(JSContext* ctx) {
    this->ctx = ctx;
    this->ctxOpaque = JS_GetContextOpaque(ctx);
    this->isOwned = false;
}

Context::~Context() {
    if (this->isOwned) {
        this->freeContextOpaque();
        JS_FreeContext(this->ctx);
        this->isOwned = false;
    }
}

void Context::freeContextOpaque() {
    if (this->ctxOpaque == NULL) {
        return;
    }

    delete this->ctxOpaque;
    this->ctxOpaque = NULL;
}

JSContext* Context::getContext() {
    return this->ctx;
}

Value Context::getGlobalProperty(const char* propName) const {
    JSValue global = JS_GetGlobalObject(this->ctx);
    Value v = Value(this->ctx, JS_GetPropertyStr(this->ctx, global, propName));
    JS_FreeValue(this->ctx, global);
    return v;
}

void Context::setGlobalProperty(const char* propName, const Value& value) {
    this->setGlobalPropertyRaw(propName, value.getRawDuplicate());
}

void Context::setGlobalPropertyRaw(const char* propName, JSValue value) {
    JSValue global = JS_GetGlobalObject(this->ctx);
    JS_SetPropertyStr(this->ctx, global, propName, value);
    JS_FreeValue(this->ctx, global);
}

void Context::registerClass(void(*jsClassInstiantiator)(JSRuntime* rt, JSContext* ctx)) {
    if (jsClassInstiantiator == 0) {
        throw Exception("Class instantiator is null!");
    }
    JSRuntime* rt = JS_GetRuntime(this->ctx);
    jsClassInstiantiator(rt, this->ctx);
}

Value Context::evalCode(const std::string& source) {
    JS_RunGC(JS_GetRuntime(this->ctx));
    JSValue v = JS_Eval(this->ctx, source.c_str(), source.length(), "<input>", 0);
    Value val = Value(this->ctx, v);
    return val;
}

Value Context::evalModuleCode(const std::string& source) {
    JS_RunGC(JS_GetRuntime(this->ctx));
    JSValue v = JS_Eval(this->ctx, source.c_str(), source.length(), "<input>", JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);

    if (JS_IsException(v)) {
        return Value(this->ctx, v);
    }

    JSValue ret = JS_EvalFunction(this->ctx, v);

    JS_FreeValue(this->ctx, v);

    Value val = Value(this->ctx, ret);
    return val;
}

Value Context::getGlobalObject() const {
    return Value(this->ctx, JS_GetGlobalObject(this->ctx));
}

JSValue Context::getGlobalObjectRaw() const {
    return JS_GetGlobalObject(this->ctx);
}

Value Context::createNumber(double value) {
    return Value(this->ctx, JS_NewFloat64(this->ctx, value));
}

Value Context::createBigInt64(int64_t value) {
    return Value(this->ctx, JS_NewBigInt64(this->ctx, value));
}

Value Context::createFunction(JSCFunction* func, const char* funcName, int argCount) {
    return Value(this->ctx, JS_NewCFunction(this->ctx, func, funcName, argCount));
}

Value Context::createString(const std::string& str) {
    return Value(this->ctx, JS_NewString(this->ctx, str.c_str()));
}

Value Context::createBoolean(bool boolean) {
    return Value(this->ctx, JS_NewBool(this->ctx, boolean));
}

Value Context::createArray() const {
    return Value(this->ctx, JS_NewArray(this->ctx));
}

Value Context::createObject() const {
    return Value(this->ctx, JS_NewObject(this->ctx));
}

Value Context::createNull() const {
    return Value(this->ctx, JS_NULL, false);
}

Value Context::createUndefined() const {
    return Value(this->ctx, JS_UNDEFINED, false);
}

JSValue Context::createNumberRaw(double value) {
    return JS_NewFloat64(this->ctx, value);
}

JSValue Context::createBigInt64Raw(int64_t value) {
    return JS_NewBigInt64(this->ctx, value);
}

JSValue Context::createFunctionRaw(JSCFunction* func, const char* funcName, int argCount) {
    return JS_NewCFunction(this->ctx, func, funcName, argCount);
}

JSValue Context::createStringRaw(const std::string& str) {
    return JS_NewString(this->ctx, str.c_str());
}

JSValue Context::createBooleanRaw(bool boolean) {
    return JS_NewBool(this->ctx, boolean);
}

JSValue Context::createArrayRaw() const {
    return JS_NewArray(this->ctx);
}

JSValue Context::createObjectRaw() const {
    return JS_NewObject(this->ctx);
}

JSValue Context::createNullRaw() const {
    return JS_NULL;
}

JSValue Context::createUndefinedRaw() const {
    return JS_UNDEFINED;
}