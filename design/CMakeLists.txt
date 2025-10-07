cmake_minimum_required(VERSION 3.21)
project(Agenda LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Encontrar Qt6 Widgets ---
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# --- Ativar automoc, autouic, autorcc ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# --- Includes ---
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# --- Fontes comuns ---
set(COMMON_SOURCES
    src/Calendar.cpp
    src/CLI.cpp
    src/Event.cpp
    src/JsonPersistence.cpp
    src/RecurrenceRule.cpp
    src/Time_x.cpp
    src/User.cpp
)

# --- Executável CLI ---
add_executable(agenda_cli
    src/main.cpp
    ${COMMON_SOURCES}
)
target_compile_features(agenda_cli PRIVATE cxx_std_17)

# --- Executável GUI ---
add_executable(agenda_gui
    src/main_gui.cpp
    src/GUI.cpp
    ${COMMON_SOURCES}
)
target_link_libraries(agenda_gui PRIVATE Qt6::Widgets)
target_compile_features(agenda_gui PRIVATE cxx_std_17)
