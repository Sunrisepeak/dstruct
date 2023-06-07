#ifndef __TYPES_HPP__DSTRUCT
#define __TYPES_HPP__DSTRUCT

namespace dstruct {
namespace types {

    template<typename T>
    struct RemoveReference {
        using Type = T;
    };

    template<typename T>
    struct RemoveReference<T &> {
        using Type = T;
    };

    template<typename T>
    struct RemoveConst {
        using Type = T;
    };

    template<typename T>
    struct RemoveConst<const T> {
        using Type = T;
    };
}
}

#endif