cmake_minimum_required(VERSION 3.10.2)
project(onecard)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER clang-18)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS src/*.c)

add_executable(onecard ${SOURCE_FILES})
target_link_libraries(onecard m)
target_compile_options(onecard PRIVATE -O2 -Werror -Wall -Wextra -Wno-unused-result -Wconversion -Wvla -Wpedantic -std=c11)

add_executable(ubsan ${SOURCE_FILES})
target_link_libraries(ubsan m)
target_compile_options(ubsan PRIVATE -DDEBUG -g -fsanitize=undefined,integer,bounds,float-divide-by-zero,implicit-conversion,float-cast-overflow,nullability -fno-omit-frame-pointer -fno-sanitize-recover=all)
target_link_options(ubsan PRIVATE -fsanitize=undefined)

add_executable(asan ${SOURCE_FILES})
target_link_libraries(asan m)
target_compile_options(asan PRIVATE -DDEBUG -g -fsanitize=address -fno-omit-frame-pointer -fno-sanitize-recover=all)
target_link_options(asan PRIVATE -fsanitize=address)

add_executable(msan ${SOURCE_FILES})
target_link_libraries(msan m)
target_compile_options(msan PRIVATE -DDEBUG -g -fsanitize=memory -fno-omit-frame-pointer -fsanitize-memory-track-origins -fPIE -O2)
target_link_options(msan PRIVATE -fsanitize=memory -pie)

