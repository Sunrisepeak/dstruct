// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __EMBEDDED_LINKED_LIST_HPP__DSTRUCT
#define __EMBEDDED_LINKED_LIST_HPP__DSTRUCT

namespace dstruct {

struct _SinglyLink {
    struct _SinglyLink *next;

public: // op
    static void init(_SinglyLink *link) {
        link->next = link;
    }

    static bool empty(_SinglyLink *link) {
        return link->next == link;
    }

    static void add(_SinglyLink *prev, _SinglyLink *curr) {
        curr->next = prev->next;
        prev->next = curr;
    }

    static void del(_SinglyLink *prev, _SinglyLink *curr) {
        prev->next = curr->next;
        curr->next = nullptr;
    }
};

struct _DoublyLink {
    struct _DoublyLink *next;
    struct _DoublyLink *prev;

public: // op
    static void init(_DoublyLink *link) {
        link->next = link->prev = link;
    }

    static bool empty(_DoublyLink *link) {
        return link->next == link;
    }

    static void add(_DoublyLink *prev, _DoublyLink *curr) {
        curr->next = prev->next;
        curr->prev = prev;
        if (prev->next)
            prev->next->prev = curr;
        prev->next = curr;
    }

    static void del(_DoublyLink *prev, _DoublyLink *curr) {
        prev->next = curr->next;
        if (curr->next)
            curr->next->prev = prev;
        curr->prev = curr->next = nullptr;
    }
};

template <typename T, typename Link = _DoublyLink>
struct _EmbeddedListNode /* Node */ {
    Link link;
    T data;
public:
    using LinkType = Link;
public: // op
    static void init(_EmbeddedListNode *list) {
        Link::init(to_link(list));
    }

    static bool empty(_EmbeddedListNode *list) {
        return Link::empty(to_link(list));
    }

    static void add(_EmbeddedListNode *prev, _EmbeddedListNode *curr) {
        Link::add(to_link(prev), to_link(curr));
    }

    static void del(_EmbeddedListNode *prev, _EmbeddedListNode *curr) {
        Link::del(to_link(prev), to_link(curr));
    }

    static _EmbeddedListNode * to_node(Link *link) {
        return reinterpret_cast<_EmbeddedListNode *>(link);
    }

    static Link * to_link(_EmbeddedListNode *node) {
        return &(node->link);
    }
};

}

#endif