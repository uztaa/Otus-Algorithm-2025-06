#pragma once

#include <string>
#include <stdexcept>

// јбстрактный интерфейс дл€ классов, считающих что-либо по числу n.
class Counter {
public:
    // n_str Ч строка, представл€юща€ целое число (ожидаетс€ uint64_t)
    virtual std::string compute(const std::string& n_str) const {
        throw "Counter::compute not implemented";
    }

    virtual ~Counter() = default;
};