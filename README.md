# Mesh Perlin Noise

This program renders a rotatable mesh using OpenGL and GLUT. The mesh is terrain generated by the perlin noise. The mesh is often very round and looks like hills because it does not use octaves.  
  
Rotate using the arrow keys and exit by pressing 'q'.  

![unable to load image](/res/img.png)

Confirmed to work on Mac OSX and Linux. On linux please change the Makefile to link the OpenGL and GLUT libraries if they are in non standard places.