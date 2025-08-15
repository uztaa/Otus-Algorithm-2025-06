
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Record.h"
#include "SortListener.h"

class SortEvent;

/**
 * @file Sortable.h
 * @brief Интерфейс для сортировщиков.
 *
 * Этот интерфейс определяет методы для сортировки массива записей,
 * добавления слушателей событий сортировки и уведомления слушателей о событиях.
 */
class Sortable {
public:
    virtual ~Sortable() = default;

    /**
     * @brief Метод для сортировки массива записей.
     * @param arr Массив записей, который нужно отсортировать.
     * @return Пара, содержащая количество сравнений и сдвигов.
     */
    virtual std::pair<size_t, size_t> sort(std::vector<Record>& arr) = 0;

    /**
     * @brief Метод для добавления слушателя событий сортировки.
     * @param listener Умный указатель на объект, реализующий интерфейс SortListener.
     */
    virtual void addListener(std::shared_ptr<SortListener> listener) = 0;

    /**
    * @briefМетод для уведомления слушателей о событии сортировки
    * @param event Событие сортировки, которое нужно уведомить слушателям.
    */
    virtual void notify(const SortEvent& event) = 0;

    /**
     * @brief Метод для установки имени сортировщика.
     * @param name Имя сортировщика.
     */
    virtual void setName(const std::string& name) = 0;

    /**
    * @brief Метод для получения имени сортировщика.
    * @return Имя сортировщика.
    */
    virtual std::string getName() const = 0;
};
