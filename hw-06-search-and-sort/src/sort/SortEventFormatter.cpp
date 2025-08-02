#include "SortEventFormatter.h"
#include "SortEvent.h"

std::string SortEventFormatter::format(const SortEvent& event) {
    if (const auto* e = dynamic_cast<const StartEvent*>(&event)) {
        return "[Start] Sorter: " + e->getSorterName();
    }
    else if (const auto* e = dynamic_cast<const CompareEvent*>(&event)) {
        return "[Compare] Indices: " + std::to_string(e->index1) + " vs " +
            std::to_string(e->index2) + " in sorter: " + e->getSorterName();
    }
    else if (const auto* e = dynamic_cast<const SwapEvent*>(&event)) {
        return "[Swap] Indices: " + std::to_string(e->index1) + " <-> " +
            std::to_string(e->index2) + " in sorter: " + e->getSorterName();
    }
    else if (const auto* e = dynamic_cast<const SetEvent*>(&event)) {
        return "[Set] Index: " + std::to_string(e->index) +
            " Value: (" + std::to_string(e->value.getKey()) + ", " +
            e->value.getValue().c_str() + ")" +
            " in sorter: " + e->getSorterName();
    }
    else if (const auto* e = dynamic_cast<const FinishEvent*>(&event)) {
        return "[Finish] Sorter: " + e->getSorterName();
    }
    else {
        return "[Unknown Event]";
    }
}
