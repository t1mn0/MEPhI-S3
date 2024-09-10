#pragma once

#include <string>



namespace tmn_exception{

class Exception{
private:
    std::string what_arg = "exception";
public:
    explicit Exception() noexcept = default;
    explicit Exception(const Exception& other) noexcept: what_arg(other.what_arg) {}
    explicit Exception(const std::string& what_arg) noexcept: what_arg(what_arg) {}
    explicit Exception(const char* what_arg) noexcept: what_arg(what_arg) {}

    virtual std::string what() const noexcept { 
        return what_arg;
    }
};

}