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
    std::vector<const SortEvent*> m_events;  // просто указатели, владения нет

public:
    void onEvent(const SortEvent& event) override {
        m_events.push_back(&event); // просто сохраняем адрес
    }

    const std::vector<const SortEvent*>& getSortEvents() const {
        return m_events;
    }
};

