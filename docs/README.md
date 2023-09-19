# DStruct Documents

## Start

### Get Source Code

```bash
git clone git@github.com:Sunrisepeak/DStruct.git
```



### Configuration Include Path

#### xmake

#### GNU Make

#### CMake

#### Visual Sudio



### Coding - Write Hello DStruct

```cpp
#include <iostream>

#include <dstruct.hpp> // DStruct

int main() {
    dstruct::Array<int, 10> arr(2);

    arr[0] = arr[-1] = 6;

    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[-(i + 1)] << " : " << arr[i] << std::endl;
    }

    return 0;
}
```



### Build & Run

#### Linux

> use compiler `-I` to include DStruct's root dir

```bash
g++ -I yourPath/DStruct HelloDStruct.cpp -o HelloDStruct && ./HelloDStruct
```

#### Windows

#### macOS



## Data Structures

### Lists

> Lists, Stacks, Queues is ADT -- **logical structure**
>
> Array and Linked-List is specific implement for Lists -- **physical structure**
>
> Stacks and Queues are **restricted Lists**(impl by array or linked-list)

- **Arrays**
  - Array
  - Vector
- **Linked-List**
  - EmbeddedList
  - SinglyLinkedList
  - DoublyLinkedList
- **Stacks**
  - Stack
  - XValueStack
- **Queues**
  - Deque
  - Queue





## Algorithms





## DStruct Spec

- [V0.0.1](DStruct-Spec.md)





## Other

- [DStruct- Github](https://github.com/Sunrisepeak/DStruct)
