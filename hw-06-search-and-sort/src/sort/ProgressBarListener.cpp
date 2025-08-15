#include "SortEvent.h"
#include "ProgressBarListener.h"
#include <iostream>

ProgressBarListener::ProgressBarListener(size_t totalSteps, int barWidth)
    : totalSteps(totalSteps), barWidth(barWidth), stopFlag(false) {
    workerThread = std::thread(&ProgressBarListener::eventLoop, this);
}

ProgressBarListener::~ProgressBarListener() {
    stop();
}

void ProgressBarListener::onEvent(const SortEvent& event) {
    EventData data;
    data.sorterName = event.getSorterName();
    data.type = event.getType();

    std::lock_guard<std::mutex> lock(queueMutex);
    eventQueue.push(std::move(data));
    
    cv.notify_one();
}

void ProgressBarListener::stop() {
    std::lock_guard<std::mutex> lock(queueMutex);
    stopFlag = true;
    
    cv.notify_one();
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void ProgressBarListener::eventLoop() {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [this]() {
            return !eventQueue.empty() || stopFlag;
        });

        if (stopFlag && eventQueue.empty())
            break;

        while (!eventQueue.empty()) {
            const EventData& data = eventQueue.front();

            if (data.type == "start") {
                progressMap[data.sorterName] = 0;
                lastPrinted[data.sorterName] = -1;
            } else if (data.type != "finish") {
                ++progressMap[data.sorterName];
            } else if (data.type == "finish") {
                progressMap[data.sorterName] = totalSteps;
            }

            eventQueue.pop();
        }

        lock.unlock();
        printProgress();
    }
}

void ProgressBarListener::printProgress() {
    std::lock_guard<std::mutex> lock(outputMutex);
    for (const auto& [name, current] : progressMap) {
        int filled = static_cast<int>((double)current / totalSteps * barWidth);
        if (filled != lastPrinted[name]) {
            lastPrinted[name] = filled;
            std::cout << name << ": [";
            for (int i = 0; i < barWidth; ++i) {
                std::cout << (i < filled ? "#" : " ");
            }
            std::cout << "] " << (100 * current / totalSteps) << "%\n";
        }
    }
    std::cout << std::flush;
}
