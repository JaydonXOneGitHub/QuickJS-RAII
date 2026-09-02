#pragma once

#include <exception>
#include <string>

namespace QuickJS {
    class Exception : public std::exception {
    private:
        std::string message;

    public:
        Exception(const std::string& msg);

        const char* what() const noexcept override;
    };
}