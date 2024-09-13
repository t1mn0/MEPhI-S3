#include "../../include/Exceptions/AllocException.hpp"



namespace tmn_exception{

AllocException::AllocException() noexcept = default;
AllocException::AllocException(const Exception& other) noexcept: what_arg(other.what()) {}
AllocException::AllocException(const std::string& what_arg) noexcept: what_arg(what_arg) {}
AllocException::AllocException(const char* what_arg) noexcept: what_arg(what_arg) {}

}