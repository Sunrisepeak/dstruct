// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#include <iostream>
#include <map>

#include <dstruct.hpp>

int main() {

    std::cout << "\nTesting: " << __FILE__;

    { // base-test
        dstruct::UFSet ufSet(6);

        DSTRUCT_ASSERT(ufSet.size() == 6);
        DSTRUCT_ASSERT(ufSet.size() == ufSet.count());

        // merge by union root
        ufSet.union_set(0, 1);
        ufSet.union_set(2, 3);
        // merge by connect element of subset
        ufSet.connect(0, 3);

        // connect test
        DSTRUCT_ASSERT(ufSet.connected(1, 2));
        DSTRUCT_ASSERT( !(ufSet.connected(1, 4)) );

        int key = ufSet.size() - 1;

        DSTRUCT_ASSERT( !(ufSet.connected(1, key)) );
        
        //ufSet.union_set(1, key); // error-op: union directly
        //ufSet.connect(1, key); // ok 
        int root1 = ufSet.find(1);
        int root2 = ufSet.find(key);
        if (root1 != root2)
            ufSet.union_set(root1, root2);

        DSTRUCT_ASSERT(ufSet.connected(1, key));
    }

    {// example base key-value
        std::map<std::string, int> city { // key-value: id-name
            {"Shanghai", 0},
            {"Hangzhou", 1},
            {"NewYork", 2},
            {"Beijing", 3},
            {"Anhui", 4},
            {"Suzhou", 5}
        };
        dstruct::UFSet ufSet(city.size());      // key

        // connect (NewYork) to (Hangzhou)
        ufSet.union_set(city["NewYork"], city["Hangzhou"]);
        // connect (Anhui) to (Suzhou)
        ufSet.connect(city["Anhui"], city["Suzhou"]);

        if (false == ufSet.connected(city["NewYork"], city["Beijing"])) {
            ufSet.connect(city["NewYork"], city["Beijing"]);
        }

        // connect (NewYork, Hangzhou, Beijing) to (Suzhou, Anhui)
        ufSet.connect(city["Hangzhou"], city["Suzhou"]);

        DSTRUCT_ASSERT(ufSet.connected(city["Anhui"], city["Beijing"]));
    }

    std::cout << "   pass" << std::endl;

    return 0;
}