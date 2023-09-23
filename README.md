# DStruct

DStruct 是一个易于移植且结构简洁的数据结构模板库

| [特性](#特性) - [用法](#用法) - [谁在用?](#谁在用) - [其他](#其他) - [English](README.en.md) |
| ------------------------------------------------------------ |
| [接口规范](docs/DStruct-Spec.md) - [静态内存分配器(SMA)](docs/sma-design) - 移植介绍 - [文档](docs) - 相关视频 |


---


## 特性

- 易于移植, 不依赖标准库(std)和其它三方库
- 易于使用, 数据结构和算法分离设计，统一接口
- 易于学习/定制和改进
- 可自定义数据结构分配器
- 支持裸机/小内存设备, 提供简单的静态内存管理/分配器SMA
- 现代 C++ / 泛型结构，包含多样丰富的数据结构



## 用法

### 1. 源码及配置

- 下载源码到本地
- 添加DStruct库的根目录到你的include的路径
- 自定义分配器&实现dstruct-port.h接口 - **可选**

### 2. 代码用例 - 动态内存

```cpp
#include <iostream>
#include <dstruct.hpp>

int main() {
    dstruct::Array<int, 10> arr(2);

    decltype(arr)::ValueType val = 6;
    arr[0] = arr[-1] = val;

    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[-(i + 1)] << " : " << arr[i] << std::endl;
    }

    return 0;
}
```

### 3. 代码用例 - 静态内存

> 使用静态内存SMA, 支持无内存管理环境(如: 裸机)

```cpp
#include <dstruct.hpp>

int main() {
    //dstruct::Vector<int> dVec;
    dstruct::smemory::Vector<int> sVec;

    for (int i = 0; i < 10; i++) {
        sVec.push_back(i);
    }

    for (auto v : sVec)
        DSTRUCT_ASSERT(v == i++);

    while (!sVec.empty()) {
        sVec.pop_back();
    }

    return 0;
}
```

**注: 静态数据结构定义在`dstruct::smemory`空间里, 其他接口及用法同动态内存支持的数据结构一样**



## 谁在用?

- [**DSVisual**：](https://github.com/Sunrisepeak/DSVisual) 数据结构可视化库

![](https://github.com/Sunrisepeak/DSVisual/blob/main/docs/imgs/DSVisual.profile.png)

- [**KHistory**：](https://github.com/Sunrisepeak/KHistory) 跨平台的按键检测/历史记录工具

![](https://github.com/Sunrisepeak/KHistory/blob/main/docs/imgs/khistory-gamepad.demo.gif)



## 其他

  - [Github](https://github.com/Sunrisepeak)
  - [项目主页](https://github.com/Sunrisepeak/DStruct)