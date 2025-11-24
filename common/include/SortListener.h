
#pragma once
#include <string>
#include <memory>

class SortEvent;

/** 
 * @brief interface for listen sort event
 */
class SortListener {
public:
    virtual ~SortListener() = default;
	
    /**
     * @brief do somthing when event come back
     * @param event incomming event
	 */
    virtual void onEvent(const SortEvent& event) = 0;
};
