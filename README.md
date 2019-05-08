# Plane_CG

Using SOIL.
Preferred installation on Ubuntu.
install 
g++
cc
opengl

Load SOIL libraries present in src folder
using:
sudo apt-get install libsoil-dev
then compile using:
g++ -g source.cpp -lglu32 -lSOIL -lopengl32 -lfreeglut