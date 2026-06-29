#define QUICKJS_FUNCTION_DECLARATION(name) \
    static JSValue name (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv); \
    static QuickJS::Value name##Wrapped(const QuickJS::Context& context, \
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