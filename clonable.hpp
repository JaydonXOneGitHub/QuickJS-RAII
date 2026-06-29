#pragma once

namespace QuickJS {
    template<typename T>
    class Clonable {
    public:
        virtual T clone() const = 0;
    };
}