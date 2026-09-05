#include <vector>
#include <string>
#include "context.hpp"

#define QUICKJS_FUNCTION_DECLARATION(name) \
    JSValue name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv); \
    QuickJS::Value name##Wrapped(QuickJS::Context& context, \
        QuickJS::Value& thisValue, \
        const std::vector<QuickJS::Value>& args \
    );

#define QUICKJS_STATIC_FUNCTION_DECLARATION(name) \
    static JSValue name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv); \
    static QuickJS::Value name##Wrapped(QuickJS::Context& context, \
        QuickJS::Value& thisValue, \
        const std::vector<QuickJS::Value>& args \
    );

#define QUICKJS_FUNCTION_DECLARATION_LAMBDA(name) \
    JSValue name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv); \

#define QUICKJS_STATIC_FUNCTION_DECLARATION_LAMBDA(name) \
    static JSValue name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv); \

#define QUICKJS_FUNCTION_IMPLEMENTATION(name) \
    JSValue name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) { \
        QuickJS::Value thisValue = QuickJS::Value(ctx, this_val, false); \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::vector<QuickJS::Value> args = std::vector<QuickJS::Value>(); \
        args.reserve(argc); \
        for (int i = 0; i < argc; i++) { \
            args.emplace_back(ctx, argv[i], false);  \
        }\
        return name##Wrapped(context, thisValue, args).getRawDuplicate(); \
    }

#define QUICKJS_STATIC_FUNCTION_IMPLEMENTATION(name) \
    static JSValue name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) { \
        QuickJS::Value thisValue = QuickJS::Value(ctx, this_val, false); \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::vector<QuickJS::Value> args = std::vector<QuickJS::Value>(); \
        args.reserve(argc); \
        for (int i = 0; i < argc; i++) { \
            args.emplace_back(ctx, argv[i], false);  \
        }\
        return name##Wrapped(context, thisValue, args).getRawDuplicate(); \
    }

#define QUICKJS_FUNCTION_IMPLEMENTATION_LAMBDA(name, callback) \
    JSValue name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) { \
        QuickJS::Value thisValue = QuickJS::Value(ctx, this_val, false); \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::vector<QuickJS::Value> args = std::vector<QuickJS::Value>(); \
        args.reserve(argc); \
        for (int i = 0; i < argc; i++) { \
            args.emplace_back(ctx, argv[i], false);  \
        }\
        QuickJS::WrappedCallbacks::Value cb = callback; \
        return cb(context, thisValue, args).getRawDuplicate(); \
    }

#define QUICKJS_STATIC_FUNCTION_IMPLEMENTATION_LAMBDA(name, callback) \
    static JSValue name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) { \
        QuickJS::Value thisValue = QuickJS::Value(ctx, this_val, false); \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::vector<QuickJS::Value> args = std::vector<QuickJS::Value>(); \
        args.reserve(argc); \
        for (int i = 0; i < argc; i++) { \
            args.emplace_back(ctx, argv[i], false);  \
        }\
        QuickJS::WrappedCallbacks::Value cb = callback; \
        return cb(context, thisValue, args).getRawDuplicate(); \
    }

#define QUICKJS_STATIC_METHOD_DECLARATION(name) QUICKJS_STATIC_FUNCTION_DECLARATION(name)
#define QUICKJS_STATIC_METHOD_DECLARATION_LAMBDA(name) QUICKJS_STATIC_FUNCTION_DECLARATION_LAMBDA(name)

#define QUICKJS_STATIC_METHOD_IMPLEMENTATION_LAMBDA(name, callback) \
    JSValue type :: name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) { \
        QuickJS::Value thisValue = QuickJS::Value(ctx, this_val, false); \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::vector<QuickJS::Value> args = std::vector<QuickJS::Value>(); \
        args.reserve(argc); \
        for (int i = 0; i < argc; i++) { \
            args.emplace_back(ctx, argv[i], false);  \
        }\
        QuickJS::WrappedCallbacks::Value cb = callback; \
        return cb(context, thisValue, args).getRawDuplicate(); \
    }

