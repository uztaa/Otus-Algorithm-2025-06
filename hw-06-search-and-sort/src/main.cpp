#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>  // для std::iota


constexpr int WindowWidth = 800;
constexpr int WindowHeight = 600;
constexpr int NumElements = 20;

void drawArray(sf::RenderWindow& window, const std::vector<int>& data) {
    float barWidth = static_cast<float>(WindowWidth) / data.size();

    for (size_t i = 0; i < data.size(); ++i) {
        float heightRatio = static_cast<float>(data[i]) / NumElements;
        float barHeight = heightRatio * WindowHeight;

        sf::RectangleShape bar;
        bar.setSize({ barWidth - 1.0f, -barHeight }); // вверх ногами — от нижнего края
        bar.setPosition({ i * barWidth, WindowHeight });
        bar.setFillColor(sf::Color::White);

        window.draw(bar);
    }
}

int main() {
    // Окно
    sf::RenderWindow window;
    window.create(sf::VideoMode({ WindowWidth, WindowHeight }), "Sorting Visualizer", sf::Style::Default);

    // Генерация массива
    std::vector<int> array(NumElements);
    std::iota(array.begin(), array.end(), 1);
    std::shuffle(array.begin(), array.end(), std::mt19937{ std::random_device{}() });

    while (window.isOpen()) {
        while (auto opt = window.pollEvent()) {
            const sf::Event& event = *opt;
            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        drawArray(window, array);
        window.display();
    }

    return 0;
}
