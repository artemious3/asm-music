cmake_minimum_required(VERSION 3.28)
project(asmmusic LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-no-pie")


find_package(SFML 3 REQUIRED Audio Graphics Window System)


# Copy resources to binary dir
file(COPY ./sharp.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./ui/fonts/PixeloidMono.ttf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# ASM part
add_custom_command(OUTPUT asm/libSequencer.a
		   COMMAND make
	   	   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/asm)
add_custom_target(libSequencer DEPENDS asm/libSequencer.a )
include_directories(asm)

# UI part
add_executable(main ui/main.cpp ui/MainWindow.cpp ui/InputHandler.cpp)
add_dependencies(main libSequencer)
target_link_libraries(main ${CMAKE_CURRENT_SOURCE_DIR}/asm/libSequencer.a
			    sfml-window sfml-system sfml-graphics portaudio)


