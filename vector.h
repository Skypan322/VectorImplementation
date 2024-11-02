#ifndef CPP2_S21_CONTAINERS_1_SRC_S21_CONTAINERS_VECTOR_S21_VECTOR_H
#define CPP2_S21_CONTAINERS_1_SRC_S21_CONTAINERS_VECTOR_S21_VECTOR_H
#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>

namespace s21 {
template <typename T>
class vector {
   public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using iterator = T *;
    using const_iterator = const T *;
    using size_type = size_t;

    //*Vector Member functions*
    // Default constructor
    vector() : vector(0) {}
    // parameterized constructor, creates the vector of size n

    vector(size_type n) : size_(n), capacity_(n) {
        array = allocate_memory(capacity_);
        std::fill_n(array, n, value_type());
    }
    // initializer list constructor, creates vector
    // initizialized using std::initializer_list<T>
    vector(std::initializer_list<value_type> const &items)
        : vector(items.size()) {
        int count = 0;
        for (auto &element : items) {
            array[count] = element;
            ++count;
        }
    }
    // copy constructor
    vector(const vector &v) : vector(v.size_) {
        for (size_t i = 0; i != v.size(); ++i) {
            array[i] = v[i];
        }
    }
    // move constructor
    vector(vector &&v)
        : array(v.array), size_(v.size_), capacity_(v.capacity_) {
        v.array = nullptr;
        v.size_ = 0;
        v.capacity_ = 0;
    }
    // assignment operator overload for moving object
    vector &operator=(vector &&v) {
        vector news(std::move(v));
        swap(news);
        return *this;
    }

    //*Vector Element access*
    vector &operator=(const vector &v) {
        vector news(std::move(v));
        swap(news);
        return *this;
    }
    ~vector() { delete[] array; }
    reference at(size_type pos) {
        if (pos >= size_) throw std::out_of_range("Index out of range");
        return &array[pos];
    }
    reference operator[](size_type pos) {
        if (pos >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return array[pos];
    }
    T *data() { return this->array; }
    const T *data() const { return this->array; }

    const_reference front() const {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return array[0];
    }
    const_reference back() const {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return array[size_ - 1];
    }
    //*Vector Iterators*
    iterator begin() { return iterator(array); }
    const_iterator begin() const { return const_iterator(array); }
    iterator end() { return iterator(array + size_ - 1); }
    const_iterator end() const { return const_iterator(array + size_ - 1); }
    //*Vector Capacity*
    bool empty() const { return (size_ == 0) ? true : false; }
    size_type size() const { return size_; }
    size_type max_size() const {
        return std::numeric_limits<value_type>::max() / sizeof(value_type);
    }
    void reserve(size_t n) {
        if (n <= capacity_) return;
        capacity_ = n;
        // iterator newarr =
        //     reinterpret_cast<iterator>(new int8_t[n * sizeof(value_type)]);
        iterator newarr = new value_type[capacity_];
        try {
            std::uninitialized_copy(array, array + size_, newarr);
        } catch (...) {
            delete[] newarr;
            // delete[] reinterpret_cast<int8_t *>(newarr);
            throw;
        }
        for (size_t i = 0; i < size_; ++i) {
            (array + i)->~T();
        }
        delete[] this->array;
        this->array = newarr;
    }
    size_type capacity() const { return capacity_; }
    void shrink_to_fit() {
        if (capacity_ > size_) {
            value_type *newarr = new value_type[size_];
            try {
                std::uninitialized_copy(array, array + size_, newarr);
            } catch (...) {
                delete[] newarr;
                throw;
            }
            delete[] array;
            array = newarr;
            capacity_ = size_;
        }
    }

    //*Vector Modifiers*
    void clear() { size_ = 0; }
    iterator insert(iterator pos, const_reference value) {
        size_t n = pos - begin();
        n == 0 ? n = 0 : n++;
        if (size_ >= capacity_) {
            reserve(capacity_ * 2);
            pos =
                begin() +
                n;  // после reserve() итератор pos может стать недействительным
        }
        // Копируем элементы справа от позиции в новый буфер
        value_type *newarr = new value_type[size_ - n];
        try {
            std::uninitialized_copy(array + n, array + size_, newarr);
        } catch (...) {
            delete[] newarr;
            throw;
        }
        // Устанавливаем новое значение в позицию
        array[n] = value;
        // Копируем элементы обратно в основной буфер
        try {
            std::uninitialized_copy(newarr, newarr + size_ - n, array + n + 1);
        } catch (...) {
            for (size_t i = 0; i < n + 1; ++i) {
                (array + i)->~T();
            }
            delete[] newarr;
            throw;
        }
        // Удаляем временный буфер
        delete[] newarr;
        ++size_;
        return begin() + n;
    }

    void erase(iterator pos) {
        // Находим позицию элемента, который нужно удалить
        size_t n = pos - begin();
        // Initialize new array
        iterator newarr = reinterpret_cast<iterator>(
            new int8_t[(size_ - 1) * sizeof(value_type)]);
        try {
            if (n > 0) {
                std::uninitialized_copy(array, array + n, newarr);
                std::uninitialized_copy(array + n + 1, array + size_,
                                        newarr + n);
            } else {
                std::uninitialized_copy(array + 1, array + size_, newarr);
            }
        } catch (...) {
            delete[] newarr;
            throw;
        }
        delete[] array;
        array = newarr;
        capacity_ = size_ - 1;
        size_ = size_ - 1;
    }
    void push_back(const_reference value) {
        if (size_ == capacity_) {
            capacity_ = capacity_ > 0 ? capacity_ * 2 : 1;
            value_type *new_arr = new value_type[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                new_arr[i] = array[i];
            }
            delete[] array;
            array = new_arr;
        }
        array[size_++] = value;
    }
    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("Vector is empty");
        } else {
            --size_;
        }
    }
    void swap(vector &other) {
        std::swap(array, other.array);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
    value_type *allocate_memory(size_t capacity) {
        return new value_type[capacity];
    }
    void resize(size_t n, const T &value = T()) {
        if (n > capacity_) reserve(n);
        for (size_t i = size_; i < n; ++i) {
            new (array + i) value_type(value);
        }
        if (n < size_) {
            size_ = n;
        }
    }
    const_reference operator[](size_t i) const { return array[i]; }

    template <class... Args>
    iterator insert_many(const_iterator pos, Args &&...args) {
        for (const auto &arg : {args...}) {
            pos = insert(iterator(pos), arg);
        }
        return iterator(pos);
    }

    template <class... Args>
    void insert_many_back(Args &&...args) {
        for (const auto &arg : {args...}) {
            push_back(arg);
        }
    }
    // void print_vector(const vector &v){
    //     for(size_t i = 0; i < v.size(); ++i){
    //         std::cout << "v[" << i << "] = " << v[i] << std::endl;
    //     }
    //     std::cout << std::endl;
    // }
   private:
    value_type *array;
    size_t size_;
    size_t capacity_;
};
}  // namespace s21
#endif  // CPP2_S21_CONTAINERS_1_SRC_S21_CONTAINERS_VECTOR_S21_VECTOR_H
