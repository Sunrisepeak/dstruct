// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __ALGORITHM_HPP__DSTRUCT
#define __ALGORITHM_HPP__DSTRUCT

#include <core/utils.hpp>
namespace dstruct {

namespace algorithm {

    template <typename Iterator, typename Callback>
    static void for_each(const Iterator &begin, const Iterator &end, Callback cb) {
        for (auto it = begin; it != end; it++) {
            cb(*it);
        }
    }

    template <typename Iterator, typename T>
    static Iterator find(const Iterator &begin, const Iterator &end, T obj) {
        for (auto it = begin; it != end; it++) {
            if (*it == obj) {
                return it;
            }
        }
        return end;
    }
}

}

#endif