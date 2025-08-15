#pragma once

#include "Array.h"
#include <stdexcept>

// реализация односвязного списка
template<typename T>
class LinkedList : public Array<T> {
private:
    // структура ноды
    struct Node {
        T data;
        Node* next;

        Node(T value, Node* next = nullptr) : data(value), next(next) {}
    };

    Node* head;
    int count;

public:
    LinkedList() : head(nullptr), count(0) {}

    ~LinkedList() override {
        while (head != nullptr) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }



    void add(T item, int index) override {
        Array<T>::check_index_for_insert(index, count);

        if (index == 0) {
            head = new Node(item, head);
        }
        else {
            Node* prev = getNode(index - 1);
            prev->next = new Node(item, prev->next);
        }

        ++count;
    }

    T remove(int index) override {
        Array<T>::check_index(index, count);

        Node* toRemove;

        if (index == 0) {
            toRemove = head;
            head = head->next;
        }
        else {
            Node* prev = getNode(index - 1);
            toRemove = prev->next;
            prev->next = toRemove->next;
        }

        T value = toRemove->data;
        delete toRemove;
        --count;
        return value;
    }

    T get(int index) const override {
        Array<T>::check_index(index, count);

        return getNode(index)->data;
    }

    int size() const override {
        return count;
    }

    // Конструктор копирования
    LinkedList(const LinkedList& other) : head(nullptr), count(0) {
        Node* current = other.head;
        while (current) {
            add(current->data, count);
            current = current->next;
        }
    }

    // Оператор присваивания
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            while (head) {
                Node* tmp = head;
                head = head->next;
                delete tmp;
            }
            count = 0;
            Node* current = other.head;
            while (current) {
                add(current->data, count);
                current = current->next;
            }
        }
        return *this;
    }

private:
    Node* getNode(int index) const {
        Node* current = head;
        for (int i = 0; i < index; ++i)
            current = current->next;
        return current;
    }
};
