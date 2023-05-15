#include <iostream>

#include "dstruct.hpp"


int main() {

    dstruct::Array<int, 10> arr(2); // 0. test cntor

    decltype(arr)::ValueType val = 6;

    arr[0] = arr[-1] = val;       // 1. test subscript access & assignment

    const auto backup = arr;
    for (auto v : backup) {     // 2. test const / range-for
        std::cout << v << " ";
    }

    std::cout << std::endl << arr[-1] << std::endl;

    for (int i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    for (int i = 0; i < arr.size(); i++) { // 3.  positive / negative subscript access
        std::cout << backup[-(i + 1)] << " : " << arr[i] << std::endl;
    }

    return 0;
}