## dynarray

Dynamic array library for C++ but simpler than `std::vector`.

## Library
It supports Makefile and premake5 only.

## Usage
```cpp
#include <iostream>

// DO NOT USE 'dynarray/array.hpp' because it will
// throw a linkage error!
#include "dynarray/core.hpp"

using namespace dynarray;

int main() {
    Array<int> array;

    array.push(10);
    array.pop();

    auto len = array.len();
    if (len == 0) {
        std::cout << "Array is empty!" << std::endl;
    }

    return 0;
}
```
