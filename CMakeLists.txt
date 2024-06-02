cmake_minimum_required(VERSION 3.24)
project(WeatherApiCpp)

set(CMAKE_CXX_STANDARD 23)

add_executable(WeatherApiCpp main.cpp Weather.h Weather.cpp DayWeather.h DayWeather.cpp)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba)
FetchContent_MakeAvailable(cpr)

target_link_libraries(WeatherApiCpp PRIVATE cpr::cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(WeatherApiCpp PRIVATE nlohmann_json::nlohmann_json)