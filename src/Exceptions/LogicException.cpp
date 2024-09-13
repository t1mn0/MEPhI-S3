#include "../../include/Exceptions/LogicException.hpp"



namespace tmn_exception {

LogicException::LogicException() noexcept = default;
LogicException::LogicException(const Exception& other) noexcept: what_arg(other.what()) {}
LogicException::LogicException(const std::string& what_arg) noexcept: what_arg(what_arg) {}
LogicException::LogicException(const char* what_arg) noexcept: what_arg(what_arg) {}

}