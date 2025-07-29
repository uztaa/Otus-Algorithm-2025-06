#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <vector>
#include <string>

// Простой список доступных сортировщиков
const std::vector<std::string> sorterNames = {
    "BubbleSort", "InsertionSort", "CocktailSort", "ShellSort"
};

// Размеры окна
constexpr int WindowWidth = 1280;
constexpr int WindowHeight = 720;

int main() {
    // Создаем окно SFML
    sf::RenderWindow window(sf::VideoMode({ WindowWidth, WindowHeight }), "Sorting Visualizer");
    window.setFramerateLimit(60);

    //IMGUI_CHECKVERSION();               // Вызов до CreateContext
    //ImGui::CreateContext();             // Затем создаем контекст
    /*
    Не вызывайте ImGui::CreateContext() вручную, если используете ImGui-SFML — он сам создаёт контекст внутри ImGui::SFML::Init(window).
    */
    if (!ImGui::SFML::Init(window)) {
        std::cerr << "ImGui-SFML initialization failed!" << std::endl;
        return 1;
    }

    if(!ImGui::GetCurrentContext()) {
        std::cerr << "ImGui context not created!" << std::endl;
        return 1;
    }

    ImGui::GetStyle().ScaleAllSizes(2.0f);         // увеличивает размеры всех виджетов
    ImGui::GetIO().FontGlobalScale = 2.0f;         // увеличивает масштаб всех шрифтов

    sf::Clock deltaClock;

    int currentSorterIndex = 0;
    bool isSorting = false;
    bool isPaused = false;

    std::string statusText = "Prepare for start";

    while (window.isOpen()) {
        sf::Time dt = deltaClock.restart();

        // Цикл обработки событий
        while (auto opt = window.pollEvent()) {
            const sf::Event& event = *opt;

            // обязательно вызываем ImGui обработку
            ImGui::SFML::ProcessEvent(window, event);

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, dt);


        // Панель управления: слева, фиксированного размера
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);          // отступ слева и сверху
        ImGui::SetNextWindowSize(ImVec2(300, WindowHeight - 20), ImGuiCond_Always);  // высота окна - отступы

        ImGui::Begin("Control");

        // Выбор сортировщика
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

        // Кнопки управления
        if (ImGui::Button("Start")) {
            isSorting = true;
            isPaused = false;
            statusText = "Sorting...";
            std::cout << "[Button] pushed: Start\n";
        }

        ImGui::SameLine();
        if (ImGui::Button("Pause")) {
            isPaused = true;
            statusText = "Pause";
            std::cout << "[Button] pushed: Pause\n";
        }

        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
            isSorting = false;
            isPaused = false;
            statusText = "Stopped";
            std::cout << "[Button] pushed: Stop\n";
        }

        // Текст состояния
        ImGui::Text("Status: %s", statusText.c_str());

        ImGui::End();

        // Панель визуализации: справа от Control
        ImGui::SetNextWindowPos(ImVec2(320, 10), ImGuiCond_Always);         // Control (300) + отступ (10)
        ImGui::SetNextWindowSize(ImVec2(WindowWidth - 330, WindowHeight - 20), ImGuiCond_Always);

        // Панель визуализации (будет здесь)
        ImGui::Begin("Visualization");
        ImGui::Text("Visualization in proress...");
        ImGui::End();

        window.clear(sf::Color(30, 30, 30));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
