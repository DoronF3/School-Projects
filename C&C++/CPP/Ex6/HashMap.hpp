
#ifndef SOLUTION_HASHMAP_HPP
#define SOLUTION_HASHMAP_HPP

#include <vector>
#include <stdlib.h>

#define UPPER 0.75
#define LOWER 0.25
#define DOUBLE 1
#define HALF 0

/**
 * A class that represents size exception.
 */
class SizeException : std::exception
{
public:
    const char *what() const noexcept override
    {
        return "The size of the vectors is not equal.";
    };
};

/**
 *  A class that represents an exception of a key not being in the map.
 */
class KeyException : std::exception
{
public:
    const char *what() const noexcept override
    {
        return "The given key is not in the map.";
    };
};

template<class K, class V>

/**
 * A class that implements a hash map.
 * @tparam K The type of the given keys.
 * @tparam V The type of the given values.
 */
class HashMap
{

public:

    typedef std::vector<std::pair<K, V>> bucket;

    /**
     * Default C'tor.
     */
    HashMap<K, V>() : _size(0), _capacity(16)
    {
        _arr = new bucket[16];
    };

    /**
     * A method that creates the hash map and adds keys and values from the given vectors.
     * @param vecK A vector of keys.
     * @param vecV A vector of values.
     */
    HashMap<K, V>(std::vector<K> vecK, std::vector<V> vecV) : HashMap()
    {
        if (vecK.size() != vecV.size())
        {
            throw SizeException();
        }
        for (int i = 0; i < (int) vecK.size(); i++)
        {
            (*this)[vecK[i]] = vecV[i];
        }
    };

    /**
     * Copy C'tor.
     * @param map The given map to copy.
     */
    HashMap<K, V>(const HashMap &map)
    {
        _capacity = map.capacity();
        _arr = new bucket[_capacity];
        _size = map._size;
        for (int i = 0; i < _capacity; i++)
        {
            _arr[i] = map._arr[i];
        }
    }

    /**
     * Default D'tor.
     */
    ~HashMap<K, V>()
    {
        clear();
        delete[](_arr);
    }

    /**
     * A method that returns the size of the map.
     * @return The size of the map.
     */
    int size() const
    {
        return _size;
    }

    /**
     * A method that returns the capacity of the map.
     * @return The capacity of the map.
     */
    int capacity() const
    {
        return _capacity;
    }

    /**
     * A method that returns if the map is empty.
     * @return True if its empty, false otherwise.
     */
    bool empty() const
    {
        return _size == 0;
    }

    /**
     * A method that inserts the key and value to the map.
     * @param key The key to insert.
     * @param val The corresponding value.
     * @return True if inserted successfully, false otherwise.
     */
    bool insert(K key, V val)
    {
        if (containsKey(key))
        {
            return false;
        }
        int num = std::hash<K>()(key);
        int index = num & (_capacity - 1);
        _arr[index].push_back(std::make_pair(key, val));
        _size++;
        if (getLoadFactor() > UPPER)
        {
            _resize(DOUBLE);
        }
        return true;
    };

    /**
     * A method that checks if a given key is in the map.
     * @param key The given key to check.
     * @return True if its in the map, false otherwise.
     */
    bool containsKey(const K &key) const
    {
        if (empty())
        {
            return false;
        }
        int num = std::hash<K>()(key);
        int index = num & (_capacity - 1);
        for (int i = 0; i < (int) _arr[index].size(); i++)
        {
            if (key == _arr[index][i].first)
            {
                return true;
            }
        }
        return false;
    };

    /**
     * A method that returns the value of the given key.
     * @param key The given key to check.
     * @return The value if the key is in the map, otherwise throws exception.
     */
    V at(const K &key) const
    {
        try
        {
            for (int i = 0; i < bucketSize(key); i++)
            {
                if (key == (_arr[bucketIndex(key)])[i].first)
                {
                    return (_arr[bucketIndex(key)])[i].second;
                }
            }

            throw KeyException();
        }
        catch (KeyException &)
        {
            throw;
        }
    }

    /**
     * A method that returns a reference to the value of the given key.
     * @param key The given key to check.
     * @return The reference of the value if the key is in the map, otherwise throws exception.
     */
    V &at(const K &key)
    {
        try
        {
            for (int i = 0; i < bucketSize(key); i++)
            {
                if (key == (_arr[bucketIndex(key)])[i].first)
                {
                    return (_arr[bucketIndex(key)])[i].second;
                }
            }

            throw KeyException();
        }
        catch (KeyException &)
        {
            throw;
        }
    }

