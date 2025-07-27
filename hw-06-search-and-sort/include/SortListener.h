
#pragma once
#include <string>
#include <memory>

class SortEvent;

/** 
 * @brief Интерфейс для слушателей событий сортировки.
 * 
 * Этот интерфейс позволяет объектам подписываться на события сортировки,
 * такие как начало, завершение, сравнение и обмен элементов.
 */
class SortListener {
public:
    virtual ~SortListener() = default;
	
    /**
     * @brief Метод, который будет вызван при возникновении события сортировки.
     * @param sorterName Имя сортировщика, который вызвал событие.
     * @param event Событие сортировки.
	 */
    virtual void onEvent(const SortEvent& event) = 0;
};
