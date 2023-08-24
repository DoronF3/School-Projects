# VLVector

`VLVector` is a C++ template class that implements a variable-length array (VLA) vector. It provides similar functionality to `std::vector` and can dynamically switch between static and dynamic storage to optimize performance.

## Usage

1. Include the `VLVector.hpp` header in your C++ code.

```cpp
#include "VLVector.hpp"
```

2. Instantiate the VLVector class with your desired data type and static capacity (optional):

```cpp
VLVector<int, 100> staticVector; // Static capacity of 100
VLVector<double> dynamicVector;   // Dynamic capacity
```

3. Use the various methods provided by VLVector to manipulate and access elements:

```cpp
staticVector.push_back(42); // Add an element
dynamicVector.push_back(3.14);

int value = staticVector[0]; // Access elements
```

## Features
- Supports both static and dynamic storage based on capacity.
- Implements memory allocation strategies to optimize performance.
- Provides common methods like ```push_back```, ```pop_back```, ```at```, ```size```, ```capacity```, etc.
- Can be used similarly to ```std::vector```.
- Implements comparison operators ```==``` and ```!=``` to compare vectors.


## Const Random Access Iterator
The ```VLVector``` class also includes a const random access iterator, ```const_Iterator```, which allows you to traverse the vector in a range-based loop or using iterator arithmetic.
```cpp
for (auto it = vector.cbegin(); it != vector.cend(); ++it)
{
    // Access *it
}
```
