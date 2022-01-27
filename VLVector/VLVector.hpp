#include <iostream>
#include <stdexcept>

#ifndef VLVECTOR_VLVECTOR_HPP
#define VLVECTOR_VLVECTOR_HPP
#define DEFAULT_SIZE 16
#define DYNAMIC_CAP (3 * (_size + 1) / 2)
#define TO_STATIC -1
#define TO_DYNAMIC 1
#define DYNAMIC_INC 0

template<class T, int StaticCapacity = DEFAULT_SIZE>

/**
 * A class that represents a VLA vector.
 * @tparam T The type of objects inside the vector.
 * @tparam StaticCapacity The maximal capacity of the static vector.
 */
class VLVector
{
public:

    /**
     * The default C'tor.
     */
    VLVector() = default;

    /**
     * A C'tor that receives two input iterators and adds the values in the range to the vector.
     * @tparam InputIterator The iterator type we get.
     * @param first An iterator pointing to the first element in the range.
     * @param last  An iterator pointing to the last element in the range.
     */
    template<class InputIterator>
    VLVector(InputIterator first, InputIterator last)
    {
        while (first != last)
        {
            push_back(*first);
            first++;
        }
    }

    /**
     * Move C'tor.
     * @param other A given vector.
     */
    VLVector(VLVector<T, StaticCapacity> && other) noexcept
    {
        swap(*this, other);
    }

    /**
     * Copy C'tor
     * @param other The given vector to copy.
     */
    VLVector(VLVector<T, StaticCapacity> &other)
    {
        _size = other._size;
        if (_size > StaticCapacity)
        {
            _dynamic_vec = new T[other._dynamic_cap];
            _dynamic_cap = other._dynamic_cap;
            for (int i = 0; i < _size; i++)
            {
                *(_dynamic_vec + i) = *(other._dynamic_vec + i);
            }
        }
        else
        {
            for (int i = 0; i < _size; i++)
            {
                *(_static_vec + i) = *(other._static_vec + i);
            }
        }
    }

    /**
     * A method that implements the equal operator.
     * @param other The given vector to copy.
     * @return The copied object.
     */
    VLVector &operator=(VLVector<T, StaticCapacity> other)
    {
        swap(*this, other);
        return *this;
    }

    /**
     * A method to swap between to vectors.
     * @param first The given vector to receive values.
     * @param second The given vector to copy.
     */
    friend void swap(VLVector<T, StaticCapacity> &first, VLVector<T, StaticCapacity> &second) noexcept
    {
        std::swap(first._size, second._size);
        std::swap(first._dynamic_cap, second._dynamic_cap);
        std::swap(first._dynamic_vec, second._dynamic_vec);
        std::swap(first._static_vec, second._static_vec);
    }


    /**
     * A method that returns the amount of objects in the vector.
     * @return The amount of objects in the vector.
     */
    size_t size() const
    {
        return _size;
    }

    /**
     * A method that returns the maximal capacity of the vector.
     * @return The maximal capacity of the vector.
     */
    size_t capacity() const
    {
        if (_size > StaticCapacity)
        {
            return _dynamic_cap;
        }
        return StaticCapacity;
    }

    /**
     *  A method that receives a value and adds it to the end of the vector.
     * @param val The given value to add.
     */
    void push_back(const T &val)
    {
        if (_size <= StaticCapacity) // Static vector.
        {
            if (_size == StaticCapacity) // Check threshold.
            {
                _resize(TO_DYNAMIC); // Resize to dynamic.
                _dynamic_vec[_size] = val; // Add value.
                _size++;
                return;
            }
            _static_vec[_size] = std::move(val); // Threshold wasn't reached.
            _size++;
        }
        else // Dynamic vector.
        {
            if (_size + 1 > _dynamic_cap) // Check threshold.
            {
                _resize(DYNAMIC_INC); // Increase size.
            }
            _dynamic_vec[_size] = val; // Add value.
            _size++;
        }
    }

    /**
     * A method that receives a R-value and adds it to the end of the vector.
     * @param val The given R-value to add.
     */
    void push_back(T && val)
    {
        if (_size <= StaticCapacity) // Static vector.
        {
            if (_size == StaticCapacity) // Check threshold.
            {
                _resize(TO_DYNAMIC); // Resize to dynamic.
                _dynamic_vec[_size] = std::move(val); // Add value.
                _size++;
                return;
            }
            _static_vec[_size] = std::move(val); // Threshold wasn't reached.
            _size++;
        }
        else // Dynamic vector.
        {
            if (_size + 1 > _dynamic_cap) // Check threshold.
            {
                _resize(DYNAMIC_INC); // Increase size.
            }
            _dynamic_vec[_size] = std::move(val); // Add value.
            _size++;
        }
    }