    /**
     * A method that removes a key and its value from the map.
     * @param key The given key to remove.
     * @return True if succeeded, false otherwise.
     */
    bool erase(K key) noexcept
    {

        int num = std::hash<K>()(key);
        int index = num & (_capacity - 1);
        for (auto iter = _arr[index].begin(); iter != _arr[index].end(); iter++)
        {
            if (iter->first == key)
            {
                _arr[index].erase(iter);
                _size--;
                while (capacity() > 1 && getLoadFactor() < LOWER)
                {
                    _resize(HALF);
                }
                return true;
            }
        }
        return false;
    }

    /**
     * A method that returns the current load factor.
     * @return The current load factor.
     */
    double getLoadFactor() const
    {
        return (double) _size / (double) _capacity;
    }

    /**
     * A method that returns the size of the bucket in which the key is at.
     * @param key The given key to check,
     * @return The given size if the key is in the map, otherwise throw exception.
     */
    int bucketSize(const K &key) const
    {
        try // The key is in the map.
        {
            return _arr[bucketIndex(key)].size();
        }
        catch (KeyException &) // The key isn't in the map.
        {
            throw;
        }
    };

    /**
     * A method that returns the index of the bucket in which the given key resides in.
     * @param key The given key to check.
     * @return The index of the bucket if the key is in the map, otherwise throw exception.
     */
    int bucketIndex(const K &key) const
    {
        if (containsKey(key))
        {
            int num = std::hash<K>()(key);
            return num & (_capacity - 1);
        }
        throw KeyException(); // The key isn't in the map.
    };

    /**
     * A method that removes all the map members.
     */
    void clear()
    {
        for (int i = 0; i < _capacity; i++)
        {
            _arr[i].clear();
        }
        _size = 0;
    }

    /**
     * A method that puts the given map inside a new one.
     * @param map The map we wish to get.
     * @return The reference to the map we got.
     */
    HashMap &operator=(const HashMap &map)
    {
        if (*this == map)
        {
            return *this;
        }
        delete[](_arr);
        _size = map._size;
        _capacity = map._capacity;
        _arr = new bucket[_capacity];
        for (int i = 0; i < _capacity; i++)
        {
            _arr[i] = map._arr[i];
        }
        return *this;
    }

    /**
     * A method that returns the value of the given key.
     * @param key The given key to check.
     * @return The value of the given key if exists, otherwise default value.
     */
    const V operator[](const K key) const
    {
        if (!containsKey(key))
        {
            return V(); // Default value.
        }
        return at(key);
    }

    /**
     * A method that returns the value of the given key if exists, otherwise adds it to the map.
     * @param key The given key to check.
     * @return The value of the given key.
     */
    V &operator[](const K key)
    {
        if (!containsKey(key))
        {
            insert(key, V());
        }
        return at(key);
    }

    /**
     * A method that checks if the given maps does hold the same pairs.
     * @param map The given map to compare with.
     * @return True if they are equal, false otherwise.
     */
    bool operator==(const HashMap &map) const
    {
        if (_size != map.size() || (_capacity != map.capacity() && (!empty() || !map.empty())))
        {
            return false;
        }
        if (_arr == map._arr || (empty() && map.empty()))
        {
            return true;
        }
        for (auto pair : map)
        {
            if (!containsKey(pair.first) || at(pair.first) != map.at(pair.first))
            {
                return false;
            }
        }
        return true;
    }

    /**
     * A method that checks if the given maps doesn't hold the same pairs.
     * @param map The given map to compare with.
     * @return True if they aren't equal, false otherwise.
     */
    bool operator!=(const HashMap &map) const
    {
        return !(*this == map);
    }

    /**
     * A class that implements an iterator for the hash map.
     */
    class const_Iterator
    {

    public:

        typedef std::pair<K, V> value_type;
        typedef std::pair<K, V> *pointer;
        typedef std::pair<K, V> &reference;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;

