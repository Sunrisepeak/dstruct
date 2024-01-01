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

    dstruct::Map<char, int> charToIntMapTable;

    charToIntMapTable['a'] = 97;
    charToIntMapTable['b'] = 98;
    charToIntMapTable['c'] = 99;
    charToIntMapTable['a'] = 'a';

    DSTRUCT_ASSERT(charToIntMapTable.size() == 3);

    for (auto kv : charToIntMapTable) {
        DSTRUCT_ASSERT(kv.key == kv.value);
        //std::cout << kv.key << " - " << kv.value << std::endl;
    }

    // test auto-add & default value
    int sum = charToIntMapTable['a'] + charToIntMapTable['d'];
    DSTRUCT_ASSERT(sum = charToIntMapTable['a']);
    DSTRUCT_ASSERT(charToIntMapTable.size() == 4);

    std::cout << "   pass" << std::endl;

    return 0;
}