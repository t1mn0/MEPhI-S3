#include "../../include/Exceptions/RuntimeException.hpp"

namespace tmn {
namespace exception {

RuntimeException::RuntimeException() noexcept = default;
RuntimeException::RuntimeException(const Exception& other) noexcept: what_arg(other.what()) {}
RuntimeException::RuntimeException(const std::string& what_arg) noexcept: what_arg(what_arg) {}
RuntimeException::RuntimeException(const char* what_arg) noexcept: what_arg(what_arg) {}

std::string RuntimeException::what() const noexcept { return what_arg; }

}
}