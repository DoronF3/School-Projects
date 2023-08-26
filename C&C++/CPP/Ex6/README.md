# HashMap Class

The `HashMap` class is an implementation of a hash map container in C++. It provides a way to store key-value pairs and efficiently retrieve values based on their corresponding keys using hash functions.

## Table of Contents

- [Overview](#overview)
- [Usage](#usage)
- [Public Member Functions](#public-member-functions)
- [Exceptions](#exceptions)
- [Iterator](#iterator)
- [License](#license)

## Overview

The `HashMap` class is a template class that can store arbitrary types of keys and values. It uses a hash function to determine the index in an array (referred to as buckets) where each key-value pair is stored. This allows for fast retrieval of values based on keys.

## Usage

To use the `HashMap` class, you need to include the `HashMap.hpp` header in your code. You can create instances of the `HashMap` class by specifying the key and value types, for example:

```cpp
#include "HashMap.hpp"

int main() {
    HashMap<int, std::string> myMap;
    // Insert key-value pairs, retrieve values, etc.
    return 0;
}
```

## Public Member Functions

The `HashMap` class provides several member functions to interact with the hash map:

- `size()`: Returns the number of elements in the map.
- `capacity()`: Returns the current capacity of the map.
- `empty()`: Returns whether the map is empty.
- `insert(key, value)`: Inserts a key-value pair into the map.
- `containsKey(key)`: Checks if a given key is present in the map.
- `at(key)`: Returns the value associated with a given key.
- `erase(key)`: Removes a key-value pair from the map.
- `getLoadFactor()`: Returns the current load factor of the map.
- `bucketSize(key)`: Returns the size of the bucket in which the key is stored.
- `bucketIndex(key)`: Returns the index of the bucket in which the key is stored.
- `clear()`: Removes all elements from the map.
- `operator==`, `operator!=`: Compare two maps for equality or inequality.
- `begin()`, `end()`, `cbegin()`, `cend()`: Iterator functions to iterate over the map's key-value pairs.

## Exceptions

The `HashMap` class provides two exception classes:

- `SizeException`: Thrown when the sizes of the key and value vectors provided during construction are not equal.
- `KeyException`: Thrown when an operation involves a key that is not present in the map.

## Iterator

The `HashMap` class includes a nested `const_Iterator` class that allows you to iterate over the elements in the map. You can use iterators to traverse the map in a range-based for loop, for example:
```cpp
HashMap<int, std::string> myMap;
// Insert key-value pairs into myMap
for (const auto& pair : myMap) {
    std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
}
```
