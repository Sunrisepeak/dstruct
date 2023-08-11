#ifndef __TYPES_HPP__DSTRUCT
#define __TYPES_HPP__DSTRUCT

namespace dstruct {
namespace types {

    template <typename T>
    struct RemoveReference {
        using Type = T;
    };

    template <typename T>
    struct RemoveReference<T &> {
        using Type = T;
    };

    template <typename T>
    struct RemoveConst {
        using Type = T;
    };

    template <typename T>
    struct RemoveConst<const T> {
        using Type = T;
    };

    template <typename T>
    struct IsPointer {
        const static bool value = false;
    };

    template <typename T>
    struct IsPointer<T *> {
        const static bool value = true;
    };
}
}

#endif