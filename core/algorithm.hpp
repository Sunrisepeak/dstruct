#ifndef __ALGORITHM_HPP__DSTRUCT
#define __ALGORITHM_HPP__DSTRUCT

namespace dstruct {

namespace algorithm {

    template<typename T>
    void swap(T &a, T &b) {
        T c = dstruct::move(a);
        a = dstruct::move(b);
        b = dstruct::move(c);
    }

}

}

#endif