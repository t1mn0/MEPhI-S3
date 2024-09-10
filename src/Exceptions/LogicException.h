#pragma once

#include "Exception.h"



namespace tmn_exception{

class LogicException : public Exception::Exception{
private:
    std::string what_arg = "logic exception";
public:
    explicit LogicException() noexcept = default;
    explicit LogicException(const Exception& other) noexcept: what_arg(other.what()) {}
    explicit LogicException(const std::string& what_arg) noexcept: what_arg(what_arg) {}
    explicit LogicException(const char* what_arg) noexcept: what_arg(what_arg) {}
};

}