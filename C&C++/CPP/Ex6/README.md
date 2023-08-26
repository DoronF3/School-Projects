# HashMap Class

The `HashMap` class is an implementation of a hash map container in C++. It provides a way to store key-value pairs and efficiently retrieve values based on their corresponding keys using hash functions.

## Table of Contents

- [HashMap Usage](#HashMap-Usage)
- [Public Member Functions](#public-member-functions)
- [Exceptions](#exceptions)
- [Iterator](#iterator)
- [SpamDetector](#SpamDetector)
- [SpamDetector Usage](#SpamDetector-Usage)
- [Program Explanation](#program-explanation)
- [Dependencies](#dependencies)

## HashMap Usage

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

# SpamDetector

The `SpamDetector` is a program that uses the `HashMap` class to detect spam messages in a given message file. It reads a database file containing words and their associated spam scores, and then analyzes the given message to determine if it's spam based on the accumulated scores of the words in the message.

## SpamDetector Usage

To use the `SpamDetector` program, you need to compile it along with the `HashMap` class and the required Boost libraries. The program expects three command-line arguments:

1. Path to the database file containing words and their associated spam scores.
2. Path to the message file to be analyzed for spam.
3. Threshold value - an integer that determines the spam threshold.

Example usage:

```sh
./SpamDetector database.txt message.txt 100
```

## Program Explanation

The program performs the following steps:

- First, it checks if the correct number of command-line arguments has been provided. If not, it displays an error message and exits.

- Next, it verifies if the threshold provided is a valid positive integer. If not, it displays an error message and exits.

- The program also checks if the provided file paths exist and ensures that the threshold value is consistent in terms of its digits and positivity.

- The `parser` function reads the database file, parsing the content line by line, and then populating the `HashMap` with words and their corresponding spam scores. If the input is invalid, the program displays an error message and exits.

- The `checkSpam` function reads the message file, converts the content to lowercase, and searches for words from the `HashMap` in the message. It accumulates the spam scores for matching words and compares the total score against the threshold. Depending on the score, it prints whether the message is spam or not.

- The `main` function coordinates the overall program execution. It reads the database file, creates the `HashMap`, analyzes the message, and outputs the result.

## Dependencies

The program depends on the following components:

- The `HashMap` class provided in the `HashMap.hpp` header.
- The Boost C++ libraries, particularly `boost/filesystem.hpp` and `boost/tokenizer.hpp`.
