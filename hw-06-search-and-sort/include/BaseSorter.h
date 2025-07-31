#pragma once

#include "Sortable.h"
#include <vector>
#include <memory>

class SortEvent;

/** 
 * @brief Базовый класс для сортировщиков, реализующий интерфейс Sortable.
 * 
 * Этот класс предоставляет общую функциональность для всех сортировщиков,
 * включая управление слушателями событий сортировки и имя сортировщика.
 */
class BaseSorter : public Sortable {
protected:
	// Список слушателей событий сортировки
    std::vector<std::shared_ptr<SortListener>> listeners;
    // Имя сортировщика
    std::string name;

public:
	BaseSorter() = default;
	BaseSorter(const std::string& n) : name(n) {}
    void addListener(std::shared_ptr<SortListener> listener) override;
    void notify(const SortEvent& event) override;
    void setName(const std::string& n) override;
    std::string getName() const override;
};
