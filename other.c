#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "noise.h"

#define H 100
#define W 100

float map[H*W];


void resize(int w, int h) {
  /* get ratio */
  if (h == 0) h = 1;
  float ratio = 1.0f * w / h;
  
  /* project to new screen */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  
  /* return to model view */
  glMatrixMode(GL_MODELVIEW);
}

void render() {
  /* clear color and depth */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* draw a triangle */
  glBegin(GL_TRIANGLES);
  glVertex3f(-0.5f, -0.5f, 0.0f);
  glVertex3f(0.0f, 0.5f, 0.0f);
  glVertex3f(0.5f, -0.5f, 0.0f);
  glEnd();

  /* swap our buffer to screen buffer */
  glutSwapBuffers();
}

void check(int key, int a, int b) {
  if (key == 'q')
    exit(0);
}

int main(int argc, char** argv) {
  /* init noise */
  init_noise();

  /* generate perlin noise map */
  int i, j;
  float x, y;
  for (j = 0; j < H; j++) {
    for (i = 0; i < W; i++) {
      x = (float)i / W;
      y = (float)j / H;
      map[j * W + i] = noise(x, y);
    }
  }

  /* init glut */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  /* init window */
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(800, 600);
  glutCreateWindow("window name");

  /* set callback functions */
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  glutIdleFunc(render);
  glutKeyboardFunc(check);

  /* begin event loop */
  glutMainLoop();

  return 0;
}
