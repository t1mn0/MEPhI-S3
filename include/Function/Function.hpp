#pragma once

#include "../SmartPtr/UniquePtr.hpp"
#include "../Exceptions/LogicException.hpp"

namespace tmn {

template <typename... Args>
class Function;

template <typename ReturnType, typename... Args>
class Function<ReturnType (Args...)> {
private:
// Support structures & methods :
    struct Wrapper {
    public:
        Wrapper() = default;
        virtual ~Wrapper() = default;

        virtual ReturnType call(Args&&...) = 0;
        virtual void move_by_ptr(void* to) = 0;
        virtual void small_copy(void* to) const = 0;
        virtual smart_ptr::UniquePtr<Wrapper> full_copy() const = 0;
    };

    template <typename F>
    struct FunctionWrapper : Wrapper {
    private:
        F f_object;
    public:
        FunctionWrapper(const F& f_object) : f_object(f_object) {};
        FunctionWrapper(F&& f_object) : f_object(std::move(f_object)) {};

        ~FunctionWrapper() = default;

        ReturnType call(Args&&... args) override;
        void move_by_ptr(void* to) override;
        void small_copy(void* to) const override;

        smart_ptr::UniquePtr<Wrapper> full_copy() const override;

    };

    void clear();

private:
// Small object optimization
    static constexpr int BUFFER_SIZE = 64;
    bool is_object_small;

    uint8_t buffer[BUFFER_SIZE];

// Classis fields (not for small objects)
    smart_ptr::UniquePtr<Wrapper> big_object_ptr;

public:
    Function(std::nullptr_t) noexcept : is_object_small(false), big_object_ptr(nullptr) {}
    Function() noexcept : Function(nullptr) {}
    Function(const Function& other_function);
    Function(Function&& other_function);

    template <typename F>
    Function(F f);

    ~Function();

    Function& operator=(const Function& other_function) = delete;
    Function& operator=(Function&& other_function);

    void swap(Function& other_function) noexcept;

    explicit operator bool() const noexcept;
    ReturnType operator()(Args... args);
};

}

#include "../../src/Function/Function.tpp"