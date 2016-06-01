cmake_minimum_required(VERSION 2.6)
project(hello)

link_directories(/usr/lib/x86_64-linux-gnu)

# link the libraries to the executable
link_libraries (libsqlite3.so)


add_executable(hello main.cpp)


install(TARGETS hello RUNTIME DESTINATION bin)