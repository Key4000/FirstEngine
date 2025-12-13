#минимальная версия cmake
cmake_minimum_required(VERSION 4.2.1)
#устанавливаем имя 
set(ENGINE_PROJECT_NAME FirstEngineCore)

#создаем библиотеку
add_library(${ENGINE_PROJECT_NAME} STATIC 
    src/FirstEngineCore/Utils/test.cpp
    includes/FirstEngineCore/Utils/test.hpp
)

#подключение к линковке таргетов(внешние библиотеки)
target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
#подключение с++17
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)