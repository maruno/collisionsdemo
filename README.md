collisionsdemo
==============
A 3D game-prototype with OpenGL Core Profile and collisions using the voronoi clipping (V-Clip) algorithm.
For my study I created this game prototype to showcase my knowledge of OpenGL and collision detection.

It's been a lot over the course of it's creation if you jump into the history, including a heightmap
generator/visualiser. In it's final form it's a prototype setup for an asteroids-like space shooter.

# Graphics
For the graphical side I used OpenGL Core Profile 3.2 with GLSL 1.50, the latest version my mac supported.
The lightning-model is based on phong shading.

# Collision detection
The collision detection algorithm is based on Object Oriented and Axis Aligned Bounding Boxes used in a scene graph
for the broad phase and the feature based Voronoi clipping (V-Clip) algorithm for the narrow phase.

For the V-Clip algorithm I could not find example implementations. I wrote this implementatation based on the
MERL TR-97-05 paper: http://www.merl.com/papers/docs/TR97-05.pdf

The V-Clip code is quite large and complex and far form perfect. It's been a good learning experience though, and
I hope it can be for other people looking at this algorithm. Beware though that it still has some bugs.

# Compiling
This prototype was developed first on Linux and finally on OS X. Linux support is still there and working, though
it might need some tweaks for your platform. The prototype uses C++11 extensively and should be build against
LLVM/Clang and libC++, which might need to be custom build on Linux.
