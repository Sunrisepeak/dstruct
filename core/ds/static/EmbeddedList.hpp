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
    struct _DoublyLink *prev, *next;

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

template<typename T, typename Link = _DoublyLink>
struct _EmbeddedList /* Node */ {
    Link link;
    T data;
public:
    using LinkType = Link;
public: // op
    static void init(_EmbeddedList *list) {
        init(to_link(list));
    }

    static void init(Link *link) {
        Link::init(link);
    }

    static bool empty(Link *link) {
        return Link::empty(link);
    }

    static void add(Link *prev, Link *curr) {
        Link::add(prev, curr);
    }

    static void del(Link *prev, Link *curr) {
        Link::del(prev, curr);
    }

    static _EmbeddedList * to_node(Link *link) {
        return reinterpret_cast<_EmbeddedList *>(link);
    }

    static Link * to_link(_EmbeddedList *node) {
        return &(node->link);
    }
};

}

#endif