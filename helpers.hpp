#define QUICKJS_FUNCTION_DECLARATION(name) \
    static JSValue name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv); \
    static QuickJS::Value name##Wrapped(QuickJS::Context& context, \
        QuickJS::Value& thisValue, \
        const std::vector<QuickJS::Value>& args \
    );

#define QUICKJS_FUNCTION_IMPLEMENTATION(name) \
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

#define QUICKJS_METHOD_IMPLEMENTATION(type, name) \
    JSValue type :: name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) { \
        QuickJS::Value thisValue = QuickJS::Value(ctx, this_val, false); \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::vector<QuickJS::Value> args = std::vector<QuickJS::Value>(); \
        args.reserve(argc); \
        for (int i = 0; i < argc; i++) { \
            args.emplace_back(ctx, argv[i], false);  \
        }\
        return type :: name##Wrapped(context, thisValue, args).getRawDuplicate(); \
    }

#define QUICKJS_MODULE_LOADER_DECLARATION(name) \
    static JSModuleDef* name(JSContext *ctx, const char *module_name, void *opaque); \
    static QuickJS::ModuleDef name##Wrapped(QuickJS::Context& context, \
        const std::string& moduleName \
    );

#define QUICKJS_MODULE_LOADER_IMPLEMENTATION(name) \
    static JSModuleDef* name(JSContext *ctx, const char *module_name, void *opaque) { \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::string moduleName(module_name ? module_name : ""); \
        try { \
            return name##Wrapped(context, moduleName).getRaw(); \
        } catch (const std::exception& e) { \
            return nullptr; \
        } \
    }

#define QUICKJS_MODULE_LOADER_CLASS_IMPLEMENTATION(type, name) \
    JSModuleDef* type :: name(JSContext *ctx, const char *module_name, void *opaque) { \
        QuickJS::Context context = QuickJS::Context(ctx); \
        std::string moduleName(module_name ? module_name : ""); \
        try { \
            return type :: name##Wrapped(context, moduleName).getRaw(); \
        } catch (const std::exception& e) { \
            return nullptr; \
        } \
    }