        /**
         * Default C'tor.
         */
        const_Iterator(bucket *arr, int size, int capacity, bool flag) : \
        _sizeIt(size), _capacityIt(capacity), _pointer(nullptr), _flag(flag), _arrIt(arr)
        {
            _index.first = 0;
            _index.second = 0;
            if (arr == nullptr)
            {
                return;
            }
            if (_sizeIt == 0) // No elements.
            {
                return;
            }
            if (_flag)
            {
                _pointer = nullptr;
                return;
            }
            while ((_index.first < _capacityIt - 1) && (arr[_index.first].size() == 0))
            {
                _index.first++;
            }
            _pointer = &arr[_index.first][0];
        }

        /**
         * Default D'tor.
         */
        ~const_Iterator() = default;

        /**
         * A method that returns the pair that the iterator points to.
         * @return The pair that the iterator points to.
         */
        reference operator*() const
        {
            return (*_pointer);
        }

        /**
         * A method that returns the pointer to the current pair.
         * @return The pointer to the current pair.
         */
        pointer operator->() const
        {
            return _pointer;
        }

        /**
         * A method that returns a reference to the current pair.
         * @return A reference to the iterator.
         */
        const_Iterator &operator++()
        {
            if ((_pointer != nullptr) && (_index.second == (int) _arrIt[_index.first].size() - 1))
            { // End of bucket, but not of the map.
                if (_index.first != _capacityIt - 1)
                {
                    _index.first++;
                }
                else
                {
                    _pointer = nullptr;
                    return *this;
                }
                while ((_index.first < _capacityIt - 1) && (_arrIt[_index.first].size() == 0))
                {
                    _index.first++;
                }
                _index.second = 0;
                if (_arrIt[_index.first].size() == 0) // No more elements.
                {
                    _pointer = nullptr;
                }
                else
                {
                    _pointer = &_arrIt[_index.first][0];
                }

            }
            else if (_index.second != (int) _arrIt[_index.first].size() - 1)
            { // Mid bucket.
                _index.second++;
                _pointer = &_arrIt[_index.first][_index.second];
            }
            return *this;
        }

        /**
         * A method that returns the iterator and then advances is to point to the next pair.
         * @return A reference to the iterator.
         */
        const_Iterator operator++(difference_type)
        {
            const_Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * A method that returns true if two iterators point to the same place in the map.
         * @param other The iterator to compare with.
         * @return True if they point to the same place, false otherwise.
         */
        bool operator==(const const_Iterator &other) const
        {
            return (_pointer == other._pointer);
        }

        /**
         * A method that returns false if two iterators point to the same place in the map.
         * @param other The iterator to compare with.
         * @return False if they point to the same place, true otherwise.
         */
        bool operator!=(const const_Iterator &other) const
        {
            return !(*this == other);
        }

    private:

        int _sizeIt;
        int _capacityIt;
        std::pair<int, int> _index;
        pointer _pointer;
        bool _flag;
        bucket *_arrIt;
    };

    /**
     * A method that returns an iterator to the first element of the list.
     * @return An iterator to the first element of the list.
     */
    const_Iterator begin() const
    {
        return const_Iterator(_arr, _size, _capacity, false);
    }

    /**
     * A method that returns an iterator to the last element of the list.
     * @return An iterator to the last element of the list.
     */
    const_Iterator end() const
    {
        return const_Iterator(_arr, _size, _capacity, true);
    }

    /**
     * A method that returns an iterator to the first element of the list.
     * @return An iterator to the first element of the list.
     */
    const_Iterator cbegin() const
    {
        return begin();
    }

    /**
     * A method that returns an iterator to the last element of the list.
     * @return An iterator to the last element of the list.
     */
    const_Iterator cend() const
    {
        return end();
    }

private:

    int _size{};
    int _capacity{};
    bucket *_arr{};

    /**
     * A method that changes the capacity of the map.
     * @param val A flag for doubling or halving the map capacity.
     */
    void _resize(const int val)
    {
        int tmpcapacity = 0;
        if (val == DOUBLE)
        {
            tmpcapacity = _capacity * 2;
        }
        else
        {
            tmpcapacity = _capacity * 0.5;
        }
        auto *tmpArr = new bucket[tmpcapacity];
        for (auto pair : *this)
        {
            int num = std::hash<K>()(pair.first);
            int index = num & (tmpcapacity - 1);
            tmpArr[index].push_back(pair);
        }
        delete[](_arr);
        _arr = tmpArr;
        _capacity = tmpcapacity;
    }
};

#endif //SOLUTION_HASHMAP_HPP
