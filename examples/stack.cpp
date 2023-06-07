#include <iostream>

#include "dstruct.hpp"


int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::Stack<int /*, dstruct::Vector<T> */ > stack1;
    dstruct::adapter::Stack<int, dstruct::Deque<int>> stack2;

    for (int i = 0; i < 100; i++) {
        stack1.push(i);
        stack2.push(i);
    }

    for (int i = 0; i < 100; i++) {
        int v1 = stack1.top();
        int v2 = stack2.top();
        //std::cout << v1 << "-" << v2 << std::endl;
        DSTRUCT_ASSERT(v1 == v2);
        stack1.pop();
        stack2.pop();
    }

    DSTRUCT_ASSERT(stack1.size() == 0);
    DSTRUCT_ASSERT(stack1.size() == stack2.size());

    std::cout << "   pass" << std::endl; 

    return 0;
}