#define QUICKJS_MODULE_INIT_DECLARATION(name) \
    int name (JSContext *ctx, JSModuleDef *m); \
    int name##Wrapped (QuickJS::Context& ctx, QuickJS::ModuleDef& md);

#define QUICKJS_MODULE_INIT_IMPLEMENTATION(name) \
    int name (JSContext *ctx, JSModuleDef *m) { \
        QuickJS::Context context = QuickJS::Context(ctx); \
        QuickJS::ModuleDef moduleDef = QuickJS::ModuleDef(ctx, m); \
        int res = name##Wrapped(context, moduleDef); \
        return res; \
    }

#define QUICKJS_MODULE_INIT_DECLARATION_LAMBDA(name) \
    int name (JSContext *ctx, JSModuleDef *m); \

#define QUICKJS_MODULE_INIT_IMPLEMENTATION_LAMBDA(name, callback) \
    int name (JSContext *ctx, JSModuleDef *m) { \
        QuickJS::Context context = QuickJS::Context(ctx); \
        QuickJS::ModuleDef moduleDef = QuickJS::ModuleDef(ctx, m); \
        QuickJS::WrappedCallbacks::ModuleInit cb = callback; \
        return cb(context, moduleDef); \
    }

#define QUICKJS_MODULE_NORMALIZER_DECLARATION(name) \
    char *name ( \
        JSContext *ctx, \
        const char *module_base_name, \
        const char *module_name, void *opaque \
    ); \
    \
    std::string name##Wrapped ( \
        QuickJS::Context& ctx, \
        const std::string& moduleBaseName, \
        const std::string& moduleName, \
        void* opaque \
    );

#define QUICKJS_MODULE_NORMALIZER_IMPLEMENTATION(name) \
    char *name ( \
        JSContext *ctx, \
        const char *module_base_name, \
        const char *module_name, void *opaque \
    ) { \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::string moduleBaseName = module_base_name; \
        std::string moduleName = module_name; \
        std::string res = name##Wrapped(context, moduleBaseName, moduleName, opaque); \
        return js_strdup(ctx, res.c_str()); \
    }

#define QUICKJS_MODULE_NORMALIZER_DECLARATION_LAMBDA(name) \
    char *name ( \
        JSContext *ctx, \
        const char *module_base_name, \
        const char *module_name, void *opaque \
    );

#define QUICKJS_MODULE_NORMALIZER_IMPLEMENTATION(name, callback) \
    char *name ( \
        JSContext *ctx, \
        const char *module_base_name, \
        const char *module_name, void *opaque \
    ) { \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::string moduleBaseName = module_base_name; \
        std::string moduleName = module_name; \
        QuickJS::WrappedCallbacks::ModuleNormalizer cb = callback; \
        std::string res = cb(context, moduleBaseName, moduleName, opaque); \
        return js_strdup(ctx, res.c_str()); \
    }

#define QUICKJS_MODULE_LOADER_DECLARATION(name) \
    JSModuleDef *name(JSContext *ctx, const char *module_name, void *opaque); \
    QuickJS::ModuleDef name##Wrapped(QuickJS::Context& ctx, const std::string& moduleName, void* opaque);
    
#define QUICKJS_MODULE_LOADER_IMPLEMENTATION(name) \
    JSModuleDef *name(JSContext *ctx, const char *module_name, void *opaque) { \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::string moduleName = module_name; \
        QuickJS::ModuleDef res = name##Wrapped(context, moduleName, opaque); \
        return res.getRawModule(); \
    }

#define QUICKJS_MODULE_LOADER_DECLARATION_LAMBDA(name) \
    JSModuleDef *name(JSContext *ctx, const char *module_name, void *opaque);

#define QUICKJS_MODULE_LOADER_IMPLEMENTATION_LAMBDA(name, callback) \
    JSModuleDef *name(JSContext *ctx, const char *module_name, void *opaque) { \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::string moduleName = module_name; \
        QuickJS::WrappedCallbacks::ModuleLoader cb = callback; \
        QuickJS::ModuleDef res = cb(context, moduleName, opaque); \
        return res.getRawModule(); \
    }