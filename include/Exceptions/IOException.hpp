#pragma once

#include "Exception.hpp"



namespace tmn_exception{

class IOException : public Exception::Exception{
private:
    std::string what_arg = "i/o exception";
public:
    explicit IOException() noexcept = default;
    explicit IOException(const Exception& other) noexcept: what_arg(other.what()) {}
    explicit IOException(const std::string& what_arg) noexcept: what_arg(what_arg) {}
    explicit IOException(const char* what_arg) noexcept: what_arg(what_arg) {}
};

}