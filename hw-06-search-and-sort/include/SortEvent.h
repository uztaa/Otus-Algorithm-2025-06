#pragma once
#include <string>
#include "Record.h"

/** 
 * @file SortEvent.h
 * @brief ќпределение базового класса дл€ событий сортировки.
 * 
 * Ётот файл содержит определение базового класса SortEvent и его производных классов,
 * которые представл€ют различные событи€, возникающие в процессе сортировки.
 */
class SortEvent {
private:
    std::string sorterName;  // »м€ сортировщика, который инициирует событие
public:
    virtual ~SortEvent() = default;

    SortEvent(const std::string& name = "unknown") : sorterName(name) {}

    //  онструкторы копировани€ и перемещени€ должны быть доступны
    SortEvent(const SortEvent&) = default;
    SortEvent(SortEvent&&) noexcept = default;

    SortEvent& operator=(const SortEvent&) = default;
    SortEvent& operator=(SortEvent&&) noexcept = default;

    /** @brief ѕолучает тип событи€ сортировки.
     * 
	 * @return —трока, представл€юща€ тип событи€.
     */
    virtual std::string getType() const = 0;

    /** @brief ѕолучает им€ сортировщика, инициирующего событие.
     * 
     * Ётот метод должен быть реализован в производных классах, чтобы вернуть им€
     * сортировщика, который инициировал событие.
     * 
     * @return —трока с именем сортировщика.
	 */
    std::string getSorterName() const { return sorterName; }
};

/** 
 * @brief Ѕазовый класс дл€ событий сортировки.
 * 
 * Ётот класс представл€ет базовое событие сортировки и содержит метод дл€ получени€ типа событи€.
 */
class StartEvent : public SortEvent {
public:
	StartEvent(std::string name = "unknown") : SortEvent(name) {}
    std::string getType() const override { return "start"; }
};

/** @brief  ласс дл€ событий завершени€ сортировки.
 * 
 * Ётот класс представл€ет событие, возникающее при завершении сортировки.
 */
class FinishEvent : public SortEvent {
public:
	FinishEvent(std::string name = "unknown") : SortEvent(name) {}
    std::string getType() const override { return "finish"; }
};

/** @brief  ласс дл€ событий сравнени€ элементов в процессе сортировки.
 * 
 * Ётот класс представл€ет событие, возникающее при сравнении двух элементов массива.
 */
class CompareEvent : public SortEvent {
public:
    size_t index1, index2;
	CompareEvent(std::string name, size_t i1, size_t i2) : SortEvent(name), index1(i1), index2(i2) {}
    std::string getType() const override { return "compare"; }
};

/** @brief  ласс дл€ событий обмена элементов в процессе сортировки.
 * 
 * Ётот класс представл€ет событие, возникающее при обмене двух элементов массива.
 */
class SwapEvent : public SortEvent {
public:
    size_t index1, index2;
	SwapEvent(std::string name, size_t i1, size_t i2) : SortEvent(name), index1(i1), index2(i2) {}
    std::string getType() const override { return "swap"; }
};

/** @brief  ласс дл€ событий установки значени€ в массиве.
 * 
 * Ётот класс представл€ет событие, возникающее при установке значени€ в определенный индекс массива.
 */
class SetEvent : public SortEvent {
public:
    size_t index;
    Record value;
	SetEvent(std::string name, size_t i, const Record& v) : SortEvent(name), index(i), value(v) {}
    std::string getType() const override { return "set"; }
};
