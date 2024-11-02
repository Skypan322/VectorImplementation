First commit
## Overview

This project is a standard implementation of `std::vector` in C++. The `std::vector` is a sequence container that encapsulates dynamic size arrays.

## Features

- Dynamic array resizing
- Element access
- Iterators
- Capacity management
- Modifiers (insert, erase, push_back, pop_back, etc.)

## Getting Started


### Usage

Include the vector header in your C++ code:

```cpp
#include "vector.h"

int main() {
    s21::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (int i : vec) {
        std::cout << i << std::endl;
    }

    return 0;
}
```

