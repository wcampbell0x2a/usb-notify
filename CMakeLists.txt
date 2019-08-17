cmake_minimum_required(VERSION 3.6)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Default)
add_compile_options(-W -Wall -Wextra -ggdb)

project (usb-notify)

# Setup output directory of bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

# Link with src library
add_executable(usb-notify src/usb-notify.c)

# Link with libnotify and libudev
find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET libnotify libudev)
target_link_libraries(usb-notify PkgConfig::deps)


