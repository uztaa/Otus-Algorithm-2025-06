#pragma once

#include "SortListener.h"
#include "SortEvent.h"
#include <iostream>

class LoggingSortListener : public SortListener {
public:
    void onEvent(const SortEvent& event) override;
};
