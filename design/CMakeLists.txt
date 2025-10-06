cmake_minimum_required(VERSION 3.16)
project(agenda_poo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(agenda_core
    src/Time.cpp
    src/Event.cpp
    src/RecurrenceRule.cpp
    src/Calendar.cpp
    src/JsonPersistence.cpp
    src/CLI.cpp
)

add_executable(agenda_cli src/main.cpp)
target_link_libraries(agenda_cli PRIVATE agenda_core)
