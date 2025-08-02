#include <vector>
#include "SortListener.h"
#include "SortEvent.h"

/** 
 * @file GuiSortListener.cpp
 * @brief Реализация класса GuiSortListener для обработки событий сортировки в графическом интерфейсе.
 * 
 * Этот класс реализует интерфейс SortListener и хранит события сортировки в векторе.
 */
class GuiSortListener : public SortListener {
private:
    std::vector<std::shared_ptr<SortEvent>> m_events;

public:
    void onEvent(const SortEvent& event) override {
        m_events.push_back(event.clone());  // сохраняем копию
    }

    const std::vector<std::shared_ptr<SortEvent>>& getSortEvents() const {
        return m_events;
    }
};

