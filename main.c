#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "noise.h"

#define n 100

static const int pts  = n*n;
static const int tris = n*n - n;

static float map[n*n];
static float indices[9*(n*n - n)];

static float rx = 90.0f;
static float ry = 0.0f;

void special(int key, int x, int y) {

  float rate = 5.0f;

  /* rotate according to key */
  switch(key) {
    case GLUT_KEY_UP:
      rx += rate;
      glutPostRedisplay();
      break;
    case GLUT_KEY_DOWN:
      rx -= rate;
      glutPostRedisplay();
      break;
    case GLUT_KEY_LEFT:
      ry += rate;
      glutPostRedisplay();
      break;
    case GLUT_KEY_RIGHT:
      ry -= rate;
      glutPostRedisplay();
      break;
    default:
      break;
  }
}

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
  glLoadIdentity();

  /* rotate view */
  glRotatef(rx, 1, 0, 0);
  glRotatef(ry, 0, 1, 0);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

  /* draw a triangle */
  for (int i = 0; i < 9*tris; i += 9) {
    glBegin(GL_TRIANGLES);
    glVertex3f(  indices[i], indices[i+1], indices[i+2]);
    glVertex3f(indices[i+3], indices[i+4], indices[i+5]);
    glVertex3f(indices[i+6], indices[i+7], indices[i+8]);
    glEnd();
  }

  /* swap our buffer to screen buffer */
  glutSwapBuffers();
}

void check(unsigned char key, int a, int b) {
  if (key == 'q')
    exit(0);
}

int main(int argc, char** argv) {
  /* check for seed */
  if (argc != 2) {
    printf("usage: ./simul [seed number]\n");
    return 1;
  }

  /* init noise */
  init_noise(strtol(argv[1], 0, 10));

  /* generate perlin noise map */
  int i;
  float x, y;
  for (i = 0; i < pts; i++) {
    x = (float)(i % n) / n;
    y = (float)(i / n) / n;
    map[i] = noise(x, y);
  }

  /* precalculate triangle indices */
  int parity;
  float inc = 2.0f / n;
  for (i = 0; i < tris; i++) {
    parity = i & 1;
    x = -1.0f + 2 * (float)(i % n) / n + parity * inc;
    y = -1.0f + 2 * (float)(i / n) / n;

    indices[9*i]   = x;
    indices[9*i+1] = -y;
    indices[9*i+2] = map[i];

    indices[9*i+3] = x;
    indices[9*i+4] = -(y + inc);
    indices[9*i+5] = map[i+n];

    indices[9*i+6] = x + 2 * inc - 4 * parity * inc;
    indices[9*i+7] = -y - parity * inc;
    indices[9*i+8] = map[i + 1 + parity*(n-2)];
  }

  /* init glut */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  /* init window */
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(600, 600);
  glutCreateWindow("window name");

  /* set callback functions */
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  // glutIdleFunc(render);
  glutKeyboardFunc(check);
  glutSpecialFunc(special);

  /* begin event loop */
  glutMainLoop();

  return 0;
}
