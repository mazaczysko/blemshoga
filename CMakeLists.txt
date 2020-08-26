cmake_minimum_required( VERSION 3.3 )
project(blemshoga)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall --std=gnu11 -pthread")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -march=native -DNDEBUG -s")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fno-builtin")

# Libs
link_libraries(
	"allegro"
	"allegro_font"
	"lua"
)

add_executable(
	blemshoga
	"${PROJECT_SOURCE_DIR}/src/blemshoga.c"
	"${PROJECT_SOURCE_DIR}/src/log.c"
	"${PROJECT_SOURCE_DIR}/src/lapi.c"
	"${PROJECT_SOURCE_DIR}/src/tile.c"
	"${PROJECT_SOURCE_DIR}/src/resmgr.c"
	"${PROJECT_SOURCE_DIR}/src/utils.c"
)


# Copy resources
file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION "${PROJECT_BINARY_DIR}")
