#pragma once

#include "Exception.hpp"



namespace tmn_exception{

class AllocException : public Exception::Exception{
private:
    std::string what_arg = "allocation exception";
public:
    explicit AllocException() noexcept = default;
    explicit AllocException(const Exception& other) noexcept: what_arg(other.what()) {}
    explicit AllocException(const std::string& what_arg) noexcept: what_arg(what_arg) {}
    explicit AllocException(const char* what_arg) noexcept: what_arg(what_arg) {}
};

}