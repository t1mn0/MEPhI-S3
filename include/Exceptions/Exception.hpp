#pragma once

#include <string>



namespace tmn {
namespace exception {

class Exception{
private:
    std::string what_arg = "exception";
public:
    explicit Exception() noexcept;
    Exception(const Exception& other) noexcept;
    explicit Exception(const std::string& what_arg) noexcept;
    explicit Exception(const char* what_arg) noexcept;
    virtual ~Exception();

    std::string what() const noexcept;
};

}
}