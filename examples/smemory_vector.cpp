#include <dstruct.hpp>

int main() {
    //dstruct::Vector<int> dVec;
    dstruct::smemory::Vector<int> sVec;

    for (int i = 0; i < 10; i++) {
        sVec.push_back(i);
    }

    int i = 0;
    for (auto v : sVec) {
        DSTRUCT_ASSERT(v == i++);
    }

    while (!sVec.empty()) {
        sVec.pop_back();
    }

    return 0;
}