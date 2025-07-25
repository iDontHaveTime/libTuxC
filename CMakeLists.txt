cmake_minimum_required(VERSION 3.10.0)
project(tuxc VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE LIBFILES "lib/*.c")

file(GLOB CPPFILES "lib/cpp/*.cpp")

add_library(tuxc SHARED ${LIBFILES} ${CPPFILES})
target_include_directories(tuxc PUBLIC "include")

target_compile_options(tuxc PRIVATE
    -ffreestanding
    -nostdinc
    -fno-builtin
)

target_link_options(tuxc PRIVATE 
    -nostdlib
    -lgcc
)

file(GLOB CRTFILE "crt/runtime.c")

add_library(crt OBJECT ${CRTFILE})

target_include_directories(crt PUBLIC "include")

target_compile_options(crt PRIVATE
    -ffreestanding
    -nostdinc
    -fno-builtin
)

target_link_options(crt PRIVATE 
    -nostdlib
    -lgcc
)

file(GLOB SRCFILES "src/*.c")

add_executable(tuxc_elf ${SRCFILES})

target_include_directories(tuxc_elf PUBLIC "include")
target_link_libraries(tuxc_elf PUBLIC tuxc crt)

target_compile_options(tuxc_elf PRIVATE
    -ffreestanding
    -nostdinc
    -fno-builtin
)

target_link_options(tuxc_elf PRIVATE 
    -nostdlib
    -lgcc
)