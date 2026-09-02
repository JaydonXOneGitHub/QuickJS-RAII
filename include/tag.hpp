#pragma once

namespace QuickJS {
    enum class Tag: int {
        Unknown = -3,
        Undefined,
        Null,
        Bool,
        Int,
        Float,
        Object,
        String,
        Array,
        Function,
        Module, 
        Exception,
        Symbol,
        BigInt,
        BigFloat,
        BigDecimal
    };
}