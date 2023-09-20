// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#include <iostream>

#include <dstruct.hpp>


int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::Array<int, 10> arr;
    dstruct::Vector<int> vec(10, 0);
    dstruct::Queue<int> queue;

    for (int i = 0; i < 10; i++) {
        arr[i] = i;
        vec[i] = i;
        queue.push(i);
    }

    dstruct::algorithm::for_each(arr.begin(), arr.end(), [](int &obj) { obj *= 2; } );
    dstruct::algorithm::for_each(vec.begin(), vec.end(), [](int &obj) { obj *= 3; } );
    //dstruct::algorithm::for_each(queue.begin(), queue.end(), [](int &obj) { obj += 1; } ); // err: queue it is a const-it/only-read

    int arrVecQueueSum { 0 }; 

    dstruct::algorithm::for_each(queue.begin(), queue.end(),
        [ & ](int obj) {
            //              queue      arr       vec
            arrVecQueueSum += obj + obj * 2 + obj * 3;
        }
    );

    for (int i = 0; i < 10; i++) {
        //std::cout << queue[i] << "-" << arr[i] << "-" << vec[i] << std::endl;
        arrVecQueueSum -= queue[i] + arr[i] + vec[i];
    }

    //std::cout << arrVecQueueSum << std::endl;

    DSTRUCT_ASSERT(arrVecQueueSum == 0);

    std::cout << "   pass" << std::endl; 

    return 0;
}