## DStruct: 静态内存分配器(SMA) 设计与实现

介绍**DStruct库**中的 **静态内存分配器 SMA** 的 设计与实现 及 相关的原理



### 一、涉及的知识

- **接口规范:** 分配器规范
- **数据结构:** 数组、嵌入式链表
- **基础概念:** 内存、内存管理、内存对齐、内存池、内存碎片、内存整理/合并、 分配器、分配效率、内存/分配器工具



### 二、目标

- 了解分配器规范
- 了解如何给一个数据结构设计专属的分配器Allocator
- 了解DStruct库中静态内存分配器(SMA)的及相关的设计原理



### 三、目录

#### 1.[静态内存分配器(SMA) 设计与实现 - 简单分配器](https://github.com/Sunrisepeak/DStruct/blob/main/docs/sma-design/sma-design-part1.md)

- 基础概念: 内存、分配器、分配器接口/规范
- 数据结构: 数组、内存块标记

#### 2.[静态内存分配器(SMA) 设计与实现 - 内存池和内存管理](https://github.com/Sunrisepeak/DStruct/blob/main/docs/sma-design/sma-design-part2.md)

- 基础概念: 内存对齐、内存池、内存碎片、内存管理、内存整理&合并
- 数据结构: 数组、链表

#### 3.静态内存分配器(SMA) 设计与实现 - "快速内存"/多级分配

- 基础概念:  固定内存块、分配效率、多级分配

- 数据结构: 数组、嵌入式链表

#### 4.静态内存分配器(SMA) 设计与实现 - 内存工具及畅想

- **dump内存信息**
- caller追踪
- 分配热点统计
- 内存踩踏检测



### 四、其他

- [Github](https://github.com/Sunrisepeak)

- [项目主页](https://github.com/Sunrisepeak/DStruct)

- [静态内存分配器SMA 源码](https://github.com/Sunrisepeak/DStruct/blob/main/memory/StaticMemAllocator.hpp)
