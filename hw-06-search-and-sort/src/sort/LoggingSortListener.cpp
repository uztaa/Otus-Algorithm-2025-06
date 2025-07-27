#include "LoggingSortListener.h"

void LoggingSortListener::onEvent(const SortEvent& event) {
    if (const auto* e = dynamic_cast<const StartEvent*>(&event)) {
        std::cout << "[Start] Sorter: " << e->getSorterName() << std::endl;
    }
    else if (const auto* e = dynamic_cast<const CompareEvent*>(&event)) {
        std::cout << "[Compare] Indices: " << e->index1 << " vs " << e->index2
            << " in sorter: " << e->getSorterName() << std::endl;
    }
    else if (const auto* e = dynamic_cast<const SwapEvent*>(&event)) {
        std::cout << "[Swap] Indices: " << e->index1 << " <-> " << e->index2
            << " in sorter: " << e->getSorterName() << std::endl;
    }
    else if (const auto* e = dynamic_cast<const SetEvent*>(&event)) {
        std::cout << "[Set] Index: " << e->index
                  << " Value: (" << e->value.getKey() << ", " << e->value.getValue() << ")"
            << " in sorter: " << e->getSorterName() << std::endl;
    }
    else if (const auto* e = dynamic_cast<const FinishEvent*>(&event)) {
		std::cout << "[Finish] Sorter: " << e->getSorterName()
         << std::endl;
    }
}