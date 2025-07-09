cmake_minimum_required(VERSION 3.28)
project(AdvCppSimPixel)

set(CMAKE_CXX_STANDARD 17)

find_package(benchmark CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

if (MSVC)
    add_compile_options(/W4 /Wall /permissive-)

    set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Ob0 /Od /RTC1")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /Zi /O2 /Ob1 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "/MD /O1 /Ob1 /DNDEBUG")
else()
    add_compile_options(-Wall -Wextra -Wpedantic)

    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
endif()

add_executable(AdvCppSimPixel main.cpp
        PixelBoard.cpp
        PixelBoard.h
        BoardPresenter.cpp
        BoardPresenter.h
)

target_link_libraries(AdvCppSimPixel
        #benchmark::benchmark
        #benchmark::benchmark_main
        ${OpenCV_LIBS}
)