    /**
     * A method that checks if the vector is empty.
     * @return True if it is, False otherwise.
     */
    bool empty() const
    {
        if (_size == 0)
        {
            return true;
        }
        return false;
    }

    /**
     * A method gets an index and returns the value of the vector or throws exception.
     * @param i The given index of the vector to check.
     * @return The reference to the given value assigned to the index, otherwise throws and exception.
     */
    T &at(const int i)
    {
        if (i > _size - 1 || i < 0)
        {
            throw std::out_of_range("The index doesn't exists.");
        }
        else if (_size < StaticCapacity)
        {
            return _static_vec[i];
        }
        return _dynamic_vec[i];
    }

    /**
     * A method gets an index and returns the value of the vector or throws exception.
     * @param i The given index of the vector to check.
     * @return The const reference to the given value assigned to the index, otherwise throws and exception.
     */
    const T &at(const int i) const
    {
        if (i > _size || i < 0)
        {
            throw std::out_of_range("The index doesn't exists.");
        }
        else if (_size < StaticCapacity)
        {
            return _static_vec[i];
        }
        return _dynamic_vec[i];
    }

    /**
     * A method that removes the last value inside the vector.
     */
    void pop_back() noexcept
    {
        if (_size == 0)
        {
            return;
        }
        _size--;
        if (_size < StaticCapacity && _size + 1 >= StaticCapacity) // Check if threshold was reached.
        {
            _resize(TO_STATIC); // Do _resize to static.
        }
    }

    /**
     *  A method that removes all the values from the vector.
     */
    void clear()
    {
        if (_size == 0)
        {
            return;
        }
        if (_size > StaticCapacity) // Check if dynamic.
        {
            delete[] _dynamic_vec;
            _dynamic_cap = 0;
            _dynamic_vec = nullptr;
        }
        _size = 0;
    }

    /**
     * A method that returns a pointer to the current data structure.
     * @return A pointer to the current data structure.
     */
    const T *data() const
    {
        if (_size > StaticCapacity) // Check if dynamic.
        {
            return _dynamic_vec;
        }
        return _static_vec;
    }

    /**
     * A method that returns a pointer to the current data structure.
     * @return A pointer to the current data structure.
     */
    T *data()
    {
        if (_size > StaticCapacity) // Check if dynamic.
        {
            return _dynamic_vec;
        }
        return _static_vec;
    }

    /**
     * A method that implements the operator [].
     * @param num The given index of the vector to check.
     * @return The reference to the object or garbage.
     */
    T &operator[](const int num) noexcept
    {
        return *(data() + num);
    }

    /**
     * A method that implements the operator [] const.
     * @param num The given index of the vector to check.
     * @return The const reference to the object or garbage.
     */
    const T &operator[](const int num) const noexcept
    {
        return *(data() + num);
    }

    /**
     * A method that implements the operator ==.
     * @param v2 The vector to compare with.
     * @return True if they are the same, false otherwise.
     */
    bool operator==(const VLVector<T, StaticCapacity> &v2) const
    {
        if (_size != v2._size)
        {
            return false;
        }
        for (int i = 0; i < _size; ++i)
        {
            if (*(data() + i) != *(v2.data() + i))
            {
                return false;
            }
        }
        return true;
    }

    /**
     * A method that implements the operator !=.
     * @param v2 The vector to compare with.
     * @return False if they are the same, true otherwise.
     */
    bool operator!=(const VLVector<T, StaticCapacity> &v2) const
    {
        return !(*this == v2);
    }

    /**
     * D'tor to the VLV object.
     */
    ~VLVector()
    {
        if (_size >= StaticCapacity)
        {
            delete[] _dynamic_vec;
        }
    }

    /**
     * A class representing the const Random access iterator to the VLV class.
     */
    class const_Iterator
    {
        typedef T value_type;
        typedef const T *pointer;
        typedef const T &reference;
        typedef int difference_type;
        typedef std::random_access_iterator_tag iterator_category;

    public:

        /**
         * The default C'tor.
         */
        const_Iterator(pointer p, int size, int index) : _pointer(p), _sizeIt(size), _indexIt(index){}

