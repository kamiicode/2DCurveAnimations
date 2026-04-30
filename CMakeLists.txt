cmake_minimum_required(VERSION 3.16)

project(Animations)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Animations ${CMAKE_SOURCE_DIR}/src/Main.cpp)

# Hide console window
# if (MSVC) # https://gist.github.com/optimalistel/dab127b58883b002a5ad
# 	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

# 	# for release mode
# 	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
# else()
#     target_link_options(Animations PRIVATE "-mwindows")
# endif()

# Raylib shared macro (so that it uses dllexport)
target_compile_definitions(Animations PUBLIC USE_LIBTYPE_SHARED=1)

target_include_directories(Animations PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_directories(Animations PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(Animations raylib)