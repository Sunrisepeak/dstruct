// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

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