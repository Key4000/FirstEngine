#минимальная версия cmake
cmake_minimum_required(VERSION 4.2.1)

#устанавливаем имя 
set(EDITOR_PROJECT_NAME FirstEngineEditor)

#создание exe файла
add_executable(${EDITOR_PROJECT_NAME}
    src/main.cpp
)

#подключение к линковке таргетов(наша библиотека - движок)
target_link_libraries(${EDITOR_PROJECT_NAME} FirstEngineCore ImGui glm)

#подключение с++17
target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)

#все exe генерируются в одной папке bin
set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
