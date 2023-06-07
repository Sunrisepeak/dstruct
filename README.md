# DStruct

DStruct 是一个易于移植且结构简洁的数据结构模板库

[**English**](README.en.md)

## 特性/目标

- 易于移植，没有依赖项（只需要实现 dstruct-port.h）
- 易于使用（数据结构和算法分离设计，统一接口，std标准库风格）
- 易于学习/定制和改进（结构简洁且只有头文件）
- 现代 C++ / 泛型结构，包含多样丰富的数据结构



## 数据结构列表

| Category |           Name            | Status |
| :------: | :-----------------------: | ------ |
|          |           Array           |        |
|          |          Vector           |        |
|          |   EList(Embedded List)    |        |
|          | DList(Doubly Linked List) |        |
|          |  Deque(DoubleEndedQueue)  |        |
|          |           Queue           |        |
|          |           Stack           |        |
|          |                           |        |

> 注: 功能开发中, 性能暂时未测试和优化



  ## 示例

  > 测试用例 / Demo

|                 DStruct                  | Status |
| :--------------------------------------: | :----: |
|       [Array](examples/array.cpp)        |        |
|      [Vector](examples/vector.cpp)       |        |
|       [Deque](examples/deque.cpp)        |        |
|   [EList](examples/embedded_list.cpp)    |        |
| [DList](examples/double_linked_list.cpp) |        |
|       [Queue](examples/queue.cpp)        |        |
|       [Stack](examples/stack.cpp)        |        |
|                                          |        |



  ### 构建和运行示例

  ```
  curl -fsSL https://xmake.io/shget.text | bash # 安装 xmake
  xmake   # 编译/构建
  xmake r dstruct_vector # 运行 dstruct_vector 示例，更多细节请查看xmake.lua
  ```

  

  ## 其他

  - [Github](https://github.com/Sunrisepeak/DStruct)

