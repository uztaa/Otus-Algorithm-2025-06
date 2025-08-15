#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include "Moveable.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "BitBoardPrinter.h"

// Функция для преобразования шахматной позиции в индекс
int parsePosition(const std::string& pos) {
    if (pos.size() != 2) return -1;
    char file = tolower(pos[0]);
    char rank = pos[1];

    if (file < 'a' || file > 'h') return -1;
    if (rank < '1' || rank > '8') return -1;

    int col = file - 'a';
    int row = rank - '1';
    return row * 8 + col;
}

// Фабрика для создания фигур по символу
std::unique_ptr<Moveable> createFigure(char figure) {
    switch (toupper(figure)) {
    case 'K': return std::make_unique<King>();
    case 'Q': return std::make_unique<Queen>();
    case 'R': return std::make_unique<Rook>();
    case 'B': return std::make_unique<Bishop>();
    case 'N': return std::make_unique<Knight>();
    default: return nullptr;
    }
}

// Функция для проверки команды выхода
bool isExitCommand(const std::string& input) {
    std::string lower = input;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower == "exit" || lower == "quit";
}

// Функция для печати приветственного сообщения и начального состояния доски
int main() {
    std::cout << "Welcome to the Chess Move Visualizer!" << std::endl;
    std::cout << "Enter 'exit' at any prompt to quit." << std::endl;
    std::cout << "\nInitial empty board:\n";
    BitBoardPrinter::print(0);

    while (true) {
        std::string inputFigure, inputPos;

        std::cout << "\nEnter a piece (K - king, Q - queen, R - rook, B - bishop, N - knight): ";
        std::cin >> inputFigure;
        if (isExitCommand(inputFigure)) break;

        auto figure = createFigure(inputFigure[0]);
        if (!figure) {
            std::cerr << "Invalid piece. Please enter K, Q, R, B or N.\n";
            continue;
        }

        std::cout << "Enter a position (e.g., e4): ";
        std::cin >> inputPos;
        if (isExitCommand(inputPos)) break;

        int index = parsePosition(inputPos);
        if (index < 0 || index >= 64) {
            std::cerr << "Invalid position. Must be between a1 and h8.\n";
            continue;
        }

        auto result = figure->getMoves(index);
        std::cout << "\nPossible moves: " << result.first << std::endl;
        BitBoardPrinter::print(result.second);
    }

    std::cout << "\nGoodbye!\n";
    return 0;
}
