#pragma once

#include "Sortable.h"
#include <vector>
#include <memory>

class SortEvent;

/** 
 * @brief abstract base class for Sortable interface.
 */
class BaseSorter : public Sortable {
protected:
	// listners
    std::vector<std::shared_ptr<SortListener>> listeners;
    // name of sorter
    std::string name;

public:
	BaseSorter() = default;
	BaseSorter(const std::string& n) : name(n) {}
    void addListener(std::shared_ptr<SortListener> listener) override;
    void notify(const SortEvent& event) override;
    void setName(const std::string& n) override;
    std::string getName() const override;
};
