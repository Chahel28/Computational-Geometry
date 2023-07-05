Triangulation.cpp conatins the triangulation code; 
Triangulation_with_OpenGL.cpp contains the extra code for plotting on OpenGL
To run it, ensure that Open GL is installed on the system and run the following commands on terminal (Ubuntu):
$ g++ Triangulation_with_OpenGL.cpp -o tri -lglut -lGLU -lGL
$ ./tri
<Enter input from here>

if you want to give a file as input, use the following:
$ ./tri < poly1.txt

poly1-12.txt contains polygons with vertices in counter clockwise direction at random distances from origin. No of vertices varies from 10 to 5000.
cutepoly1-3.txt contains polygons with 15-25 verticies that are more spaced out.

As the algorithm is O(n^2), the input of 5000 vertices takes roughly 1.5 sec, corresponding to 10^7 to 10^8 operations that roughly match our claim.

Test cases were generated using test-specific.cpp
