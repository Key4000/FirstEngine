#минимальная версия cmake
cmake_minimum_required(VERSION 4.2.1)
#устанавливаем имя 
set(PROJECT_NAME FirstEngine)
project(${PROJECT_NAME})

#добавляем поддиректории
add_subdirectory(FirstEngineCore)
add_subdirectory(FirstEngineEditor)

#выбрать таргет SimpleEngineEditor , как стартовый(при сборке)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT FirstEngineEditor)