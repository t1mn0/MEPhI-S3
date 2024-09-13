#pragma once

#include "Exception.hpp"



namespace tmn_exception {

class IOException : public Exception::Exception {
private:
    std::string what_arg = "i/o exception";
public:
    explicit IOException() noexcept;
    explicit IOException(const Exception& other) noexcept;
    explicit IOException(const std::string& what_arg) noexcept;
    explicit IOException(const char* what_arg) noexcept;
};

}