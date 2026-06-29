# QuickJS-RAII
A custom QuickJS wrapper designed for idiomatic C++

# Prerequisites
<ui>
    <li>You must have compiled a static or dynamic library of QuickJS (preferably version "2020-07-05" - if you use a different version that is incompatible with this API, do so at your own risk).</li>
    <li>You must have a C++ compiler that is compatible with C++17 or later.<li>
</ui>

# Additional Notes
I have not gotten around to adding properties for custom JS classes yet, although this can be replicated by having `getX` and `setX` methods rather than a single property `x`, for example.<br>
If any of the following bother you enough, please submit a request or feel free to fork yourself, provided you're comfortable dealing with a C API underneath. :relaxed:

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

            if (Point* p = o.getOpaque<Point>(); p) {
                std::cout << "Point { x: " << p->x << ", " << p->y << " }";
            }
        }
    }
    return 0;
}
```