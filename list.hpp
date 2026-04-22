#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>

namespace sjtu {

template<typename T>
class list {
protected:
    struct node {
        T *data;
        node *prev;
        node *next;
        node() : data(nullptr), prev(nullptr), next(nullptr) {}
        node(const T &val) : data(new T(val)), prev(nullptr), next(nullptr) {}
        ~node() { delete data; }
    };

    node *head;
    size_t m_size;

    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        ++m_size;
        return cur;
    }

    node *erase(node *pos) {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        --m_size;
        return pos;
    }

public:
    class const_iterator;

    class iterator {
    private:
        node *ptr;
        const list *container;
    public:
        friend class list;
        friend class const_iterator;

        iterator() : ptr(nullptr), container(nullptr) {}
        iterator(node *p, const list *c) : ptr(p), container(c) {}

        iterator operator++(int) {
            iterator tmp = *this;
            if (ptr == container->head) throw invalid_iterator();
            ptr = ptr->next;
            return tmp;
        }

        iterator &operator++() {
            if (ptr == container->head) throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            if (ptr->prev == container->head) throw invalid_iterator();
            ptr = ptr->prev;
            return tmp;
        }

        iterator &operator--() {
            if (ptr->prev == container->head) throw invalid_iterator();
            ptr = ptr->prev;
            return *this;
        }

        T &operator*() const {
            if (ptr == container->head) throw invalid_iterator();
            return *(ptr->data);
        }

        T *operator->() const {
            if (ptr == container->head) throw invalid_iterator();
            return ptr->data;
        }

        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };

    class const_iterator {
    private:
        const node *ptr;
        const list *container;
    public:
        friend class list;
        friend class iterator;

        const_iterator() : ptr(nullptr), container(nullptr) {}
        const_iterator(const node *p, const list *c) : ptr(p), container(c) {}
        const_iterator(const iterator &it) : ptr(it.ptr), container(it.container) {}

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            if (ptr == container->head) throw invalid_iterator();
            ptr = ptr->next;
            return tmp;
        }

        const_iterator &operator++() {
            if (ptr == container->head) throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            if (ptr->prev == container->head) throw invalid_iterator();
            ptr = ptr->prev;
            return tmp;
        }

        const_iterator &operator--() {
            if (ptr->prev == container->head) throw invalid_iterator();
            ptr = ptr->prev;
            return *this;
        }

        const T &operator*() const {
            if (ptr == container->head) throw invalid_iterator();
            return *(ptr->data);
        }

        const T *operator->() const {
            if (ptr == container->head) throw invalid_iterator();
            return ptr->data;
        }

        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };

    list() : head(new node()), m_size(0) {
        head->prev = head;
        head->next = head;
    }

    list(const list &other) : head(new node()), m_size(0) {
        head->prev = head;
        head->next = head;
        for (node *cur = other.head->next; cur != other.head; cur = cur->next) {
            push_back(*(cur->data));
        }
    }

    virtual ~list() {
        clear();
        delete head;
    }

    list &operator=(const list &other) {
        if (this == &other) return *this;
        clear();
        for (node *cur = other.head->next; cur != other.head; cur = cur->next) {
            push_back(*(cur->data));
        }
        return *this;
    }

    const T &front() const {
        if (m_size == 0) throw container_is_empty();
        return *(head->next->data);
    }

    const T &back() const {
        if (m_size == 0) throw container_is_empty();
        return *(head->prev->data);
    }

    iterator begin() {
        return iterator(head->next, this);
    }

    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }

    iterator end() {
        return iterator(head, this);
    }

    const_iterator cend() const {
        return const_iterator(head, this);
    }

    virtual bool empty() const {
        return m_size == 0;
    }

    virtual size_t size() const {
        return m_size;
    }

    virtual void clear() {
        while (m_size > 0) {
            pop_front();
        }
    }

    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this) throw invalid_iterator();
        node *n = new node(value);
        insert(pos.ptr, n);
        return iterator(n, this);
    }

    virtual iterator erase(iterator pos) {
        if (pos.container != this) throw invalid_iterator();
        if (pos.ptr == head) throw invalid_iterator();
        if (m_size == 0) throw container_is_empty();
        node *n = erase(pos.ptr);
        node *next_node = n->next;
        delete n;
        return iterator(next_node, this);
    }

    void push_back(const T &value) {
        node *n = new node(value);
        n->prev = head->prev;
        n->next = head;
        head->prev->next = n;
        head->prev = n;
        ++m_size;
    }

    void pop_back() {
        if (m_size == 0) throw container_is_empty();
        node *n = head->prev;
        n->prev->next = head;
        head->prev = n->prev;
        --m_size;
        delete n;
    }

    void push_front(const T &value) {
        node *n = new node(value);
        n->next = head->next;
        n->prev = head;
        head->next->prev = n;
        head->next = n;
        ++m_size;
    }

    void pop_front() {
        if (m_size == 0) throw container_is_empty();
        node *n = head->next;
        head->next = n->next;
        n->next->prev = head;
        --m_size;
        delete n;
    }

    void sort() {
        if (m_size <= 1) return;
        T *arr = static_cast<T*>(::operator new(m_size * sizeof(T)));
        node *cur = head->next;
        for (size_t i = 0; i < m_size; ++i) {
            ::new (arr + i) T(*(cur->data));
            cur = cur->next;
        }
        sjtu::sort<T>(arr, arr + m_size, [](const T &a, const T &b) -> bool {
            return a < b;
        });
        cur = head->next;
        for (size_t i = 0; i < m_size; ++i) {
            *(cur->data) = arr[i];
            arr[i].~T();
            cur = cur->next;
        }
        ::operator delete(arr);
    }

    void merge(list &other) {
        if (this == &other) return;
        if (other.m_size == 0) return;
        if (m_size == 0) {
            head->next = other.head->next;
            head->prev = other.head->prev;
            head->next->prev = head;
            head->prev->next = head;
            m_size = other.m_size;
            other.head->next = other.head;
            other.head->prev = other.head;
            other.m_size = 0;
            return;
        }
        node *a = head->next;
        node *b = other.head->next;
        node *tail = head;
        while (a != head && b != other.head) {
            if (*(a->data) < *(b->data)) {
                a->prev = tail;
                tail->next = a;
                tail = a;
                a = a->next;
            } else {
                b->prev = tail;
                tail->next = b;
                tail = b;
                b = b->next;
            }
        }
        if (a == head) {
            tail->next = b;
            b->prev = tail;
            head->prev = other.head->prev;
            other.head->prev->next = head;
        } else {
            tail->next = a;
            a->prev = tail;
            head->prev = tail;
        }
        m_size += other.m_size;
        other.head->next = other.head;
        other.head->prev = other.head;
        other.m_size = 0;
    }

    void reverse() {
        if (m_size <= 1) return;
        node *cur = head;
        do {
            node *tmp = cur->next;
            cur->next = cur->prev;
            cur->prev = tmp;
            cur = tmp;
        } while (cur != head);
    }

    void unique() {
        if (m_size <= 1) return;
        node *cur = head->next;
        while (cur != head && cur->next != head) {
            if (*(cur->data) == *(cur->next->data)) {
                node *to_del = cur->next;
                to_del->next->prev = cur;
                cur->next = to_del->next;
                --m_size;
                delete to_del;
            } else {
                cur = cur->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