        /**
         * The copy C'tor.
         * @param other The iterator to copy.
         */
        const_Iterator(const const_Iterator &other)
        {
            _indexIt = other._indexIt;
            _sizeIt = other._sizeIt;
            _pointer = other._pointer;
        }

        /**
         * The default D'tor.
         */
        ~const_Iterator() = default;

        /**
         * A method that implements the equal operator.
         * @param other The iterator to copy.
         * @return The iterator we copied to.
         */
        const_Iterator &operator=(const const_Iterator &other)
        {
            if (*this != other)
            {
                _indexIt = other._indexIt;
                _sizeIt = other._sizeIt;
                _pointer = other._pointer;
            }
            return *this;
        }

        /**
         * A method that implements the dereference operator.
         * @return A const reference to the pointed value.
         */
        reference operator*() const noexcept
        {
            return (*(_pointer + _indexIt));
        }

        /**
         * A method that implements the operator ->.
         * @return A const pointer to the value.
         */
        pointer operator->() const noexcept
        {
            return _pointer + _indexIt;
        }

        /**
         * A method that implements the operator ++ from the right.
         * @return An iterator to the next object.
         */
        const_Iterator &operator++() noexcept
        {
            if (_indexIt < _sizeIt)
            {
                _indexIt++;
                return *this;
            }
            return *this;
        }

