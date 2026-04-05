cmake_minimum_required(VERSION 3.15)
project(terme_examples)

set(CMAKE_CXX_STANDARD 17)

find_package(terme REQUIRED)
find_package(nbkit REQUIRED)

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
endif()

set(TERME_EXAMPLES_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/terme_examples")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${TERME_EXAMPLES_SRC_DIR}/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
    "${TERME_EXAMPLES_SRC_DIR}"
    "${TERME_EXAMPLES_SRC_DIR}/platformer"
    "${TERME_EXAMPLES_SRC_DIR}/pong"
    "${TERME_EXAMPLES_SRC_DIR}/space_invaders"
)

if(UNIX AND NOT APPLE)
    target_include_directories(${PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIR})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE terme::terme nbkit::nbkit)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
endif()