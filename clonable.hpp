#pragma once

#include <string>

namespace QuickJS {
    template<typename T>
    class Clonable {
    public:
        virtual T clone() const = 0;
        virtual std::string toString() const = 0;
    };
}