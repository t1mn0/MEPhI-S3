#pragma once

#include "Exception.hpp"



namespace tmn_exception {

class RuntimeException : public Exception::Exception {
private:
    std::string what_arg = "runtime exception";
public:
    explicit RuntimeException() noexcept;
    explicit RuntimeException(const Exception& other) noexcept;
    explicit RuntimeException(const std::string& what_arg) noexcept;
    explicit RuntimeException(const char* what_arg) noexcept;
};

}