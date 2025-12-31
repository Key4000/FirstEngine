#минимальная версия cmake
cmake_minimum_required(VERSION 4.2.1)
#устанавливаем имя 
set(ENGINE_PROJECT_NAME FirstEngineCore)

#здесь хранятся все инклуды , что хранятся публично
set(ENGINE_PUBLIC_INCLUDES 
    includes/FirstEngineCore/Application.hpp
    includes/FirstEngineCore/Log.hpp
    includes/FirstEngineCore/Event.hpp
)

#здесь хранятся все приватные инклуды
set(ENGINE_PRIVATE_INCLUDES 
    src/FirstEngineCore/Window.hpp
)

#здесь хранятся все исходные коды 
set(ENGINE_PRIVATE_SOURCES 
    src/FirstEngineCore/Application.cpp
    src/FirstEngineCore/Window.cpp
)


#создаем библиотеку
add_library(${ENGINE_PROJECT_NAME} STATIC 
    ${ENGINE_PUBLIC_INCLUDES}
    ${ENGINE_PRIVATE_INCLUDES}
    ${ENGINE_PRIVATE_SOURCES}
)

#подключение к линковке таргетов(внешние библиотеки)
target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
#подключение к линковке таргетов(приватно)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)
#подключение с++17
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

#---------------------подключение-библиотек-------------------------------

#добавляем внешнюю библиотеку (как subdirectory) благодаря тому что в glfw поддерживается cmake
add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
#прилинкуем библиотеку к движку(приватно , чтобы editor при линковке не видел библиотеку(снаружи движка ничего не будет известно о библиотеке))
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

#добавляем внешнюю библиотеку (как subdirectory) благодаря тому что в spdlog поддерживается cmake
add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
#прилинкуем библиотеку к движку(приватно , чтобы editor при линковке не видел библиотеку(снаружи движка ничего не будет известно о библиотеке))
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

#добавляем внешнюю библиотеку (как subdirectory) благодаря тому что в spdlog поддерживается cmake
add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
#прилинкуем библиотеку к движку(приватно , чтобы editor при линковке не видел библиотеку(снаружи движка ничего не будет известно о библиотеке))
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)