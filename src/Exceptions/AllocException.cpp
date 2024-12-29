#include "../../include/Exceptions/AllocException.hpp"



namespace tmn {
namespace exception {

AllocException::AllocException() noexcept = default;
AllocException::AllocException(const Exception& other) noexcept: what_arg(other.what()) {}
AllocException::AllocException(const std::string& what_arg) noexcept: what_arg(what_arg) {}
AllocException::AllocException(const char* what_arg) noexcept: what_arg(what_arg) {}

std::string AllocException::what() const noexcept { return what_arg; }

}
}