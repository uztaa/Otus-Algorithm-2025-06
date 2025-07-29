#pragma once

#include <vector>
#include <chrono>
#include <cstddef>
#include "SortEvent.h"

/**
 * @brief  ласс Visualizer отвечает за поэтапное воспроизведение событий сортировки.
 *
 * ѕринимает в конструкторе список событий, и умеет запускать,
 * приостанавливать и останавливать воспроизведение с контролем паузы между шагами.
 */
class Visualizer {
public:
    /**
     * @brief  онструктор принимает ссылку на вектор событий сортировки.
     * @param events ¬ектор событий сортировки.
     */
    explicit Visualizer(const std::vector<SortEvent>& events);

    /**
     * @brief «апуск воспроизведени€ с текущей позиции.
     * ≈сли воспроизведение завершено, начинаетс€ сначала.
     */
    void start();

    /**
     * @brief ѕауза воспроизведени€.
     */
    void pause();

    /**
     * @brief ќстановка и сброс воспроизведени€.
     */
    void stop();

    /**
     * @brief ”становка времени паузы между шагами воспроизведени€.
     * @param pauseTime ¬рем€ паузы в миллисекундах.
     */
    void setPauseTime(std::chrono::milliseconds pauseTime);

    /**
     * @brief ќбновление состо€ни€ визуализации.
     * @param deltaTime ¬рем€ прошедшее с последнего вызова.
     */
    void update(std::chrono::milliseconds deltaTime);

    /**
     * @brief ѕолучить индекс текущего событи€, используемый дл€ визуализации.
     * @return »ндекс текущего событи€.
     */
    size_t getCurrentIndex() const;

    /**
     * @brief ѕроверить, идет ли воспроизведение.
     * @return true, если воспроизведение активно.
     */
    bool isRunning() const;

    /**
     * @brief ѕроверить, находитс€ ли воспроизведение на паузе.
     * @return true, если воспроизведение на паузе.
     */
    bool isPaused() const;

private:
    const std::vector<SortEvent>& m_events;

    std::chrono::milliseconds m_pauseTime{ 500 };
    size_t m_currentIndex{ 0 };

    bool m_isRunning{ false };
    bool m_isPaused{ false };

    std::chrono::milliseconds m_accumulator{ 0 };
};
