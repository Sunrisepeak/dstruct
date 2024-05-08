// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef EMBEDDED_LINKED_LIST_HPP_DSTRUCT
#define EMBEDDED_LINKED_LIST_HPP_DSTRUCT

namespace dstruct {

struct SinglyLink_ {
    struct SinglyLink_ *next;

public: // op
    static void init(SinglyLink_ *link) {
        link->next = link;
    }

    static bool empty(SinglyLink_ *link) {
        return link->next == link;
    }

    static void add(SinglyLink_ *prev, SinglyLink_ *curr) {
        curr->next = prev->next;
        prev->next = curr;
    }

    static void del(SinglyLink_ *prev, SinglyLink_ *curr) {
        prev->next = curr->next;
        curr->next = nullptr;
    }
};

struct DoublyLink_ {
    struct DoublyLink_ *next;
    struct DoublyLink_ *prev;

public: // op
    static void init(DoublyLink_ *link) {
        link->next = link->prev = link;
    }

    static bool empty(DoublyLink_ *link) {
        return link->next == link;
    }

    static void add(DoublyLink_ *prev, DoublyLink_ *curr) {
        curr->next = prev->next;
        curr->prev = prev;
        if (prev->next)
            prev->next->prev = curr;
        prev->next = curr;
    }

    static void del(DoublyLink_ *prev, DoublyLink_ *curr) {
        prev->next = curr->next;
        if (curr->next)
            curr->next->prev = prev;
        curr->prev = curr->next = nullptr;
    }
};

template <typename T, typename Link = DoublyLink_>
struct EmbeddedListNode_ /* Node */ {
    Link link;
    T data;
public:
    using LinkType = Link;
public: // op
    static void init(EmbeddedListNode_ *list) {
        Link::init(to_link(list));
    }

    static bool empty(EmbeddedListNode_ *list) {
        return Link::empty(to_link(list));
    }

    static void add(EmbeddedListNode_ *prev, EmbeddedListNode_ *curr) {
        Link::add(to_link(prev), to_link(curr));
    }

    static void del(EmbeddedListNode_ *prev, EmbeddedListNode_ *curr) {
        Link::del(to_link(prev), to_link(curr));
    }

    static EmbeddedListNode_ * to_node(Link *link) {
        return reinterpret_cast<EmbeddedListNode_ *>(link);
    }

    static Link * to_link(EmbeddedListNode_ *node) {
        return &(node->link);
    }
};

}

#endif