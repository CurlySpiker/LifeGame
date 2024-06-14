CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

PROJECT(LifeGame VERSION 1.0)

configure_file(config.h.in config.h)

file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(LifeGame main.cpp ${SOURCES})
target_include_directories(LifeGame PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
)

find_package(Catch2 3 REQUIRED PATHS ./catch2_install/lib/cmake/Catch2)
add_executable(LifeGameTest test/test.cpp ${SOURCES})
target_link_libraries(LifeGameTest PRIVATE Catch2::Catch2WithMain)

target_include_directories(LifeGameTest PUBLIC
    "${PROJECT_BINARY_DIR}/"
    "${PROJECT_SOURCE_DIR}/test"
    "${PROJECT_SOURCE_DIR}/src"
)
