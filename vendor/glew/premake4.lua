project "glew"
	kind "StaticLib"
	includedirs "include"
	objdir "obj"
	targetdir "lib"
	
	files {
		"src/glew.c",
		"include/GL/*.h",
	}
	
	defines {
		"GLEW_NO_GLU", "GLEW_STATIC"
	}
