cmake_minimum_required(VERSION 3.6)
project(PiPong)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/PiGpio.cpp src/LedController2.cpp src/RangeController.cpp src/sleep.cpp
        src/TextDisplay.cpp src/BinaryDisplay.cpp src/FrameBuffer.cpp src/GameApp.cpp)

if(WIN32)
set(SOURCE_FILES ${SOURCE_FILES} src/pigpio_interface.cpp)
endif()

add_executable(pipong ${SOURCE_FILES})

if(WIN32)
    target_link_libraries(pipong)
else()
    target_link_libraries(pipong pigpiod_if2 pthread)
endif()