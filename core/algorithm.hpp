#ifndef __ALGORITHM_HPP__DSTRUCT
#define __ALGORITHM_HPP__DSTRUCT

#include <core/types.hpp>
namespace dstruct {

namespace algorithm {

    template<typename Iterator, typename Callback>
    static void for_each(const Iterator &begin, const Iterator &end, Callback cb) {
        for (auto it = begin; it != end; it++) {
            cb(*it);
        }
    }
}

}

#endif