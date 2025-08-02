#pragma once

#include <string>
#include <memory>

#include "Record.h"

/** 
 * @file SortEvent.h
 * @brief Определение базового класса для событий сортировки.
 * 
 * Этот файл содержит определение базового класса SortEvent и его производных классов,
 * которые представляют различные события, возникающие в процессе сортировки.
 */
class SortEvent {
private:
    std::string sorterName;  // Имя сортировщика, который инициирует событие
public:
public:
    SortEvent() = default;
    SortEvent(const std::string& name = "unknown") : sorterName(name) {}

    // Конструкторы копирования и перемещения должны быть доступны
    SortEvent(const SortEvent&) = default;
    SortEvent(SortEvent&&) noexcept = default;

    SortEvent& operator=(const SortEvent&) = default;
    SortEvent& operator=(SortEvent&&) noexcept = default;

    virtual ~SortEvent() = default;

    /** @brief Получает тип события сортировки.
     * 
	 * @return Строка, представляющая тип события.
     */
    virtual std::string getType() const = 0;

    /** @brief Получает имя сортировщика, инициирующего событие.
     * 
     * Этот метод должен быть реализован в производных классах, чтобы вернуть имя
     * сортировщика, который инициировал событие.
     * 
     * @return Строка с именем сортировщика.
	 */
    std::string getSorterName() const { return sorterName; }

    /** 
     * @brief Создает клон текущего события сортировки.
     * 
     * Этот метод должен быть реализован в производных классах, чтобы создать
     * копию текущего события сортировки.
     * 
     * @return Указатель на новый объект SortEvent, представляющий клон текущего события.
	 */
    virtual std::shared_ptr<SortEvent> clone() const = 0;
};

/** 
 * @brief Базовый класс для событий сортировки.
 * 
 * Этот класс представляет базовое событие сортировки и содержит метод для получения типа события.
 */
class StartEvent : public SortEvent {
public:
	StartEvent(std::string name = "unknown") : SortEvent(name) {}
    std::string getType() const override { return "start"; }

    std::shared_ptr<SortEvent> clone() const override {
        return std::make_shared<StartEvent>(*this);
    }
     
};

/** @brief Класс для событий завершения сортировки.
 * 
 * Этот класс представляет событие, возникающее при завершении сортировки.
 */
class FinishEvent : public SortEvent {
public:
	FinishEvent(std::string name = "unknown") : SortEvent(name) {}
    std::string getType() const override { return "finish"; }

    std::shared_ptr<SortEvent> clone() const override {
        return std::make_shared<FinishEvent>(*this);
    }
};

/** @brief Класс для событий сравнения элементов в процессе сортировки.
 * 
 * Этот класс представляет событие, возникающее при сравнении двух элементов массива.
 */
class CompareEvent : public SortEvent {
public:
    size_t index1, index2;
	CompareEvent(std::string name, size_t i1, size_t i2) : SortEvent(name), index1(i1), index2(i2) {}
    std::string getType() const override { return "compare"; }

    std::shared_ptr<SortEvent> clone() const override {
        return std::make_shared<CompareEvent>(*this);
	}
};

/** @brief Класс для событий обмена элементов в процессе сортировки.
 * 
 * Этот класс представляет событие, возникающее при обмене двух элементов массива.
 */
class SwapEvent : public SortEvent {
public:
    size_t index1, index2;
	SwapEvent(std::string name, size_t i1, size_t i2) : SortEvent(name), index1(i1), index2(i2) {}
    std::string getType() const override { return "swap"; }

    std::shared_ptr<SortEvent> clone() const override {
        return std::make_shared<SwapEvent>(*this);
    }
};

/** @brief Класс для событий установки значения в массиве.
 * 
 * Этот класс представляет событие, возникающее при установке значения в определенный индекс массива.
 */
class SetEvent : public SortEvent {
public:
    size_t index;
    Record value;
	SetEvent(std::string name, size_t i, const Record& v) : SortEvent(name), index(i), value(v) {}
    std::string getType() const override { return "set"; }

    std::shared_ptr<SortEvent> clone() const override {
        return std::make_shared<SetEvent>(*this);
	}
};
