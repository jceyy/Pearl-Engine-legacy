cmake_minimum_required(VERSION 2.6)

project(PearlEngine)
if (NOT CMAKE_CXX_FLAGS)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++11")
endif(NOT CMAKE_CXX_FLAGS)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)


include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(

	pearl-engine

	src/main.cpp

	src/PRL_animation.cpp

	src/PRL_camera.cpp

	src/PRL_collision.cpp

	src/PRL_configuration.cpp

	src/PRL_display.cpp

	src/PRL_handler.cpp

	src/PRL_HUD.cpp

	src/PRL_input.cpp

	src/PRL_math.cpp

	src/PRL_menu_functions.cpp

	src/PRL_time.cpp

)

target_link_libraries(pearl-engine ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
