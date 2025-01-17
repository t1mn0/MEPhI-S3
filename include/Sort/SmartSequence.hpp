#pragma once

#include <cstdint>
#include <memory>
#include <initializer_list>

#include "../Iterator/Iterator.hpp"
#include "../SmartPtr/UniquePtr.hpp"
#include "../Sequence/Sequence.hpp"
#include "../Sequence/ArraySequence.hpp"
#include "../../include/tmn.hpp"

namespace tmn {
namespace sequence {

inline std::size_t BASE_CAPACITY = 4;

template <typename T>
class SmartSequence : public Sequence<T> {
private:
    smart_ptr::UniquePtr<T[]> _array;
    std::size_t _size = 0;
    std::size_t _capacity = 0;

public:
// Using's :
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

// Constructors & assignment & conversion :
    SmartSequence();
    explicit SmartSequence(std::size_t _size);
    explicit SmartSequence(std::size_t _size, const T& default_element);
    SmartSequence(const SmartSequence<T>& other);
    SmartSequence(SmartSequence<T>&& other) noexcept;
    SmartSequence(const std::initializer_list<T>& lst);

    void swap(SmartSequence<T>& other);
    SmartSequence<T>& operator=(const SmartSequence<T>& other);

    ~SmartSequence();

// Capacity & size :
    std::size_t size() const noexcept override;
    std::size_t capacity() const noexcept;
    bool empty() const noexcept override;

    void reserve(std::size_t new_capacity);
    void resize(std::size_t new_size, const T& value = T());
    SmartSequence<T>& clear();

    SmartSequence<T>& push_back(const T& value) override;
    SmartSequence<T>& push_back(T&& value) override;
    SmartSequence<T>& pop_back();

    template <typename... Args>
    void emplace_back(Args... args);

// Element access methods :
    const T& front() const;
    T& front();
    const T& back() const;
    T& back();
    SmartSequence<T>& set(std::size_t index, const T& item);
    SmartSequence<T>& set(std::size_t index, T&& item);
    T& get(std::size_t index) override;
    const T& get(std::size_t index) const override;
    T& operator[](std::size_t index) noexcept override;
    const T& operator[](std::size_t index) const noexcept override;
};

}
}

#include "../../src/Sort/SmartSequence.tpp"