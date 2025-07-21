project(Bits)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем интерфейсы из папки include
include_directories(include)

set(CPP_SOURCES
    src/main.cpp 
    src/King.cpp
    src/Knight.cpp
    src/Rook.cpp src/Bishop.cpp src/Queen.cpp 
    src/BitBoardPrinter.cpp    
)

add_library(BitsLib STATIC ${CPP_SOURCES})
add_executable(Bits src/main.cpp)
target_link_libraries(Bits PRIVATE BitsLib)

# Добавляем тесты
add_subdirectory(tests)
