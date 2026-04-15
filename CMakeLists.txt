cmake_minimum_required(VERSION 3.10)
project(imgdwnldr)
include(FetchContent)
FetchContent_Declare(
    rapidcsv
    GIT_REPOSITORY https://github.com/d99kris/rapidcsv.git
    GIT_TAG v8.90
)
FetchContent_MakeAvailable(rapidcsv)
find_package(CURL REQUIRED)
find_package(RAPIDJSON REQUIRED)
file(GLOB SOURCES "*.cpp" "include/*.cpp")
add_executable(imgdwnldr ${SOURCES})
target_include_directories(imgdwnldr PRIVATE ${RAPIDJSON_INCLUDE_DIRS} include)
target_link_libraries(imgdwnldr PRIVATE CURL::libcurl rapidcsv)