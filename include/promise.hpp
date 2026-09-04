#pragma once

#include "quickjs.h"
#include "clonable.hpp"
#include "value.hpp"

namespace QuickJS {
    class Promise final : public Clonable<Promise> {
    private:
        JSValue resolveFunc;
        JSValue rejectFunc;
        JSValue promise;
        JSContext* ctx;

    private:
        void destroy();

    public:
        Promise() = default;
        Promise(JSContext* ctx);
        ~Promise();

        Promise(const Promise&) = delete;
        Promise(Promise&& other) noexcept;
        Promise& operator=(Promise&& other) noexcept;

        Promise clone() const override;
        std::string toString() const override;

        void resolve(const QuickJS::Value& value);
        void reject(const QuickJS::Value& error);

        QuickJS::Value toValue() const;
    };
}