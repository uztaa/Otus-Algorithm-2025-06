#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

#include "AppState.h"
#include "sort/GuiSortListener.cpp"
#include "SorterFactory.h"
#include "RandomArrayGenerator.h"

#include "SortEventFormatter.h"
#include "sort/LoggingSortListener.cpp"

// Настройки окна и интерфейса (UI Settings)
constexpr int WindowWidth = 2500;
constexpr int WindowHeight = 1500;
constexpr float GlobalScale = 2.0f;
constexpr int ControlSize = 500;


// Цвета для визуализации
const sf::Color DefaultColor = sf::Color::White;
const sf::Color CompareColor = sf::Color::Yellow;
const sf::Color SwapColor = sf::Color::Red;
const sf::Color SetColor = sf::Color::Green;

auto loggingSortListener = std::make_shared<LoggingSortListener>();


// Список доступных алгоритмов сортировки
std::vector<std::string> sortAlgorithmNames = SorterFactory::getAvailableSorters();

// Индекс текущего выбранного алгоритма сортировки
size_t currentSorterIndex = 0;

// Состояние приложения
std::unique_ptr<AppState> appState = std::make_unique<AppState>(); 

// Таймеры и задержки
constexpr int StepDelay = 400; // Задержка между шагами сортировки в миллисекундах
// Таймер для управления шагами сортировки
sf::Clock stepClock;
const sf::Time stepInterval = sf::milliseconds(StepDelay); // задержка мс между шагами

