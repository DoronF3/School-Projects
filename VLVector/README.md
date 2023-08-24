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
- Provides both ```const_Iterator``` and ```Iterator``` for iterating over the vector.


## Iterator and const_Iterator
The ```VLVector``` class includes two types of iterators: ```Iterator``` and ```const_Iterator```. These iterators allow you to traverse the vector and perform various operations:

- ```operator++``` and ```operator--``` for moving the iterator.
- ```operator+```, ```operator-```, ```operator+=```, and ```operator-=``` for arithmetic operations on iterators.
- ```operator==``` and ```operator!=``` for comparison.
- ```operator*``` and ```operator->``` for dereferencing the iterator.
- ```operator[]``` for accessing elements using index.
- Various comparison operators like ```<```, ```>```, ```<=```, and ```>=``` for comparison between iterators.

## Erase and Insert
The ```VLVector``` class provides methods for erasing and inserting elements at a specific position using iterators:

```cpp
Iterator it = vector.begin();
it = vector.erase(it);       // Erase element at the iterator position
it = vector.insert(it, val); // Insert a value at the iterator position
```

## License
This project is licensed under the [MIT License](LICENSE).
Feell free to use and modify this class according to your needs.
