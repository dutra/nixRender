#ifndef OPTIONAL_VECTOR_H
#define OPTIONAL_VECTOR_H

#include <vector>
#include <functional>

#include "types.h"

#define NULL_ENTRY 0

template <typename T>
struct OptionalEntry {
    std::size_t id;
    T data;
};

template <class T>
class OptionalVector {
public:
    OptionalVector();
    ~OptionalVector();
    std::size_t add(T new_element, std::function<void(T&, std::size_t id)> func);
    std::size_t insert(T new_element, std::size_t index);
    T& operator[](uint32 index);

private:
    std::vector<OptionalEntry<T>> _entries;

};

template<class T>
OptionalVector<T>::OptionalVector() {
    OptionalEntry<T> null_entry;
    null_entry.id = NULL_ENTRY;
    _entries.push_back(null_entry);
}

template<class T>
OptionalVector<T>::~OptionalVector() {

}

template<class T>
std::size_t OptionalVector<T>::insert(T new_element, std::size_t index) {
    OptionalEntry<T> new_entry;
    new_entry.id = index;
    new_entry.data = new_element;

    if (index > _entries.size())
        return NULL_ENTRY;

    if (index == _entries.size()) {
        _entries.push_back(new_entry);
        return new_entry.id;
    }

    if (index == 0)
        return NULL_ENTRY;

    OptionalEntry<T>& current_entry = _entries[index];
    if (current_entry.id != 0)
        return NULL_ENTRY;

    current_entry = new_entry;
    return index;

}

template<class T>
std::size_t OptionalVector<T>::add(T new_element, std::function<void(T&, std::size_t id)> func) {
    if (_entries.size() == 1) {
        func(new_element, 1);
        OptionalEntry<T> new_entry;
        new_entry.id = 1;
        new_entry.data = new_element;
        _entries.push_back(new_entry);
        return 1;
    }

    for (auto it = _entries.begin() + 1; it != _entries.end(); ++it) {
        if (it->id == 0) {
            std::size_t index = it - _entries.begin();
            func(new_element, index);
            it->id = index;
            it->data = new_element;

            return index;
        }
    }
    return NULL_ENTRY;
}

template<class T>
T& OptionalVector<T>::operator[](uint32 index) {
    if (index >= _entries.size())
        return nullptr;
    OptionalEntry<T> entry = _entries[index];
    if (entry.id == 0){
        return nullptr;
    } else {
        return entry.data;
    }
}

#endif
