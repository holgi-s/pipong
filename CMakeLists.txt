cmake_minimum_required(VERSION 3.6)
project(LedBar)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp PiGpio.cpp LedController2.cpp RangeController.cpp sleep.cpp TextDisplay.cpp BinaryDisplay.cpp FrameBuffer.cpp GameApp.cpp)
if(WIN32)
set(SOURCE_FILES ${SOURCE_FILES} pigpio_interface.cpp)
endif()

add_executable(ledbar ${SOURCE_FILES})

if(WIN32)
    target_link_libraries(ledbar)
else()
    target_link_libraries(ledbar pigpiod_if2 pthread)
endif()