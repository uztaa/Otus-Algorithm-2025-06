#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <iostream>
#include <vector>
#include <string>

#include "AppState.h"

// Настройки окна и интерфейса (UI Settings)
constexpr int WindowWidth = 2500;
constexpr int WindowHeight = 1500;
constexpr float GlobalScale = 2.0f;
constexpr int ControlSize = 500;


// Список доступных алгоритмов сортировки
std::vector<std::string> sorterNames = {
    "BubbleSort", "InsertionSort", "CocktailSort", "ShellSort"
};
// Индекс текущего выбранного алгоритма сортировки
size_t currentSorterIndex = 0;

AppState appState; // Состояние приложения

// Функция для отрисовки панели управления
void renderControlPanel() {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(ControlSize, WindowHeight - 20), ImGuiCond_Always);

    ImGui::Begin("Control");

    if (ImGui::BeginCombo("Algorithm", sorterNames[currentSorterIndex].c_str())) {
        for (int i = 0; i < sorterNames.size(); ++i) {
            bool isSelected = (currentSorterIndex == i);
            if (ImGui::Selectable(sorterNames[i].c_str(), isSelected)) {
                currentSorterIndex = i;
                std::cout << "[ComboBox] algorithm checked: " << sorterNames[i] << "\n";
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Start")) {
        appState.setMode(AppMode::Sorting);
        std::cout << "[Button] pushed: Start\n";
    }

    ImGui::SameLine();
    if (ImGui::Button("Pause")) {
		appState.setMode(AppMode::Paused);
        std::cout << "[Button] pushed: Pause\n";
    }

    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
		appState.setMode(AppMode::Stopped);
        std::cout << "[Button] pushed: Stop\n";
    }

	//std::cout << "[AppState] current mode: " << appState.getModeAsString() << "\n";
    // FIXME: не работает отображение текущего состояния приложения
	ImGui::Text("Status: %s", "test string"); // так работает, но не отображает текущее состояние приложения
	//ImGui::Text("Status: %s", appState.getModeAsString()); // так не рабоатет, но должно отображать текущее состояние приложения

    ImGui::End();
};

// Функция для отрисовки панели визуализации
void renderVisualizationPanel() {
    ImGui::SetNextWindowPos(ImVec2(ControlSize + 20, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(WindowWidth - ControlSize - 30, WindowHeight - 20), ImGuiCond_Always);

    ImGui::Begin("Visualization");

    ImGui::Text("Visualization in progress...");

    ImGui::End();
};

// Основная функция обработки событий и отрисовки интерфейса
void process() {
    renderControlPanel();
    renderVisualizationPanel();
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

		process();

        window.clear(sf::Color(30, 30, 30));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}