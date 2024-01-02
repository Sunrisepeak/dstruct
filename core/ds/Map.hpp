// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __MAP_HPP__DSTRUCT
#define __MAP_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/tree/AVLTree.hpp>

namespace dstruct {

template <typename KeyType, typename ValueType>
struct KeyValue {
    KeyType key;
    ValueType value;

    KeyValue() : key(), value() { }

    KeyValue(const KeyType &_key, const ValueType &_value) :
        key { _key }, value { _value } { }
};

// KeyValue
template <typename KVType, typename CMP>
struct KVCMPKey {
    KVCMPKey(CMP cmp = CMP()) : __mCMP { cmp } { }

    bool operator()(const KVType &a, const KVType &b) const {
        return __mCMP(a.key, b.key);
    }

private:
    CMP __mCMP;
};

template <
    typename KType, typename VType,
    typename KeyCMP = dstruct::less<KType>,
    typename DStruct = dstruct::AVLTree<
        KeyValue<const KType, VType>,
        KVCMPKey<KeyValue<const KType, VType>, KeyCMP>,
        dstruct::Alloc
    >
> class Map {

public:
    using ValueType            = VType;
    using KeyType              = KType;
    using ReferenceType        = ValueType &;
    using ConstReferenceType   = const ValueType &;
    using PointerType          = ValueType *;
    using ConstPointerType     = const ValueType *;
    using SizeType             = unsigned long long;
    using DifferenceType       = long long;
public:
    using KeyValueType         = typename DStruct::ValueType;
    using IteratorType         = typename DStruct::IteratorType;
    using ConstIteratorType    = typename DStruct::ConstIteratorType;

public: // big five
    Map() = default;
    Map(const Map &) = default;
    Map & operator=(const Map &) = default;
    Map(Map &&) = default;
    Map & operator=(Map &&) = default;
    ~Map() = default;

public: // Capacity
    bool empty() const {
        return _mDStruct.empty();
    }

    SizeType size() const {
        return _mDStruct.size();
    }

    SizeType capacity() const {
        return _mDStruct.capacity();
    }

public: // Access

    void push(const KeyValueType &element) {
        _mDStruct.push(element);
    }

    void pop(const KeyType &key) {
        _mDStruct.pop({key, ValueType()});
    }

    ConstReferenceType operator[](const KType &key) const {
        auto target = find(key);
        DSTRUCT_ASSERT(target != _mDStruct.end());
        return target->value;
    }

    ReferenceType operator[](const KType &key) {
        auto target = find(key);
        if (target == end()) {
            // TODO: optimize to only once find(by find_and_insert)
            _mDStruct.push({key, ValueType()});
            return dstruct::_remove_const(find(key)->value);
        }
        // convert from const ValueType to ValueType - remove const
        return dstruct::_remove_const(target->value);
    }

public:
    IteratorType find(const KeyType &key) {
        return _mDStruct.find(KeyValueType { key, ValueType() });
    }

    IteratorType erase(IteratorType &it) {
        return _mDStruct.erase(it);
    }

    void clear() {
        _mDStruct.clear();
    }

public:

    IteratorType begin() {
        return _mDStruct.begin();
    }

    IteratorType end() {
        return _mDStruct.end();
    }

    ConstIteratorType begin() const {
        return _mDStruct.begin();
    }

    ConstIteratorType end() const {
        return _mDStruct.end();
    }

protected:
    DStruct _mDStruct;
};


}

#endif