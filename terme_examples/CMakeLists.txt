#----------------------- base configuration
cmake_minimum_required(VERSION 3.22)
project(terme_examples VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#----------------------- sources
file(GLOB_RECURSE SRC "terme_examples/*.cpp")

add_executable(
    ${PROJECT_NAME}
    ${SRC}
)
#------------------- link levels
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/terme_examples/platformer)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/terme_examples/pong)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/terme_examples/space_invaders)

#--------------------------------------------- conan linking
find_package(nbkit REQUIRED)
find_package(terme REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE nbkit::nbkit terme::terme)

#------------------- copy files to build directory
# remove all files from persistence folder
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/persistence
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/persistence
    COMMENT "Deleting all files inside resources/Persistence before build"
)

# copy resources to build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
)
