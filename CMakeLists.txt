cmake_minimum_required(VERSION 2.8.3)
project(rosdigit)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  cv_bridge
)

catkin_package(
    INCLUDE_DIRS src
    CATKIN_DEPENDS roscpp cv_bridge
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(rosdigit
    src/DigitRecogniser.cpp
    src/FrameProcessor.cpp
    src/main.cpp)

target_link_libraries(rosdigit ${catkin_LIBRARIES})

install(TARGETS rosdigit
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)