#pragma once

#include <string>
#include <memory>

#include "Record.h"

/** 
 * @file SortEvent.h
 * @brief abstract sort event
 */
class SortEvent {
private:
    std::string sorterName;
public:
public:
    SortEvent() = default;
    SortEvent(const std::string& name = "unknown") : sorterName(name) {}

    SortEvent(const SortEvent&) = default;
    SortEvent(SortEvent&&) noexcept = default;

    SortEvent& operator=(const SortEvent&) = default;
    SortEvent& operator=(SortEvent&&) noexcept = default;

    virtual ~SortEvent() = default;

    /** @brief sort type
     * 
	 * @return name of sorter type
     */
    virtual std::string getType() const = 0;

    /** @brief sorter name getter
     * 
     * @return sorter name
	 */
    std::string getSorterName() const { return sorterName; }

    /** 
     * @brief clone event
     * 
     * @return cloned SortEvent
	 */
    virtual std::shared_ptr<SortEvent> clone() const = 0;
};

/** 
 * @brief start event
 */
class StartEvent : public SortEvent {
public:
	StartEvent(std::string name = "unknown") : SortEvent(name) {}
    std::string getType() const override { return "start"; }

    std::shared_ptr<SortEvent> clone() const override {
        return std::make_shared<StartEvent>(*this);
    }
     
};

/** 
 * @brief finish event
 */
class FinishEvent : public SortEvent {
public:
	FinishEvent(std::string name = "unknown") : SortEvent(name) {}
    std::string getType() const override { return "finish"; }

    std::shared_ptr<SortEvent> clone() const override {
        return std::make_shared<FinishEvent>(*this);
    }
};

/** 
 * @brief compare event
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

/** 
 * @brief swap event
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

/** 
 * @brief set event
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
