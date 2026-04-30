cmake_minimum_required(VERSION 3.16)

project(2DCurveAnimations)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(2DCurveAnimations ${CMAKE_SOURCE_DIR}/src/Main.cpp)

# Hide console window
if (MSVC) # https://gist.github.com/optimalistel/dab127b58883b002a5ad
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
else()
    target_link_options(2DCurveAnimations PRIVATE "-mwindows")
endif()

# Raylib shared macro (so that it uses dllexport)
target_compile_definitions(2DCurveAnimations PUBLIC USE_LIBTYPE_SHARED=1)

target_include_directories(2DCurveAnimations PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_directories(2DCurveAnimations PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(2DCurveAnimations raylib)