// Функция для отладки и вывода цветных записей в консоль
void printColoredRecordsForDebug(const std::vector<ColoredRecord>& records) {
    std::cout << "[";
    for (size_t i = 0; i < records.size(); ++i) {
        const auto& record = records[i];
        const sf::Color& color = record.getColor();
        std::cout << record.getKey();
        if (i + 1 != records.size()) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}

// Функция для генерации данных и запуска сортировки
// TODO: разбить по файлам: DataGenerator.h/cpp + тесты к нему
extern std::unique_ptr<AppState> generateData(const std::string& sorterName, size_t size) {
    // 1. Сгенерировать исходные данные
    RandomArrayGenerator generator;
    std::vector<Record> originalData = generator.generate(size);

    // 2. Клонировать массив для сортировки и визуализации
    std::vector<Record> dataToSort = originalData;
    std::vector<ColoredRecord> coloredData;
    coloredData.reserve(originalData.size());

    for (const Record& record : originalData) {
        coloredData.emplace_back(record.getKey(), record.getValue());
    }

    // 3. Создать listener
    auto listener = std::make_shared<GuiSortListener>();

    // 4. Создать сортировщик по имени
    auto sorter = SorterFactory::createSorter(sorterName);
    if (!sorter) {
        throw std::runtime_error("Unknown sorter: " + sorterName);
    }

	// 5. подписываем listener на события сортировки
	sorter->addListener(listener);

    // 6. Запустить сортировку
    sorter->sort(dataToSort);

    // 7. Получить события
   // const auto& events = listener->getSortEvents();
	auto events = listener->getSortEvents(); // должны быть скопированы, т.к. GuiSortListener хранит их по значению

    // 8. Создать AppState и вернуть
    return std::make_unique<AppState>(std::move(coloredData), std::move(events));
}

// Функция для обновления состояния приложения
void update(AppState& state) {
    // Получаем следующее событие
    if (!state.hasNextEvent()) {
        return;
    }

	// Получаем ссылку на массив ColoredRecord
	auto& coloredRecords = state.getColoredRecords();

	// Получаем следующее событие из состояния
    const SortEvent& event = state.getNextEvent();

	// Устанавливаем текущее событие в состояние приложения
    //appState->setCurrentEventInfo(SortEventFormatter::format(event));

    loggingSortListener->onEvent(event); // FIXME: Логируем событие

    if (event.getType() == "compare") {
        const CompareEvent& e = static_cast<const CompareEvent&>(event);

        coloredRecords[e.index1].setColor(CompareColor);
        coloredRecords[e.index2].setColor(CompareColor);
    }
    else if (event.getType() == "swap") {
        const SwapEvent& e = static_cast<const SwapEvent&>(event);
        
        std::swap(coloredRecords[e.index1], coloredRecords[e.index2]);
        coloredRecords[e.index1].setColor(SwapColor);
        coloredRecords[e.index2].setColor(SwapColor);
    }
    else if (event.getType() == "set") {
        const SetEvent& e = static_cast<const SetEvent&>(event);
        coloredRecords[e.index] = ColoredRecord(e.value.getKey(), e.value.getValue(), SetColor);
    }
    else if (event.getType() == "start" || event.getType() == "finish") {
        // Ничего не делаем — можно добавить лог/эффекты при желании
    }
    else {
        assert(false && "Unknown SortEvent type");
		std::cerr << "Unknown SortEvent type: " << event.getType() << "\n";
    }

	printColoredRecordsForDebug(coloredRecords); //FIXME: Отладочный вывод
}

// Функция для отрисовки графика сортировки
void render(sf::RenderWindow& window, sf::Vector2f panelPos, sf::Vector2f panelSize, AppState& state) {
    // обновляем только по таймеру
	// TODO: сделать более гибким, чтобы можно было настраивать шаги
    while (stepClock.getElapsedTime() < stepInterval);
    stepClock.restart();

	//window.clear(sf::Color::Black); //FIXME: очищаем всё окно перед отрисовкой
    
	// ---- закрашиваем старые бары, если еще не все прорисовались ----
    if (appState->hasNextEvent()) {
        sf::RectangleShape background;
        background.setPosition(panelPos);
        background.setSize(panelSize);
        background.setFillColor(sf::Color::Black);
        window.draw(background);
    }

    // ---- рисуем бары ----
    const auto& records = state.getColoredRecords();
    size_t count = records.size();

    if (count == 0) return;

    float barWidth = panelSize.x / count;
    float maxHeight = panelSize.y;

    for (size_t i = 0; i < count; ++i) {
        float value = static_cast<float>(records[i].getKey());
        float barHeight = (value / 100.f) * maxHeight;

        sf::RectangleShape bar;
        bar.setSize({ (barWidth - 2.f) * 0.9f, -barHeight }); // -barHeight, чтобы рисовать вверх от низа
        bar.setPosition({
            panelPos.x + i * barWidth,
            panelPos.y + panelSize.y
            });
        bar.setFillColor(records[i].getColor());

        window.draw(bar);
    }
}

// Функция для отрисовки панели управления
void renderControlPanel() {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(ControlSize, WindowHeight - 20), ImGuiCond_Always);

    ImGui::Begin("Control");

    // combo box для выбора алгоритма сортировки
    if (ImGui::BeginCombo("Algorithm", sortAlgorithmNames[currentSorterIndex].c_str())) {
        for (int i = 0; i < sortAlgorithmNames.size(); ++i) {
            bool isSelected = (currentSorterIndex == i);
            if (ImGui::Selectable(sortAlgorithmNames[i].c_str(), isSelected)) {
                currentSorterIndex = i;
                std::cout << "[ComboBox] algorithm checked: " << sortAlgorithmNames[i] << "\n";
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // Кнопки управления сортировкой
    // Кнопка "Start" для запуска сортировки
    if (ImGui::Button("Start")) {  
        std::cout << "[Button] pushed: Start\n";
        appState->setMode(AppMode::Sorting);

        try {
            // Генерируем данные и запускаем сортировку
            appState = generateData(sortAlgorithmNames[currentSorterIndex], 10); // Пример размера массива
            appState->setMode(AppMode::Sorting);

            std::cout << "Data generated and sorting started with " << sortAlgorithmNames[currentSorterIndex] << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            appState->setMode(AppMode::Stopped);
        }
    }

	/* TODO: Реализовать паузу
    ImGui::SameLine();
    // Кнопка "Pause" для приостановки сортировки
    if (ImGui::Button("Pause")) {
        appState->setMode(AppMode::Paused);
        std::cout << "[Button] pushed: Pause\n";
    }
    */

    ImGui::SameLine();
    // Кнопка "Resume" для возобновления сортировки
    if (ImGui::Button("Stop")) {
        appState->setMode(AppMode::Stopped);
        std::cout << "[Button] pushed: Stop\n";

        // Очищаем состояние приложения
		appState.reset(); // Удаляем текущее состояние приложения
        appState = std::make_unique<AppState>();
        std::cout << "AppState cleared\n";
    }

    //ImGui::Text("Status: %s", appState->getModeAsString().c_str()); // Отображение текущего состояния приложения

    ImGui::End();
};

// Функция для отрисовки панели визуализации
void renderVisualizationPanel(sf::RenderWindow& window) {

	auto panelPos = sf::Vector2f(ControlSize + 20, 10);
	auto panelSize = sf::Vector2f(WindowWidth - ControlSize - 30, WindowHeight - 20);

    ImGui::SetNextWindowPos(panelPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(panelSize, ImGuiCond_Always);

    ImGui::Begin("Visualization");

	// Проверяем, есть ли данные для визуализации
    if (!appState || appState->getMode() == AppMode::Stopped) {
        ImGui::Text("No data to visualize. Please start a sorting algorithm.");
        ImGui::End();
        return;
	}

	// Обновляем состояние приложения
	update(*appState);

	// Отрисовываем график сортировки
	render(window, panelPos, panelSize, *appState);

	// TODO: Добавить информацию о текущем состоянии сортировки
	// взятую из AppState информацию о текущем состоянии
	ImGui::Text("Visualization in progress..."); // FIXME: Отображение статуса визуализации appState->getCurrentEventInfo().c_str()

    ImGui::End();
};

// Основная функция обработки событий и отрисовки интерфейса
void process(sf::RenderWindow& window) {
    renderControlPanel();
    renderVisualizationPanel(window);
}

// Основная функция приложения
int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u( WindowWidth, WindowHeight )), "Sorting Visualizer");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        std::cerr << "ImGui-SFML initialization failed!" << std::endl;
        return 1;
    }
    if (!ImGui::GetCurrentContext()) {
        std::cerr << "ImGui context not created!" << std::endl;
        return 1;
    }

    ImGui::GetStyle().ScaleAllSizes(GlobalScale);
    ImGui::GetIO().FontGlobalScale = GlobalScale;

    sf::Clock deltaClock;
   
    while (window.isOpen()) {
        sf::Time dt = deltaClock.restart();

        while (auto opt = window.pollEvent()) {
            const sf::Event& event = *opt;
            ImGui::SFML::ProcessEvent(window, event);

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, dt);

		process(window);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}