#include "BaseSorter.h"
#include "SortListener.h"
#include "SortEvent.h"

/** 
 * @file BaseSorter.cpp
 * @brief base Sortable inplementation
 */
void BaseSorter::addListener(std::shared_ptr<SortListener> listener) {
    if (listener != nullptr) {
        listeners.push_back(listener);
    }
}

void BaseSorter::notify(const SortEvent& event) {
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
