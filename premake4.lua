solution "Artillery"
	configurations {"Debug", "Release"}
	language "C++"
	
	configuration "Debug"
		defines {"_DEBUG", "DEBUG"}
		flags "Symbols"
	
	configuration "Release"
		defines "NDEBUG"
		flags "OptimizeSize"
	
	configuration "linux"
		defines "LINUX"
	
	configuration "windows"
		defines "WIN32"
	
	configuration "vs*"
		defines {"_CRT_SECURE_NO_WARNINGS", "__func__=__FUNCTION__"}
	
	include "vendor"
	
	project "Artillery"
		includedirs
		{
			"vendor",
			"vendor/FreeImage",
			"vendor/glfw/include",
			"vendor/glew/include",
		}
		
		libdirs
		{
			"vendor/FreeImage",
			"vendor/glew/lib",
		}
		
		files
		{
			"src/*.h",
			"src/*.cpp",
		}
		
		links
		{
			"freeimage",
			"glfw3",
			"glew",
		}
		
		-- Precompiled header
		includedirs "src"
		pchheader "stdafx.h"
		pchsource "src/stdafx.cpp"
		
		-- Enable C++11 for GCC
		configuration "gmake or codeblocks"
			buildoptions "-std=c++11 -fno-strict-aliasing"
		
		configuration {"windows", "gmake or codeblocks"}
			libdirs "vendor/glfw/lib/mingw"
		
		configuration {"linux", "gmake or codeblocks"}
			if os.is64bit() then
				libdirs "vendor/glfw/lib/gcc_x64"
			else
				libdirs "vendor/glfw/lib/gcc" -- TODO
			end
		
		configuration "vs*"
			libdirs "vendor/glfw/lib/msvc"
		
		configuration "windows"
			links {"gdi32", "opengl32"}
		
		configuration "linux"
			links {"GL", "pthread", "X11", "Xxf86vm", "Xrandr", "Xi"}
		
		configuration "Debug"
			kind "ConsoleApp"
			targetdir "bin/debug"
			objdir "obj/debug"
		
		configuration "Release"
			--kind "WindowedApp"
			kind "ConsoleApp"
			targetdir "bin/release"
			objdir "obj/release"
