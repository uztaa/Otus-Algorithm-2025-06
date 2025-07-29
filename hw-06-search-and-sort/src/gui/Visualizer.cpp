#include "Visualizer.h"

Visualizer::Visualizer(const std::vector<SortEvent>& events)
    : m_events(events)
{
}

void Visualizer::start() {
    if (m_events.empty()) {
        // Нет событий — ничего не делаем
        return;
    }
    if (m_currentIndex >= m_events.size()) {
        // Если воспроизведение дошло до конца, сбрасываемся
        m_currentIndex = 0;
    }
    m_isRunning = true;
    m_isPaused = false;
    m_accumulator = std::chrono::milliseconds{ 0 };
}

void Visualizer::pause() {
    if (m_isRunning) {
        m_isPaused = true;
    }
}

void Visualizer::stop() {
    m_isRunning = false;
    m_isPaused = false;
    m_currentIndex = 0;
    m_accumulator = std::chrono::milliseconds{ 0 };
}

void Visualizer::setPauseTime(std::chrono::milliseconds pauseTime) {
    m_pauseTime = pauseTime;
}

void Visualizer::update(std::chrono::milliseconds deltaTime) {
    if (!m_isRunning || m_isPaused || m_events.empty()) {
        return;
    }

    m_accumulator += deltaTime;

    // Проходим события с учётом накопленного времени
    while (m_accumulator >= m_pauseTime && m_currentIndex < m_events.size()) {
        m_accumulator -= m_pauseTime;
        ++m_currentIndex;

        if (m_currentIndex >= m_events.size()) {
            // Достигли конца воспроизведения
            m_isRunning = false;
            m_isPaused = false;
            m_currentIndex = m_events.size(); // Можно оставить для отрисовки финального состояния
            break;
        }
    }
}

size_t Visualizer::getCurrentIndex() const {
    return m_currentIndex;
}

bool Visualizer::isRunning() const {
    return m_isRunning;
}

bool Visualizer::isPaused() const {
    return m_isPaused;
}
