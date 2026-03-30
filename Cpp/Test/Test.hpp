//
// Created by Stefan on 29.03.2026.
//

#pragma once
#include <concepts>
#include <numeric>
#include <ranges>

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

inline int test(){
    using namespace std::ranges::views;
    std::vector<int> Numbers(100);
    std::iota(Numbers.begin(), Numbers.end(), 0);

    std::for_each(
        std::execution::par_unseq,
        Numbers.begin(), Numbers.end(),
            Log
        );

    return 0;
}