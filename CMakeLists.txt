cmake_minimum_required( VERSION 3.3 )
project(blemshoga)

# Source dir
set(PROJECT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall --std=c11 -pthread")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -march=native -DNDEBUG -s")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fno-builtin")

# Libs
link_libraries(
	"allegro"
	"allegro_font"
)

add_executable(
	blemshoga
	"${PROJECT_SOURCE_DIR}/blemshoga.c"
)

