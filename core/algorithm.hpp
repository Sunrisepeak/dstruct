#ifndef __ALGORITHM_HPP__DSTRUCT
#define __ALGORITHM_HPP__DSTRUCT

#include <types.hpp>
namespace dstruct {

namespace algorithm {

    template<typename T>
    void swap(T &a, T &b) {
        T c = dstruct::move(a);
        a = dstruct::move(b);
        b = dstruct::move(c);
    }

    template<typename Iterator, typename CallBack>
    void for_each(const Iterator &begin, const Iterator &end, CallBack cb) {
        for (auto it = begin; it != end; it++) {
            cb(*it);
        }
    }

}

}

#endif