# QuickJS-RAII
A custom QuickJS wrapper designed for idiomatic C++

# Prerequisites

- You must have compiled a static or dynamic library of QuickJS (preferably version `2020-07-05`, as detailed in [quickjs-version.h](https://github.com/JaydonXOneGitHub/QuickJS-RAII/blob/main/quickjs-version.h) - if you use a different version that is incompatible with this API, do so at your own risk).
- You must have a C++ compiler that is compatible with C++17 or later.

# Additional Notes
Properties for custom JS classes have not been implemented yet. Currently, to get a similar approach for them, as an example, use `getX` and `setX` methods as opposed to using a single property `x`.<br>
If any of the preceding bother you enough, please submit a request, or feel free to fork yourself, provided you're comfortable dealing with a C API underneath. :relaxed:

# How to Build
1. Install Python.
2. Install `pip` if not already installed.
3. Install `scons`. (on Windows, this is achieved by `python -m pip install scons` or `py -m pip install scons`)
4. In the project folder, simply use the `scons` command in the terminal.

# How to Use - Basic
A basic use of the library goes like this for evaluating a script:
```cpp
#include "quickjs-raii/runtime.hpp"
#include "quickjs-raii/context.hpp"
#include "quickjs-raii/value.hpp"
#include "quickjs-raii/object.hpp"
#include "quickjs-raii/function.hpp"

#include <iostream>

int main() {
    // Initialize runtime and context
    QuickJS::Runtime runtime = QuickJS::Runtime();
    QuickJS::Context context = QuickJS::Context(runtime);

    // Run script
    QuickJS::Value eval = context.evalCode("({ a: 30, b: () => 50, c: \"Hello, World!\" })");

    if (eval.isObject()) {
        // Check and evaluate that it's an object
        QuickJS::Object obj = eval.asObject();

        QuickJS::Value a = obj.get("a");

        if (a.isNumber()) {
            // Get obj.a as number
            std::cout << "a: " << a.asNumber() << std::endl;
        }

        if (b.isFunction()) {
            // Get obj.b as function
            QuickJS::Function bFunc = b.asFunction();

            QuickJS::Value bFuncRet = bFunc.call(context.createUndefined());

            if (bFuncRet.isNumber()) {
                // Get return value of obj.b()
                std::cout << "b returned: " << bFuncRet.asNumber() << std::endl;
            }
        }

        if (c.isString()) {
            // Get obj.c as string
            std::cout << "c: " << c.asString() << std::endl;
        }
    }
    return 0;
}
```

# How to Use - Class Binding
In order to bind and use a class in JS, we'll follow a simple example with a `Point` class:
```cpp
#include "quickjs-raii/runtime.hpp"
#include "quickjs-raii/context.hpp"
#include "quickjs-raii/helpers.hpp"
#include "quickjs-raii/classbinding.hpp"

#include <iostream>

struct Point {
public:
    int x;
    int y;

public:
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    QUICKJS_FUNCTION_DECLARATION(constructor)
};

QUICKJS_METHOD_IMPLEMENTATION(Point, constructor)

QuickJS::Value Point::constructorWrapped(
    QuickJS::Context& ctx, 
    QuickJS::Value& thisValue, 
    const std::vector<QuickJS::Value>& args
) {
    std::cout << "Constructor called!" << std::endl;

    if (args.size() < 2) {
        return QuickJS::Value(ctx.getContext(), JS_EXCEPTION);
    }

    // Cloning these because the value representation types do not allow copying
    QuickJS::Value arg1 = args[0].clone();
    QuickJS::Value arg2 = args[1].clone();

    if (arg1.isNumber() && arg2.isNumber()) {
        int x = (int)arg1.asNumber();
        int y = (int)arg2.asNumber();

        return ctx.createNativeObject<Point>(x, y);
    }

    return QuickJS::Value(ctx.getContext(), JS_EXCEPTION);
}

int main() {
    QuickJS::Runtime jsRuntime = QuickJS::Runtime();
    QuickJS::Context jsContext = QuickJS::Context(jsRuntime);
    
    {
        QuickJS::ClassBinding<Point> pointBind = jsContext.createClassBinding<Point>("Point");

        pointBind.registerConstructor(Point::constructor, 2);
    }
    {
        QuickJS::Value v = jsContext.evalCode("let p = new Point(20, 40); p;");

        if (v.isObject()) {
            QuickJS::Object o = v.asObject();

            if (QuickJS::Pointer<Point>* p = o.getOpaque<Point>(); p) {
                std::cout << "Point { x: " << p->x << ", " << p->y << " }";
            }
        }
    }
    return 0;
}
```

# How to Use - Function Binding

In order to bind a function, do something like the following:

```cpp
#include "quickjs-raii/runtime.hpp"
#include "quickjs-raii/context.hpp"
#include "quickjs-raii/helpers.hpp"

#include <iostream>
#include <string>
#include <vector>


static std::string getObjectAsString(const QuickJS::Object& obj);
static std::string getValueAsString(const QuickJS::Value& value);

QUICKJS_FUNCTION_DECLARATION(log);

int main() {
    QuickJS::Runtime rt = QuickJS::Runtime();
    QuickJS::Context ctx = QuickJS::Context(rt);

    {
        QuickJS::Value console = ctx.createObject();

        {
            QuickJS::Value g = ctx.getGlobalObject();

            QuickJS::Object globals = g.asObject();

            globals.set("console", console);
        }

        QuickJS::Value funcLog = ctx.createFunction(log, "log");

        QuickJS::Object consoleObj = console.asObject();

        consoleObj.set("log", funcLog);

        QuickJS::Value res = ctx.evalCode("console.log(\"Hello, World!\")");
    }
    
    
    return 0;
}

QUICKJS_FUNCTION_IMPLEMENTATION(log)

QuickJS::Value logWrapped(
    QuickJS::Context& ctx, 
    QuickJS::Value& thisValue, 
    const std::vector<QuickJS::Value>& args
) {
    for (const QuickJS::Value& arg : args) {
        std::cout << getValueAsString(arg) << ",";
    }

    std::cout << std::endl;

    return ctx.createUndefined();
}

static std::string getValueAsString(const QuickJS::Value& value) {
    std::string output = "";
    
    switch (value.getTag()) {
        case QuickJS::Tag::Bool: {
            output = (value.asBoolean()) ? "true" : "false";
            break;
        }
        case QuickJS::Tag::Int: {
            output = std::to_string((int64_t)value.asNumber());
            break;
        }
        case QuickJS::Tag::Float: {
            output = std::to_string(value.asNumber());
            break;
        }
        case QuickJS::Tag::Null: {
            output = "null";
            break;
        }
        case QuickJS::Tag::Undefined: {
            output = "undefined";
            break;
        }
        case QuickJS::Tag::String: {
            output = value.asString();
            break;
        }
        case QuickJS::Tag::Object: {
            QuickJS::Object o = value.asObject();
            output = getObjectAsString(o);
            break;
        }
        default: {
            break;
        }
    }

    return output;
}

static std::string getObjectAsString(const QuickJS::Object& obj) {
    std::string output = "";

    QuickJS::Value f = obj.get("toString");

    if (f.isFunction()) {
        QuickJS::Function func = f.asFunction();

        QuickJS::Value objValue = obj.backToJSValue();

        QuickJS::Value res = func.call(objValue);

        if (res.isString()) {
            output = res.asString();
        }
    }

    return output;
}

```

# How to Use - Modules

For setting up modules, use code such as the following for C++-defined modules:
```cpp
#include "init_modules.hpp"
#include "quickjs-raii/helpers.hpp"
#include <unordered_map>

static std::unordered_map<std::string, QuickJS::ModuleDef> modules;

QUICKJS_MODULE_LOADER_DECLARATION(moduleLoader)

QUICKJS_MODULE_LOADER_IMPLEMENTATION(moduleLoader)

QuickJS::ModuleDef moduleLoaderWrapped(
    QuickJS::Context& ctx, const std::string& moduleName, void* opaque
) {
    return modules[moduleName];
}

QUICKJS_MODULE_NORMALIZER_DECLARATION(moduleNormalizer)

QUICKJS_MODULE_NORMALIZER_IMPLEMENTATION(moduleNormalizer)

std::string moduleNormalizerWrapped( 
    QuickJS::Context& ctx, 
    const std::string& moduleBaseName, 
    const std::string& moduleName, 
    void* opaque 
) {
    std::cout << "normalize: base="
              << moduleBaseName
              << ", name="
              << moduleName
              << '\n';

    return moduleName;
}

QUICKJS_MODULE_INIT_DECLARATION(moduleInit)

void init_modules(QuickJS::Context& ctx) {
    QuickJS::ModuleDef test = ctx.createModule("test", moduleInit);

    modules["test"] = test;

    int ret = 0;

    std::string foo = "foo";
    std::string bar = "bar";

    ret = test.addModuleExport(foo);

    if (ret != 0) {
        throw QuickJS::Exception("Export add failed!");
    }
    
    ret = test.addModuleExport(bar);

    if (ret != 0) {
        throw QuickJS::Exception("Export add failed!");
    }

    QuickJS::ModuleLoader loader = ctx.createModuleLoader();

    loader.setLoader(moduleLoader)
        .setNormalizer(moduleNormalizer)
        .submit();
}

QUICKJS_MODULE_INIT_IMPLEMENTATION(moduleInit)

int moduleInitWrapped(QuickJS::Context& ctx, QuickJS::ModuleDef& md) {
    int ret = 0;

    std::string foo = "foo";
    std::string bar = "bar";

    ret = md.exportValue(foo, ctx.createArray());
    
    if (ret != 0) {
        return ret;
    }
    
    ret = md.exportValue(bar, ctx.createObject());

    if (ret != 0) {
        return ret;
    }

    return 0;
}
```

# How to Use - Promises
JS Promises can be constructed in the following way from a function:
```cpp

#include "quickjs-raii/runtime.hpp"
#include "quickjs-raii/context.hpp"
#include "quickjs-raii/helpers.hpp"
#include "quickjs-raii/promise.hpp"

#include <iostream>
#include <string>
#include <vector>


static std::string getObjectAsString(const QuickJS::Object& obj);
static std::string getValueAsString(const QuickJS::Value& value);

QUICKJS_FUNCTION_DECLARATION(testPromise);

int main() {
    QuickJS::Runtime rt = QuickJS::Runtime();
    QuickJS::Context ctx = QuickJS::Context(rt);

    {
        QuickJS::Value global = ctx.getGlobalObject();

        QuickJS::Value funcTestPromise = ctx.createFunction(testPromise, "testPromise");

        QuickJS::Object globalObj = global.asObject();

        globalObj.set("testPromise", testPromise);

        // Do something with the promise in your script
    }
    
    
    return 0;
}

QUICKJS_FUNCTION_IMPLEMENTATION(testPromise)

QuickJS::Value testPromiseWrapped(
    QuickJS::Context& ctx, 
    QuickJS::Value& thisValue, 
    const std::vector<QuickJS::Value>& args
) {
    QuickJS::Promise p = ctx.createPromise();

    // handle promise resolve or reject with some work

    return p.toValue();
}

```