#include "../../include/Exceptions/Exception.hpp"



namespace tmn_exception{

Exception::Exception() noexcept = default;
Exception::Exception(const Exception& other) noexcept: what_arg(other.what_arg) {}
Exception::Exception(const std::string& what_arg) noexcept: what_arg(what_arg) {}
Exception::Exception(const char* what_arg) noexcept: what_arg(what_arg) {}
Exception::~Exception() = default;

std::string Exception::what() const noexcept { return what_arg; }

}