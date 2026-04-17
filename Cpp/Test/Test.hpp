//
// Created by Stefan on 29.03.2026.
//

#pragma once
#include <concepts>
#include <numeric>
#include <ranges>
#include <cmath>

template<class F>
auto calculate(F f) -> int
    requires std::invocable<F, int> {
    using namespace std::ranges::views;
    auto it = iota(0, 5)
        | transform(f)
        | common;
    return std::reduce(it.begin(), it.end());
}

static int f(int x) {
    return x + x;
}

#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <execution>

inline void Log(int Number){
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    std::cout << std::this_thread::get_id() << " " << Number << '\n';
}

struct cursed {
    std::ostream* os;

    template<typename T>
    cursed operator%(T&& v) {
        *os << std::forward<T>(v);
        return *this;
    }
};

template<typename F>
struct callable_wrapper {
    F f;
};

template<typename F>
std::ostream& operator<<(std::ostream& os, callable_wrapper<F> w) {
    return w.f(os);
}

namespace awesome_shit::jej {
    struct numberHolder {
        double d;

        std::ostream& operator()(std::ostream& os, int) const {
            return os << static_cast<int>(d);
        }

        std::ostream& operator()(std::ostream& os, float) const {
            return os << static_cast<float>(d);
        }

        std::ostream& operator()(std::ostream& os, double) const {
            return os << d;
        }

        std::ostream& operator()(std::ostream& os, const char*) const {
            std::function<std::string(int)> to_words = [&](int n)->std::string {
                static const std::array<std::string, 20> ones = {
                    "zero","one","two","three","four","five","six","seven","eight","nine",
                    "ten","eleven","twelve","thirteen","fourteen","fifteen",
                    "sixteen","seventeen","eighteen","nineteen"
                };

                static const std::array<std::string, 10> tens = {
                    "", "", "twenty","thirty","forty","fifty",
                    "sixty","seventy","eighty","ninety"
                };

                if (n < 20) return ones[n];

                if (n < 100) {
                    return tens[n / 10] + (n % 10 ? ones[n % 10] : "");
                }

                if (n < 1000) {
                    return ones[n / 100] + "hundred" +
                           (n % 100 ? to_words(n % 100) : "");
                }

                return "toolarge"; // placeholder
            };
            return os << to_words(d);
        }
    };
}

inline int test() {

    namespace standard = std;

    auto& c_output = standard::cout;
    const auto& c_end_of_line_and_flush_buffer_pls = static_cast<std::ostream& (*)(std::ostream&)>(std::endl);

    int* ptr = nullptr;
    int** o = &ptr;
    int* zeroPtr = reinterpret_cast<int *>(o);
    int zero = *zeroPtr;

#define IotaARGS Numbers.begin(), Numbers.end(), zero

    int one = exp(zero);
    int two = one << one;
    int ten = (two*two*two) +two;

    int oneHundred = ten*ten;
    auto cursed_callable = callable_wrapper{
        [](std::ostream& os) -> std::ostream& {
            return os << "cursed\n";
        }
    };
    c_output << ten << c_end_of_line_and_flush_buffer_pls << cursed_callable;

    awesome_shit::jej::numberHolder apHOLD{5};
    apHOLD(c_output, "give me the string representation pls");

    cursed{&c_output} % 10 % "\n";

    using namespace standard::ranges::views;
    standard::vector<int> Numbers(oneHundred);
    standard::iota(IotaARGS);

    standard::for_each(standard::execution::par_unseq,
        Numbers.begin(), Numbers.end(),
            Log
    );

    struct X { const int n; };
    union U { X x; float f; };

    U u = {{ 1 }};

    X *p = new (&u.x) X {2};
    return 0;
#include "ClosingBrace.cpp"