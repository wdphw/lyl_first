cmake_minimum_required(VERSION 3.10)
project(lyl_project)


set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER "/usr/local/5.4.0/bin/arm-linux-gcc" CACHE STRING "Updated variable" FORCE)
set(CMAKE_PREFIX_PATH "/usr/local/SQ")

set(CMAKE_C_STANDARD 99)#C99 # lvgl officially support C99 and above
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#指定链接器链接的时候需要链接的库
SET(GCC_USER_LINK_FLAGS "-lm -lpthread -lfreetype -lsqlite3 -lasound")

#指定include目录
include_directories(include)
include_directories(lvgl)
include_directories(/usr/local/SQ/include)
include_directories(/_install/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#设置PAGE变量，然后这个变量等于“page/*.c”
file(GLOB_RECURSE PAGE "page/*.c")

add_subdirectory(lvgl)
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR})

link_directories(/usr/local/SQ/lib)
link_directories(/_install/lib)

add_executable(${PROJECT_NAME} "main.c" "mouse_cursor_icon.c" "ui/ui.c" ${PAGE})

target_link_libraries(${PROJECT_NAME} PRIVATE ${GCC_USER_LINK_FLAGS} lvgl lvgl::examples lvgl::demos lvgl::thorvg)
add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME} DEPENDS ${PROJECT_NAME})

#cmake -D CMAKE_C_COMPILER=/usr/local/5.4.0/bin/arm-linux-gcc -D CMAKE_PREFIX_PATH=/usr/local/SQ ..
