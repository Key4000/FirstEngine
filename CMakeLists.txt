#минимальная версия cmake
cmake_minimum_required(VERSION 4.2.1)
#устанавливаем имя 
set(PROJECT_NAME FirstEngine)

#добавляем поддиректории
add_subdirectory(FirstEngineCore)
add_subdirectory(FirstEngineEditor)

