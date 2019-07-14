cmake_minimum_required(VERSION 3.6)
project (usb-notify)

add_executable(usb-notify usb-notify.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET libnotify)
target_link_libraries(usb-notify PkgConfig::deps)
