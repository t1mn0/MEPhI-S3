#pragma once

#include "Exception.hpp"



namespace tmn_exception{

class RuntimeException : public Exception::Exception{
private:
    std::string what_arg = "runtime exception";
public:
    explicit RuntimeException() noexcept = default;
    explicit RuntimeException(const Exception& other) noexcept: what_arg(other.what()) {}
    explicit RuntimeException(const std::string& what_arg) noexcept: what_arg(what_arg) {}
    explicit RuntimeException(const char* what_arg) noexcept: what_arg(what_arg) {}
};

}