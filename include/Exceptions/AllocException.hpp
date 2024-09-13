#pragma once

#include "Exception.hpp"



namespace tmn_exception {

class AllocException : public Exception::Exception {
private:
    std::string what_arg = "allocation exception";
public:
    explicit AllocException() noexcept;
    explicit AllocException(const Exception& other) noexcept;
    explicit AllocException(const std::string& what_arg) noexcept;
    explicit AllocException(const char* what_arg) noexcept;
};

}