cmake_minimum_required(VERSION 3.16)
project(agenda_poo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Apenas biblioteca vazia para compilar cabeçalhos
add_library(agenda_core INTERFACE)
target_include_directories(agenda_core INTERFACE include)

# Executável placeholder (sem código ainda)
add_executable(agenda_cli src/main.cpp)
target_link_libraries(agenda_cli PRIVATE agenda_core)