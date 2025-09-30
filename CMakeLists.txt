cmake_minimum_required(VERSION 3.20)

project(autoeq LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_TESTS "Build C++ unit tests" ON)
option(BUILD_PYBIND "Build Python bindings via pybind11" ON)

add_library(autoeq STATIC
    src/biquad.cpp
    src/eq3band.cpp
    src/library.cpp
)
target_include_directories(autoeq PUBLIC include)

if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(autoeq PUBLIC Threads::Threads)
endif()

# ------------------------- Tests (Catch2) -------------------------
if(BUILD_TESTS)
    include(FetchContent)
    set(CATCH_ENABLE_WERROR OFF)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(dsp_tests
        tests/test_main.cpp
        tests/test_biquad.cpp
    )
    target_link_libraries(dsp_tests PRIVATE autoeq Catch2::Catch2WithMain)
    enable_testing()
    add_test(NAME dsp_tests COMMAND dsp_tests)
endif()

# ----------------------- Python bindings -------------------------
if(BUILD_PYBIND)
    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v2.11.1
    )
    FetchContent_MakeAvailable(pybind11)

    pybind11_add_module(autoeq_py bindings/pybind_module.cpp)
    target_link_libraries(autoeq_py PRIVATE autoeq)
    target_include_directories(autoeq_py PRIVATE include)
endif()

# ----------------------- Install targets -------------------------
include(GNUInstallDirs)
install(TARGETS autoeq
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


