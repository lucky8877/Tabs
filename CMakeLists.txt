cmake_minimum_required(VERSION 3.14)
project(Table)

# Все исходные файлы
add_executable(main 
    src/main.cpp
    src/Table/ArrayTable.cpp
    src/Table/HashArray.cpp
    src/Table/HashTable.cpp
    src/Table/ScanTable.cpp
    src/Table/TabRecord.cpp
    src/Table/SortTable.cpp
    src/Table/TreeTable.cpp
)

# Подключаем заголовки
target_include_directories(main PRIVATE include)