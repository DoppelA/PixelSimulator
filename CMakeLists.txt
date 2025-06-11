cmake_minimum_required(VERSION 3.28)
project(AdvCppSimPixel)

set(CMAKE_CXX_STANDARD 17)

find_package(benchmark CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(AdvCppSimPixel main.cpp
        PixelBoard.cpp
        PixelBoard.h
        BoardPresenter.cpp
        BoardPresenter.h
)

target_link_libraries(AdvCppSimPixel
        benchmark::benchmark
        benchmark::benchmark_main
        ${OpenCV_LIBS}
)
