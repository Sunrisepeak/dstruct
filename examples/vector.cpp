#include <iostream>

#include "dstruct.hpp"

int main() {

    std::cout << "\n\nTesting: " << __FILE__ << std::endl;

    dstruct::Vector<int> vec(10, 2);

    for (auto v : vec) {     // 2. test const / range-for
        std::cout << v << " " << vec.size() << " " << vec.capacity() << std::endl;
    }

    std::cout << std::endl;

    decltype(vec)::ValueType val = 11;

    vec[0] = vec[5] = val;

    while (!vec.empty()) {
        std::cout << vec.back() << " " << vec.size() << " " << vec.capacity() << std::endl;
        vec.pop_back();
    }

    std::cout << std::endl;

    for (int i = 0; i < 10; i++) {
        vec.push_back(i);
        std::cout << vec.back() << " " << vec.size() << " " << vec.capacity() << std::endl;
    }

    return 0;
}