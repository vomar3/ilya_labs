#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>
#include <initializer_list>

class vector final {
private:
    double *arr;
    int capacity, size;

    void resize(int newcapacity) {
        double *resize;
        resize = new double[newcapacity];
        auto count = std::min(size, newcapacity);
        std::copy(arr, arr + count, resize);
        delete[] arr;
        arr = resize;
        capacity = newcapacity;
        size = count;
    }

public:
    class iterator final {

    private:
        double *ptr;

    public:
        iterator(double *ptr) : ptr{ptr} {}

        double const &operator *() const {
            return *ptr;
        }

        double &operator *() {
            return *ptr;
        }

        iterator &operator ++() {
            ptr++;
            return *this;
        }

        iterator operator ++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator &operator --() {
            ptr--;
            return *this;
        }

        iterator operator --(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        iterator operator +(int shift) const {
            return iterator(ptr + shift);
        }

        iterator &operator +=(int shift) {
            ptr += shift;
            return *this;
        }

        iterator operator -(int shift) const {
            return iterator(ptr - shift);
        }

        iterator &operator -=(int shift) {
            ptr -= shift;
            return *this;
        }

        double *operator ->() {
            return ptr;
        }

        double const *operator ->() const {
            return ptr;
        }

        double &operator [](int shift) {
            return *(ptr + shift);
        }

        double operator [](int shift) const {
            return *(ptr + shift);
        }

        int operator -(const iterator &iter) const {
            return ptr - iter.ptr;
        }

        bool operator ==(const iterator &iter) const {
            return ptr == iter.ptr;
        };

        bool operator !=(const iterator &iter) const {
            return ptr != iter.ptr;
        };

        bool operator >(const iterator &iter) const {
            return ptr > iter.ptr;
        };

        bool operator >=(const iterator &iter) const {
            return ptr >= iter.ptr;
        };

        bool operator <(const iterator &iter) const {
            return ptr < iter.ptr;
        };

        bool operator <=(const iterator &iter) const {
            return ptr <= iter.ptr;
        };
    };

    vector(int count_elements, double default_value) : arr{new double[count_elements]}, size(count_elements), capacity(count_elements) {
        std::fill_n(arr, count_elements, default_value);
    }

    vector(int count_elements) : vector(count_elements, 0.0) {}

    template <typename Iterator>

    vector(Iterator const begin, Iterator const end) {
        auto distance = end - begin;
        arr = new double[distance];
        capacity = size = distance;
        std::copy(begin, end, arr);
    }

    vector(std::initializer_list<double> init_list) : capacity{static_cast<int>(init_list.size())}, size(init_list.size()), arr{new double[init_list.size()]} {
        std::copy(init_list.begin(), init_list.end(), arr);
    }

    vector(const vector &v) : vector(v.begin(), v.end()) {};

    double &at(int index) {
        if (index < 0 || index >= size)
            throw std::range_error("Out of range");

        return arr[index];
    }

    double const &at(int index) const {
        if (index < 0 || index >= size)
            throw std::range_error("Out of range");

        return arr[index];
    }

    double &front() {
        if (size == 0)
            throw std::range_error("Out of range");

        return arr[0];
    }

    double const &front() const {
        if (size == 0)
            throw std::range_error("Out of range");

        return arr[0];
    }

    double &back() {
        if (size == 0)
            throw std::range_error("Out of range");

        return arr[size - 1];
    }

    double const &back() const {
        if (size == 0)
            throw std::range_error("Out of range");

        return arr[size - 1];
    }

    double *data() {
        return arr;
    }

    double const *data() const {
        return arr;
    }

    bool empty() const {
        return !size;
    }

    void reserve(int num) {
        if (num <= capacity)
            return;

        resize(num);
    }

    int func_capacity() const {
        return capacity;
    }

    int func_size() const {
        return size;
    }

    void shrink_to_fit() {
        if (capacity <= size)
            return;

        resize(size);
    }

    void clear() {
        size = 0;
    }

    void push_back(double elem) {
        if (capacity == size)
        {
            resize(capacity * 2);
        }
        arr[size++] = elem;
    }

    void pop_back() {
        if (size > 0)
            --size;
    }

    void resize(int size, double elem) {
        int oldsize = size;
        resize(size);
        if (size > oldsize) {
            std::fill(arr + oldsize, arr + size, elem);
        }
    }

    void erase(int index) {
        int i;

        if (index >= size)
            return;

        for (i = index; i < size - 1; ++i) {
            arr[i] = arr[i + 1];
        }

        --size;
    }

    void insert(int index, double elem) {
        int i;

        if (index > size)
            throw std::range_error("Out of range");

        if (size == capacity)
            resize(capacity * 2);

        ++size;

        for (i = size - 1; i > index; --i) {
            arr[i] = arr[i - 1];
        }

        arr[index] = elem;
    }

    bool operator ==(vector const &v) const {
        int i;

        if (size != v.size)
            return false;

        for (i = 0; i < size; ++i) {
            if (arr[i] != v.arr[i])
                return false;
        }

        return true;
    }

    int operator <=>(vector const &v) const {
        int i;

        for (i = 0; i < size && i < v.size; ++i) {
            if (arr[i] - v.arr[i] < -__DBL_EPSILON__)
                return -1;
            if (arr[i] - v.arr[i] > __DBL_EPSILON__)
                return 1;
        }

        if (size > v.size)
            return 1;

        if (size < v.size)
            return -1;

        return 0;
    }

    vector &operator =(vector const &v) {
        if (this != &v) {
            vector tmp(v.begin(), v.end());
            std::swap(tmp.arr, this->arr);
            std::swap(tmp.capacity, this->capacity);
            std::swap(tmp.size, this->size);
        }

        return *this;
    }

    iterator begin() {
        return iterator(arr);
    }

    iterator end() {
        return iterator(arr + func_size());
    }

    const iterator begin() const {
        return iterator(arr);
    }

    const iterator end() const {
        return iterator(arr + func_size());
    }

    ~vector() {
        delete[] arr;
    }
};

int main()
{
    std::vector<double> v1(5, 10);
    v1.insert(v1.cbegin() + 5, 15);
    vector a{1, 2, 3, 4, 5};
    a.insert(2, 10);
    vector b(10, 3.0);

    for (auto elem : a) {
        std::cout << elem << std::endl;
    }
    const vector edds{9., 2., 3., 4., 5.};

    if (a == b) {
        std::cout << "Vector a = b" << std::endl;
    } else {
        std::cout << "Vector a != b" << std::endl;
    }

    a.push_back(52);
    for (auto elem : a) {
        std::cout << elem << std::endl;
    }

    a.pop_back();
    for (auto elem : a) {
        std::cout << elem << std::endl;
    }

    std::cout << a.func_size() << std::endl;
    std::cout << a.func_capacity() << std::endl;
    std::cout << a.end() - a.begin() << std::endl;

    const double &check = edds.at(3);
    std::cout << "check " << check << std::endl;

    double &check_a = a.at(4);
    std::cout << check_a << std::endl;

    std::cout << a.empty() << std::endl;

    vector::iterator it = a.begin();
    std::cout << "Start: " << *it << std::endl;

    vector::iterator tmp = it++;
    std::cout << "it++: " << *it << std::endl;
    std::cout << "tmp: " << *tmp << std::endl;
    tmp += 1;
    std::cout << "tmp: " << *tmp << std::endl;

    a = b;
    for (auto elem : a) {
        std::cout << elem << std::endl;
    }
    return 0;
}