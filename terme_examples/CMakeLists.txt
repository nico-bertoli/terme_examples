cmake_minimum_required(VERSION 3.22)
project(terme_examples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Level sources
file(GLOB PLATFORMER_SRC "terme_examples/platformer/*.cpp")
file(GLOB PONG_SRC "terme_examples/pong/*.cpp")
file(GLOB SPACE_INVADERS_SRC "terme_examples/space_invaders/*.cpp")

# Main sources
file(GLOB ENGINE_DEMO_SRC "terme_examples/*.cpp")

add_executable(
    ${PROJECT_NAME}
    ${ENGINE_DEMO_SRC}
    ${PLATFORMER_SRC}
    ${PONG_SRC}
    ${SPACE_INVADERS_SRC}
)

#------------------- link levels
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/terme_examples/platformer)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/terme_examples/pong)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/terme_examples/space_invaders)

#---------------------------------------- nbkit
include(FetchContent)
set(nbkit_version 1.0.0)

FetchContent_Declare(  # Download library
    nbkit
    URL https://raw.githubusercontent.com/nico-bertoli/nbkit/master/packages/nbkit-${nbkit_version}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_MakeAvailable(nbkit)  # Configure library and make it available

#---------------------------------------- Link Terme
set(terme_version 1.0.0)

FetchContent_Declare(  # Download library
    terme
    URL https://raw.githubusercontent.com/nico-bertoli/terme/main/terme/packages/terme-${terme_version}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_MakeAvailable(terme)  # Configure library and make it available

# Link Terme library
target_link_libraries(${PROJECT_NAME} terme)

#------------------- copy files to build directory
# set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/terme_examples/Dependencies/SFML")
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         $<$<CONFIG:Debug>:${SFML_DIR}/bin/sfml-system-d-2.dll>
#         $<$<CONFIG:Debug>:${SFML_DIR}/bin/sfml-audio-d-2.dll>
#         $<$<NOT:$<CONFIG:Debug>>:${SFML_DIR}/bin/sfml-system-2.dll>
#         $<$<NOT:$<CONFIG:Debug>>:${SFML_DIR}/bin/sfml-audio-2.dll>
#         $<TARGET_FILE_DIR:${PROJECT_NAME}>
# )

# resources folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
)

# remove all files from persistence folder
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/persistence
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/persistence
    COMMENT "Deleting all files inside resources/Persistence before build"
)