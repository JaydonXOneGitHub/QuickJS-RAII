# QuickJS-RAII
A custom QuickJS wrapper designed for idiomatic C++

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
Coming soon!