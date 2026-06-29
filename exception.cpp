#include "exception.hpp"

QuickJS::Exception::Exception(const std::string& msg) {
    this->message = msg;
}

const char* QuickJS::Exception::what() const noexcept {
    return this->message.c_str();
}