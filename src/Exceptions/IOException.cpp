#include "../../include/Exceptions/IOException.hpp"



namespace tmn_exception {

IOException::IOException() noexcept = default;
IOException::IOException(const Exception& other) noexcept: what_arg(other.what()) {}
IOException::IOException(const std::string& what_arg) noexcept: what_arg(what_arg) {}
IOException::IOException(const char* what_arg) noexcept: what_arg(what_arg) {}

std::string IOException::what() const noexcept { return what_arg; }

}