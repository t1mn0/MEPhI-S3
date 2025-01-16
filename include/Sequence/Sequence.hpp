#pragma once

#include <cstdint>

namespace tmn {
namespace sequence {

template<typename T>
class Sequence {
public:
    virtual size_t size() const noexcept = 0;
    virtual bool empty() const noexcept = 0;

    virtual T& get(std::size_t index) = 0;
    virtual const T& get(std::size_t index) const = 0;

    virtual T& operator[](std::size_t index) noexcept = 0;
    virtual const T& operator[](std::size_t index) const noexcept = 0;

    virtual Sequence<T>& push_back(const T& value) = 0;
    virtual Sequence<T>& push_back(T&& value) = 0;
};

}
}