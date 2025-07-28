
#pragma once

#include <string>
#include <queue>
#include <map>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "SortListener.h"

struct EventData {
    std::string sorterName;
    std::string type;
};

class ProgressBarListener : public SortListener {
public:
    ProgressBarListener(size_t totalSteps, int barWidth = 50);
    ~ProgressBarListener();

    void onEvent(const SortEvent& event) override;
    void stop();

private:
    void eventLoop();
    void printProgress();

    std::queue<EventData> eventQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::thread workerThread;
    bool stopFlag = false;

    std::map<std::string, size_t> progressMap;
    std::map<std::string, int> lastPrinted;
    std::mutex outputMutex;

    size_t totalSteps;
    int barWidth;
};
