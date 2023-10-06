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

    dstruct::UFSet ufSet(6);

    DSTRUCT_ASSERT(ufSet.size() == ufSet.count());

    // union-op
    ufSet.union_set(0, 1);
    ufSet.union_set(2, 3);
    ufSet.connect(0, 3);

    // connect test
    DSTRUCT_ASSERT(ufSet.connected(1, 2));
    DSTRUCT_ASSERT( !(ufSet.connected(1, 4)) );

    int setNumber = ufSet.count();
    ufSet.push();

    DSTRUCT_ASSERT(setNumber + 1 == ufSet.count());

    int key = ufSet.size() - 1;

    DSTRUCT_ASSERT( !(ufSet.connected(1, key)) );
    
    //ufSet.union_set(1, key); // error-op: union directly
    //ufSet.connect(1, key); // ok 
    int root1 = ufSet.find(1);
    int root2 = ufSet.find(key);
    if (root1 != root2)
        ufSet.union_set(root1, root2);

    DSTRUCT_ASSERT(ufSet.connected(1, key));

    std::cout << "   pass" << std::endl;

    return 0;
}