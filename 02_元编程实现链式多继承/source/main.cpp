#include <format>
#include <iostream>
#include <vector>
using namespace std;

struct A {
    A(string_view str) { cout << str << endl; }
};

struct B {
    B(int num) { cout << "Number: " << num << endl; }
};

struct C {
    C(double val) { cout << "Value: " << val << endl; }
};

struct D {
    D(char ch) { cout << "Char: " << ch << endl; }
};

namespace details {

template <class T, class... Rest>
struct compose_impl : T, compose_impl<Rest...> {
    compose_impl() = default;
    compose_impl(const T& t, const Rest&... rest) : T(t), compose_impl<Rest...>(rest...) {}
};

template <class T>
struct compose_impl<T> : T {
    compose_impl() = default;
    compose_impl(const T& t) : T(t) {}
};

template <class... Args>
struct compose : compose_impl<Args...> {
    compose() = default;
    compose(const Args&... args) : compose_impl<Args...>(args...) {}
};

template <>
struct compose<> {
    compose() = default;
};

}  // namespace details

template <class... Args>
struct object : details::compose<Args...> {
    object() = delete;
    object(const object&) = delete;
    object& operator=(const object&) = delete;
    object(object&&) = default;
    object& operator=(object&&) = default;

    object(const Args&... args) : details::compose<Args...>(args...) {}
};

using Iface = object<A, B, C, D>;

int main() {
    Iface i(A("Hello"), B(123), C(3.14), D('X'));

    return 0;
}