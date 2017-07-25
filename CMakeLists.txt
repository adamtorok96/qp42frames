cmake_minimum_required(VERSION 3.8)
project(qp42frames)

set(CMAKE_CXX_STANDARD 11)

include(FindLua REQUIRED)

include_directories(${LUA_INCLUDE_DIR})

set(PROJECT_NAME qp4frames)
set(SOURCE_FILES main.cpp Frame.cpp Frame.h Clip.cpp Clip.h Embed.cpp Embed.h Matrix.cpp Matrix.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${LUA_LIBRARIES})