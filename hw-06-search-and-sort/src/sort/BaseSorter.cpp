#include "BaseSorter.h"
#include "SortListener.h"
#include "SortEvent.h"

/** 
 * @file BaseSorter.cpp
 * @brief Реализация базового класса BaseSorter для сортировщиков.
 */
void BaseSorter::addListener(std::shared_ptr<SortListener> listener) {
    listeners.push_back(listener);
}

void BaseSorter::notify(const SortEvent& event) {
	// Уведомляем всех слушателей о событии
    for (const auto& listener : listeners) {
        listener->onEvent(event);
    }
}

void BaseSorter::setName(const std::string& n) {
    name = n;
}

std::string BaseSorter::getName() const {
    return name;
}
