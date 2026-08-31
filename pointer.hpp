#pragma once

namespace QuickJS {
    template<typename T>
    class Pointer {
    private:
        T* ptr;

    public:
        explicit Pointer(T* ptr) {
            this->ptr = ptr;
        }

        T* operator->() const {
            return this->ptr;
        }

        operator bool() const {
            return this->ptr != nullptr;
        }

        T& operator*() {
            return (*(this->ptr));
        }
    };
}