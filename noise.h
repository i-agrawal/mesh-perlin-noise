#ifndef __noise_h__
#define __noise_h__

void init_noise(long int);
float noise(float, float);

#endif

  // for (int j = 0; j < N-1; j++) {
  //   for (int i = 0; i < N; i++) {
  //     parity = i & 1;
  //     x = -1.0f + 2 * (float)i / N + parity * inc;
  //     y = -1.0f + 2 * (float)j / N;
  //     k = j*N + i;
  //     if (parity) {
  //       glBegin(GL_TRIANGLES);
  //       glVertex3f(        x,         -y, map[k]);
  //       glVertex3f(        x, -(y + inc), map[k+N]);
  //       glVertex3f(x - 2*inc, -(y + inc), map[k+N-1]);
  //       glEnd();
  //     }
  //     else {
  //       glBegin(GL_TRIANGLES);
  //       glVertex3f(        x,         -y, map[k]);
  //       glVertex3f(x + 2*inc,         -y, map[k+1]);
  //       glVertex3f(        x, -(y + inc), map[k+N]);
  //       glEnd();
  //     } 
  //   }
  // }