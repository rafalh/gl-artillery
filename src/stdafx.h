#ifndef STDAFX_H
#define STDAFX_H

#include <cstddef>
#include <cstdio>
#include <ctime>
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <array>
#include <memory>
#ifdef WIN32
#  include <direct.h>
#endif

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#ifndef WIN32
#  include <GL/glx.h>
#endif

// GLFW
#include <GLFW/glfw3.h>
#undef APIENTRY // fix MSVC warning

// FreeImage
#include <FreeImage.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#endif // STDAFX_H
