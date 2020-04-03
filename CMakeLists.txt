# Версия CMake
cmake_minimum_required(VERSION 3.5)

# Название проекта (решение в Visual Studio)
project(QtHelloWorld)

# Стандарт С/С++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Устанавливаем каталоги для выходных файлов
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)

# Gui приложение на базе QT (калькулятор)
add_subdirectory("Sources/Calculator")
