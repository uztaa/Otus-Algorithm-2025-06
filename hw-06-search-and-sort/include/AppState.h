#pragma once

#include "SortEvent.h"
#include "ColoredRecord.h"
#include "AppMode.h"
#include <vector>

/** @file AppState.h
 * @brief Содержит описание состояния приложения, включая массив записей, события сортировки и текущее состояние.
 */
class AppState {
private:
    std::vector<ColoredRecord> coloredRecords;           // Массив записей для отображения и сортировки
    std::vector<SortEvent> events;             // Очередь событий визуализации
    std::size_t currentEventIndex = 0;         // Индекс текущего события визуализации
    AppMode mode = AppMode::Stopped;           // Текущее состояние приложения

public:
    /** @brief Получает текущий массив записей.
     * 
     * @return Ссылка на вектор ColoredRecord, представляющий текущий массив записей.
     */
    const std::vector<ColoredRecord>& getColoredRecords() const {
        return coloredRecords;
    }
    
    /** @brief Получает текущее состояние приложения.
     * 
     * @return Текущее состояние приложения (AppMode).
     */
    AppMode getMode() const {
        return mode;
    }

    /** @brief Получает текущее состояние приложения в виде строки.
     * 
     * @return Строковое представление текущего состояния приложения.
     */
    std::string getModeAsString() const {
        switch (mode) {
        case AppMode::Stopped: return "Stopped";
        case AppMode::Sorting: return "Sorting";
        case AppMode::Paused: return "Paused";
        default: return "Unknown";
        }
    }

    /** @brief Устанавливает новое состояние приложения.
     * 
     * @param newMode Новое состояние приложения (AppMode).
     */
    void setMode(AppMode newMode) {
        mode = newMode;
	}

	// Конструктор и деструктор
    AppState() = default;
    ~AppState() = default;
	/** 
    * @brief Конструктор для инициализации состояния приложения с массивом записей и событиями сортировки.
	* @param arr Массив записей, который будет использоваться в приложении для визуализации сортировки.
	* @param sortEvents Вектор событий сортировки, которые будут использоваться для визуализации.
	* @details Этот конструктор позволяет инициализировать состояние приложения с заданным массивом записей и событиями сортировки.
	* @note Конструктор перемещения и копирования удалены, чтобы предотвратить несанкционированное копирование или перемещение состояния приложения.
	* @warning Убедитесь, что массив записей и события сортировки корректно инициализированы перед использованием этого конструктора.
	* @see AppState(const AppState&) для запрета копирования.
	* @see AppState(AppState&&) для запрета перемещения.
	* @see AppState& operator=(const AppState&) для запрета копирования присваивания.
	* @see AppState& operator=(AppState&&) для запрета перемещения присваивания.
	*/
    AppState(std::vector<Record> records, std::vector<SortEvent> sortEvents)
		: events(std::move(sortEvents)), currentEventIndex(0) {
        // Инициализация массива ColoredRecord из массива Record
        coloredRecords.reserve(records.size());
        for (const auto& record : records) {
            coloredRecords.emplace_back(record.getKey(), record.getValue());
		}
	}

    // Запрет копирования и перемещения
    AppState(const AppState&) = delete;
    AppState(AppState&&) noexcept = delete;
    AppState& operator=(const AppState&) = delete;
	AppState& operator=(AppState&&) noexcept = delete;
   
    /** @brief Проверяет, есть ли еще события для визуализации.
     * 
     * @return true, если есть еще события; false в противном случае.
     */
    bool hasNextEvent() const {
        return currentEventIndex < events.size();
    }
    
    /** @brief Получает следующее событие для визуализации.
     * 
     * @return Ссылка на следующее событие.
     */
    const SortEvent& getNextEvent() {
        return events[currentEventIndex++];
	}
};
