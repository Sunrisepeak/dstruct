# DStruct
an easily portable data structure template library...



## Features/Target

- Ease to port, no dependencies (only need to impl [dstruct-port.h](port/dstruct-port.h))
- Diverse and rich data structure
- Ease to use(dstruct and algo separation design, unified interfaces, similar std style)
- Header-only and easy to custom and improve
- Modern C++ / Generics Structure



## Data Structure Lists

| Category |           Name            | Status         |
| :------: | :-----------------------: | -------------- |
|          |           Array           | basic function |
|          |          Vector           | basic function |
|          |   EList(Embedded List)    | basic function |
|          | DLinkedList(Doubly Linked List) | basic function |
|          |  Deque(DoubleEndedQueue)  | basic function |
|          |           Queue           | basic function |
|          |           Stack           | basic function |
|          |                           |                |



## Examples

> test case / demo

|                 DStruct                  | Status |
| :--------------------------------------: | :----: |
|       [Array](examples/array.cpp)        |        |
|      [Vector](examples/vector.cpp)       |        |
|       [Deque](examples/deque.cpp)        |        |
|   [EList](examples/embedded_list.cpp)    |        |
| [DLinkedList](examples/double_linked_list.cpp) |        |
|       [Queue](examples/queue.cpp)        |        |
|       [Stack](examples/stack.cpp)        |        |
|                                          |        |



### build and run a demo

~~~bash
curl -fsSL https://xmake.io/shget.text | bash # install xmake
xmake   # build
xmake r dstruct_vector # r dstruct_vector demo, more details in xmake.lua
~~~



## Other

- [Github](https://github.com/Sunrisepeak/DStruct)
