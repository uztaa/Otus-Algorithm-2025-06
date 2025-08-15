#pragma once

#include "Array.h"
#include <stdexcept>

// Реализация двусвязного списка
template<typename T>
class DoublyLinkedList : public Array<T> {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(T value, Node* prev = nullptr, Node* next = nullptr)
            : data(value), prev(prev), next(next) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~DoublyLinkedList() override {
        Node* current = head;
        while (current != nullptr) {
            Node* tmp = current;
            current = current->next;
            delete tmp;
        }
    }

    void add(T item, int index) override {
        Array<T>::check_index_for_insert(index, count);

        if (index == 0) {
            Node* newNode = new Node(item, nullptr, head);
            if (head) head->prev = newNode;
            head = newNode;
            if (count == 0) tail = newNode;
        } else if (index == count) {
            Node* newNode = new Node(item, tail, nullptr);
            if (tail) tail->next = newNode;
            tail = newNode;
            if (count == 0) head = newNode;
        } else {
            Node* nextNode = getNode(index);
            Node* prevNode = nextNode->prev;
            Node* newNode = new Node(item, prevNode, nextNode);
            prevNode->next = newNode;
            nextNode->prev = newNode;
        }
        ++count;
    }

    T remove(int index) override {
        Array<T>::check_index(index, count);

        Node* toRemove = getNode(index);
        T value = toRemove->data;

        if (toRemove->prev) toRemove->prev->next = toRemove->next;
        else head = toRemove->next;

        if (toRemove->next) toRemove->next->prev = toRemove->prev;
        else tail = toRemove->prev;

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

private:
    Node* getNode(int index) const {
        Node* current;
        if (index < count / 2) {
            current = head;
            for (int i = 0; i < index; ++i)
                current = current->next;
        } else {
            current = tail;
            for (int i = count - 1; i > index; --i)
                current = current->prev;
        }
        return current;
    }
};