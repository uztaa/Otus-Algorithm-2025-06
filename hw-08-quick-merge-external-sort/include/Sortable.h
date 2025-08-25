
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Record.h"
#include "SortListener.h"

class SortEvent;

/**
 * @file Sortable.h
 * @brief interface for sort data.
 */
class Sortable {
public:
    virtual ~Sortable() = default;

    /**
     * @brief sort data
     * @param arr data
     */
    virtual void sort(std::vector<Record>& arr) = 0;

    /**
     * @brief add listener
     * @param listener any SortListener.
     */
    virtual void addListener(std::shared_ptr<SortListener> listener) = 0;

    /**
    * @brief notify all listeners
    * @param event SortEvent
    */
    virtual void notify(const SortEvent& event) = 0;

    /**
     * @brief sorter name setter
     * @param name sorter name
     */
    virtual void setName(const std::string& name) = 0;

    /**
    * @brief sorter name getter
    * @return sorter name
    */
    virtual std::string getName() const = 0;
};
