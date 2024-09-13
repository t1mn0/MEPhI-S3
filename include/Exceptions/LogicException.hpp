#pragma once

#include "Exception.hpp"



namespace tmn_exception{

class LogicException : public Exception::Exception {
private:
    std::string what_arg = "logic exception";
public:
    explicit LogicException() noexcept;
    explicit LogicException(const Exception& other) noexcept;
    explicit LogicException(const std::string& what_arg) noexcept;
    explicit LogicException(const char* what_arg) noexcept;
};

}