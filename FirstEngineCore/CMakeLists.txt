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

#---------добавляем руками библиотеку ImGui-----------

#устанавливаем файлы под именами IMGUI_INCLUDES и IMGUI_SOURCES
set(IMGUI_INCLUDES
 ../external/imgui/imgui.h
 ../external/imgui/backends/imgui_impl_glfw.h
 ../external/imgui/backends/imgui_impl_opengl3.h
) 
set(IMGUI_SOURCES
 ../external/imgui/imgui.cpp
 ../external/imgui/backends/imgui_impl_glfw.cpp
 ../external/imgui/backends/imgui_impl_opengl3.cpp
 ../external/imgui/imgui_draw.cpp
 ../external/imgui/imgui_tables.cpp
 ../external/imgui/imgui_widgets.cpp
 ../external/imgui/imgui_demo.cpp
) 

#добавляем статичную библиотеку
add_library(ImGui STATIC
  ${IMGUI_INCLUDES} 
  ${IMGUI_SOURCES} 
) 

#дериктории подключения
target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)

#подключаем к библиотеке glfw и glad
target_link_libraries(ImGui PRIVATE glad glfw)

#подключаем библиотеку определенную сверху к проекту 
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGui)

