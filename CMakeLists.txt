cmake_minimum_required (VERSION 3.1)

project (artillery)

#################### Global Options ####################

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
	$<$<CONFIG:Debug>:_DEBUG;DEBUG>
	$<$<CONFIG:Release>:NDEBUG>
)

if (UNIX)
	set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS LINUX)
elseif (WIN32)
	set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS WIN32)
endif()

if (UNIX OR MINGW)
	add_compile_options(-Wall)
elseif (MSVC)
	set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
		_CRT_SECURE_NO_WARNINGS __func__=__FUNCTION__)
	add_compile_options(/W4)
endif()

add_subdirectory(vendor)

#################### artillery ####################

link_directories(vendor/FreeImage)

aux_source_directory(src SOURCES)
file(GLOB_RECURSE HEADERS src/*.h)

add_executable(artillery ${SOURCES} ${HEADERS})

target_include_directories(artillery PRIVATE
	src
	vendor
	vendor/FreeImage
	vendor/glfw/include
	vendor/glew/include
)

target_link_libraries(artillery freeimage glfw glew)
if(WIN32)
	target_link_libraries(artillery gdi32 opengl32)
elseif(UNIX)
	target_link_libraries(artillery GL pthread X11 Xxf86vm Xrandr Xi)
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT artillery)
set_property(TARGET artillery PROPERTY CXX_STANDARD 11)

# Precompiled Header
include(cmake/cotire.cmake)
set_target_properties(artillery PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "src/stdafx.h")
cotire(artillery)
