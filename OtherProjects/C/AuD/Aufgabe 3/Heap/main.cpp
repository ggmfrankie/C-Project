//
// Created by ggmfrankie on 11/2/26.
//

#include "Heap.hpp"

int main() {

    int df = std::max({3, 2, 32, 32232332});
    Heap heap {1, 2, 3, 4, 25, 435, 234};

}

using Part = std::pair<int, int>;

std::vector<Part> parts;

int best(int price, int count, int ziel) {
    int bestPrice = price;
    for (auto p : parts) {
        if (count + p.second <= ziel) {
            bestPrice = std::max(best(price + p.first, count + p.second, ziel), bestPrice);
        }
    }
    return bestPrice;
}