        /**
         * A method that implements the operator ++ from the left.
         * @return An iterator to the object before we increased.
         */
        const const_Iterator operator++(difference_type) noexcept
        {
            const_Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * A method that implements the operator ==.
         * @param other The given iterator to check.
         * @return True if they are the same, false otherwise.
         */
        bool operator==(const const_Iterator &other) const noexcept
        {
            return (_pointer + _indexIt == other._pointer + other._indexIt);
        }

        /**
         * A method that implements the operator !=.
         * @param other The given iterator to check.
         * @return False if they are the same, true otherwise.
         */
        bool operator!=(const const_Iterator &other) const noexcept
        {
            return !(*this == other);
        }

        /**
         * A method that implements the operator -- from the right.
         * @return An iterator to the object before.
         */
        const_Iterator &operator--() noexcept
        {
            if (_indexIt == 0)
            {
                return *this;
            }
            _indexIt--;
            return *this;
        }

        /**
         * A method that implements the operator -- from the left.
         * @return An iterator to the object before we decremented.
         */
        const const_Iterator operator--(difference_type) noexcept
        {
            const_Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        /**
         * A method that gets 2 iterators and returns the difference in indices.
         * @param other The iterator to subtract.
         * @return The number representing the value difference.
         */
        difference_type operator-(const const_Iterator &other) const noexcept
        {
            return _indexIt - other._indexIt;
        }

        /**
         * A method that checks if this iterator is smaller then another iterator.
         * @param other The iterator to compare to.
         * @return True if the index of this iterator is smaller, false otherwise.
         */
        bool operator<(const const_Iterator &other) const noexcept
        {
            return (other - (*this) > 0);
        }

        /**
         * A method that checks if this iterator is bigger then another iterator.
         * @param other The iterator to compare to.
         * @return True if the index of this iterator is bigger, false otherwise.
         */
        bool operator>(const const_Iterator &other) const noexcept
        {
            return (other < (*this));
        }

        /**
         * A method that checks if this iterator is smaller or equal then another iterator.
         * @param other The iterator to compare to.
         * @return True if the index of this iterator is smaller or equal, false otherwise.
         */
        bool operator<=(const const_Iterator &other) const noexcept
        {
            return !(*this > other);
        }

        /**
         * A method that checks if this iterator is bigger or equal then another iterator.
         * @param other The iterator to compare to.
         * @return True if the index of this iterator is bigger or equal, false otherwise.
         */
        bool operator>=(const const_Iterator &other) const noexcept
        {
            return !((*this < other));
        }

        /**
         * A method that increases a value from the iterator.
         * @param num The number to decrease.
         * @return The reference to the iterator.
         */
        const_Iterator &operator+=(const difference_type num) noexcept
        {
            _indexIt += num;
            return *this;
        }

        /**
         * A method that add a number to an iterator.
         * @param num The number to add to the iterator index.
         * @param it The iterator to add to.
         * @return An iterator to the index + value.
         */
        friend const_Iterator operator+(const int num, const const_Iterator &it) noexcept
        {
            return it + num;
        }

        /**
         * A method that adds a number to the iterator.
         * @param num The number to add.
         * @return An iterator pointing to the index + num;
         */
        const_Iterator operator+(const difference_type num) const noexcept
        {
            const_Iterator tmp = *this;
            return tmp += num;
        }

        /**
         * A method that decreases a value from the iterator.
         * @param num The number to decrease.
         * @return The reference to the iterator.
         */
        const_Iterator &operator-=(const difference_type num) noexcept
        {
            _indexIt -= num;
            return *this;
        }

        /**
         * A method that implements the - operator.
         * @param num The number of steps to move.
         * @return An iterator to the index of the current index - num.
         */
        const_Iterator operator-(const difference_type num) const noexcept
        {
            const_Iterator tmp = *this;
            return tmp -= num;
        }

        /**
         * A method that implements the [] operator.
         * @param num The given index to access.
         * @return The reference to the value that the iterator points to.
         */
        reference operator[](const difference_type num) const noexcept
        {
            return *(_pointer + _indexIt + num);
        }

    private:
        pointer _pointer;
        int _sizeIt{};
        difference_type _indexIt{};
    };

    /**
     * A method that returns an iterator to the first element of the list.
     * @return An iterator to the first element of the list.
     */
    const_Iterator cbegin() const
    {
        return const_Iterator(data(), _size, 0);
    }

    /**
     * A method that returns an iterator to the last element of the list.
     * @return An iterator to the last element of the list.
     */
    const_Iterator cend() const
    {
        return const_Iterator(data(), _size, _size);
    }


    /**
     * A class representing the const Random access iterator to the VLV class.
     */
    class Iterator
    {
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef int difference_type;
        typedef std::random_access_iterator_tag iterator_category;

    public:

        /**
         * The default C'tor.
         */
        Iterator(pointer p, int size, int index) : _pointer(p), _sizeIt(size), _indexIt(index){}

        /**
         * The copy C'tor.
         * @param other The iterator to copy.
         */
        Iterator(const Iterator &other)
        {
            _indexIt = other._indexIt;
            _sizeIt = other._sizeIt;
            _pointer = other._pointer;
        }

        /**
         * The default D'tor.
         */
        ~Iterator() = default;

        /**
         * A method that implements the equal operator.
         * @param other The iterator to copy.
         * @return The iterator we copied to.
         */
        Iterator &operator=(const Iterator &other)
        {
            if (*this != other)
            {
                _indexIt = other._indexIt;
                _sizeIt = other._sizeIt;
                _pointer = other._pointer;
            }
            return *this;
        }

        /**
         * A method that implements the dereference operator.
         * @return A reference to the pointed value.
         */
        reference operator*() const noexcept
        {
            return (*(_pointer + _indexIt));
        }

        /**
         * A method that implements the operator ->.
         * @return A pointer to the value.
         */
        pointer operator->() const noexcept
        {
            return _pointer + _indexIt;
        }

        /**
         * A method that implements the operator ++ from the right.
         * @return An iterator to the next object.
         */
        Iterator &operator++() noexcept
        {
            if (_indexIt < _sizeIt)
            {
                _indexIt++;
                return *this;
            }
            return *this;
        }

        /**
         * A method that implements the operator ++ from the left.
         * @return An iterator to the object before we increased.
         */
        const Iterator operator++(difference_type) noexcept
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * A method that implements the operator ==.
         * @param other The given iterator to check.
         * @return True if they are the same, false otherwise.
         */
        bool operator==(const Iterator &other) const noexcept
        {
            return (_pointer + _indexIt == other._pointer + other._indexIt);
        }

        /**
         * A method that implements the operator !=.
         * @param other The given iterator to check.
         * @return False if they are the same, true otherwise.
         */
        bool operator!=(const Iterator &other) const noexcept
        {
            return !(*this == other);
        }

        /**
         * A method that implements the operator -- from the right.
         * @return An iterator to the object before.
         */
        Iterator &operator--() noexcept
        {
            if (_indexIt == 0)
            {
                return *this;
            }
            _indexIt--;
            return *this;
        }

        /**
          * A method that implements the operator -- from the left.
          * @return An iterator to the object before we decremented.
          */
        const Iterator operator--(difference_type) noexcept
        {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        /**
         * A method that gets 2 iterators and returns the difference in indices.
         * @param other The iterator to subtract.
         * @return The number representing the value difference.
         */
        difference_type operator-(const Iterator &other) const noexcept
        {
            return _indexIt - other._indexIt;
        }

        /**
         * A method that checks if this iterator is smaller then another iterator.
         * @param other The iterator to compare to.
         * @return True if the index of this iterator is smaller, false otherwise.
         */
        bool operator<(const Iterator &other) const noexcept
        {
            return (other - (*this) > 0);
        }

        /**
         * A method that checks if this iterator is bigger then another iterator.
         * @param other The iterator to compare to.
         * @return True if the index of this iterator is bigger, false otherwise.
         */
        bool operator>(const Iterator &other) const noexcept
        {
            return (other < (*this));
        }

        /**
         * A method that checks if this iterator is smaller or equal then another iterator.
         * @param other The iterator to compare to.
         * @return True if the index of this iterator is smaller or equal, false otherwise.
         */
        bool operator<=(const Iterator &other) const noexcept
        {
            return !((*this > other));
        }

        /**
         * A method that checks if this iterator is bigger or equal then another iterator.
         * @param other The iterator to compare to.
         * @return True if the index of this iterator is bigger or equal, false otherwise.
         */
        bool operator>=(const Iterator &other) const noexcept
        {
            return !((*this < other));
        }

        /**
         * A method that increases a value from the iterator.
         * @param num The number to decrease.
         * @return The reference to the iterator.
         */
        Iterator &operator+=(const difference_type num) noexcept
        {
            _indexIt += num;
            return *this;
        }

        /**
         * A method that add a number to an iterator.
         * @param num The number to add to the iterator index.
         * @param it The iterator to add to.
         * @return An iterator to the index + value.
         */
        friend Iterator operator+(const int num, const Iterator &it) noexcept
        {
            return it + num;
        }

        /**
         * A method that adds a number to the iterator.
         * @param num The number to add.
         * @return An iterator pointing to the index + num;
         */
        Iterator operator+(const difference_type num) const noexcept
        {
            Iterator tmp = *this;
            return tmp += num;
        }

        /**
         * A method that decreases a value from the iterator.
         * @param num The number to decrease.
         * @return The reference to the iterator.
         */
        Iterator &operator-=(const difference_type num) noexcept
        {
            _indexIt -= num;
            return *this;
        }

        /**
         * A method that implements the - operator.
         * @param num The number of steps to move.
         * @return An iterator to the index of the current index - num.
         */
        Iterator operator-(const difference_type num) const noexcept
        {
            Iterator tmp = *this;
            return tmp -= num;
        }

        /**
         * A method that implements the [] operator.
         * @param num The given index to access.
         * @return The reference to the value that the iterator points to.
         */
        reference operator[](const difference_type num) const noexcept
        {
            return (*(_pointer + _indexIt + num));
        }

    private:
        pointer _pointer;
        int _sizeIt;
        difference_type _indexIt;
    };

    /**
     * A method that returns an iterator to the first element of the list.
     * @return An iterator to the first element of the list.
     */
    Iterator begin()
    {
        return Iterator(data(), _size, 0);
    }

    /**
     * A method that returns an iterator to the last element of the list.
     *  @return An iterator to the last element of the list.
     */
    Iterator end()
    {
        return Iterator(data(), _size, _size);
    }

    /**
     * A method that erases the value in the current iterator position.
     * @param it The given iterator that points to the object.
     * @return The iterator to the next value from the right.
     */
    Iterator erase(const Iterator &it)
    {
        Iterator tmp = it;
        Iterator tmp1 = it;
        tmp++;
        int diff = it - begin();
        while (tmp != end())
        {
            *tmp1 = *tmp;
            tmp1++;
            tmp++;
        }
        _size--;
        if (_size == StaticCapacity)
        {
            _resize(TO_STATIC);
        }
        Iterator new_It = begin();
        while (new_It != end())
        {
            if (diff == new_It - begin())
            {
                break;
            }
            new_It++;
        }
        return new_It;
    }

    /**
     * A method that erases the value in the current iterator position.
     * @param it The given const_iterator that points to the object.
     * @return The iterator to the next value from the right.
     */
    const_Iterator erase(const const_Iterator &it)
    {
        const_Iterator tmp = it;
        tmp++;
        int diff = it - cbegin();
        Iterator tmp1 = begin() + diff;
        while (tmp != cend())
        {
            *tmp1 = *tmp;
            tmp1++;
            tmp++;
        }
        _size--;
        if (_size == StaticCapacity)
        {
            _resize(TO_STATIC);
        }
        const_Iterator new_It = cbegin();
        while (new_It != cend())
        {
            if (diff == new_It - cbegin())
            {
                break;
            }
            new_It++;
        }
        return new_It;
    }

    /**
     * A method that inserts a value to the given position in the vector.
     * @param it The iterator to the place we wish to insert.
     * @param val The value to insert to the array.
     * @return An iterator to the added value.
     */
    Iterator insert(const Iterator &it, const T &val)
    {
        int diff = it - begin();
        if (_size == StaticCapacity)
        {
            _resize(TO_DYNAMIC);
        }
        else if (_size == _dynamic_cap && _size > StaticCapacity)
        {
            _resize(DYNAMIC_INC);
        }
        _size++;
        T current = *(data() + diff);
        *(data() + diff) = std::move(val);
        for (int i = diff + 1; i < _size; i++)
        {
            T tmp = *(data() + i);
            *(data() + i) = current;
            current = tmp;
        }
        return Iterator(data(), _size, diff);
    }

    /**
     * A method that inserts a value to the given position in the vector.
     * @param it The const_iterator to the place we wish to insert.
     * @param val The value to insert to the array.
     * @return An iterator to the added value.
     */
    const_Iterator insert(const const_Iterator &it, const T &val)
    {
        int diff = it - cbegin();
        if (_size == StaticCapacity)
        {
            _resize(TO_DYNAMIC);
        }
        else if (_size == _dynamic_cap && _size > StaticCapacity)
        {
            _resize(DYNAMIC_INC);
        }
        _size++;
        T current = *(data() + diff);
        *(data() + diff) = std::move(val);
        for (int i = diff + 1; i < _size; i++)
        {
            T tmp = *(data() + i);
            *(data() + i) = current;
            current = tmp;
        }
        return const_Iterator(data(), _size, diff);
    }

    /**
     * A method that inserts a R-value to the given position in the vector.
     * @param it The iterator to the place we wish to insert.
     * @param val The R-value to insert to the array.
     * @return An iterator to the added value.
     */
    Iterator insert(const Iterator &it, T && val)
    {
        int diff = it - begin();
        if (_size == StaticCapacity)
        {
            _resize(TO_DYNAMIC);
        }
        else if (_size == _dynamic_cap && _size > StaticCapacity)
        {
            _resize(DYNAMIC_INC);
        }
        _size++;
        T current = *(data() + diff);
        *(data() + diff) = std::move(val);
        for (int i = diff + 1; i < _size; i++)
        {
            T tmp = *(data() + i);
            *(data() + i) = current;
            current = tmp;
        }
        return Iterator(data(), _size, diff);
    }

    /**
     * A method that inserts a R-value to the given position in the vector.
     * @param it The iterator to the place we wish to insert.
     * @param val The R-value to insert to the array.
     * @return An iterator to the added value.
     */
    const_Iterator insert(const const_Iterator &it, T && val)
    {
        int diff = it - cbegin();
        if (_size == StaticCapacity)
        {
            _resize(TO_DYNAMIC);
        }
        else if (_size == _dynamic_cap && _size > StaticCapacity)
        {
            _resize(DYNAMIC_INC);
        }
        _size++;
        T current = *(data() + diff);
        *(data() + diff) = std::move(val);
        for (int i = diff + 1; i < _size; i++)
        {
            T tmp = *(data() + i);
            *(data() + i) = current;
            current = tmp;
        }
        return const_Iterator(data(), _size, diff);
    }

private:

    /**
    * A method that resizes and changes the vector type.
    * @param num The given operation representation.
    */
    void _resize(int num)
    {
        if (num == TO_STATIC)
        {
            for (int i = 0; i < _size; ++i)
            {
                _static_vec[i] = _dynamic_vec[i];
            }
            delete[] _dynamic_vec;
            _dynamic_vec = nullptr;
        }
        else if (num == TO_DYNAMIC)
        {
            _dynamic_vec = new T[DYNAMIC_CAP];
            for (int i = 0; i < _size; ++i)
            {
                _dynamic_vec[i] = _static_vec[i];
            }
            _dynamic_cap = DYNAMIC_CAP;
        }
        else
        {
            T *tmp = new T[DYNAMIC_CAP];
            _dynamic_cap = DYNAMIC_CAP;
            for (int i = 0; i < _size; i++)
            {
                tmp[i] = *(_dynamic_vec + i);
            }
            delete[] _dynamic_vec;
            _dynamic_vec = tmp;
        }
    }

    int _size{};
    int _dynamic_cap{};
    T _static_vec[StaticCapacity]{};
    T *_dynamic_vec{};
};

#endif //VLVECTOR_VLVECTOR_HPP
