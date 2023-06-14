#ifndef __ALGORITHM_HPP__DSTRUCT
#define __ALGORITHM_HPP__DSTRUCT

#include <core/types.hpp>
namespace dstruct {

namespace algorithm {

    template<typename Iterator, typename CallBack>
    void for_each(const Iterator &begin, const Iterator &end, CallBack cb) {
        for (auto it = begin; it != end; it++) {
            cb(*it);
        }
    }

}

}

#endif