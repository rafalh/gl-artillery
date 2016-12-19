OpenGL Artillery
================

This was my project for graphics classes at my university (studing information technology).

Exercise
--------
Futuristic artillery - on the hill levitates plasma cannon firing distant city
covered by a dome (armor or force field). You can see stabilization, targeting and gun recoil.

We were allowed to use OpenGL and GLU functions only - no 3D engines, no mesh loaders, no 3D modeling software.
I had to make entire scene in code.

Most people used OpenGL 1.1 (no shaders) but I wanted to handle it the modern way.
Project uses OpenGL 3.x (core profile) - all effects are implemented in shaders.

Features
--------

* glowing projectile
* dynamicly changing force field
* dynamic lighting from projectile
* terrain loaded from highmap

Building
--------
Use CMake to prepare project files.
Compilation under Visual Studio 2015 was tested but other compilers should work too (I used mingw to compile
this project in the past).

Before running project make sure that working directory is set to project root (needed for shaders and textures to load)
and copy vendor/FreeImage/FreeImage.dll to directory where EXE was built (on Linux you have to install FreeImage somehow).

Screenshot
-----------

![alt tag](https://raw.githubusercontent.com/rafalh/opengl-artillery/master/doc/screenshot.jpg)
