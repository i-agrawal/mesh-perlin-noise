#include "noise.h"
#include <stdlib.h>

int perm[512];

void init_noise(long int seed) {
  int i, permutation[] = { 
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
    7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
    190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
    203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
    20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27,
    166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230,
    220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
    63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
    200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173,
    186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118,
    126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
    189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163,
    70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
    98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246,
    97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
    241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
    199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150,
    254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128,
    195, 78, 66, 215, 61, 156, 180
  };

  if (seed) {
    srand(seed);
    int j, temp;
    for (i = 0; i < 256; i++) {
      j = rand() & 255;
      temp = permutation[i];
      permutation[i] = permutation[j];
      permutation[j] = temp;
    }
  }

  for (i = 0; i < 256; i++)
     perm[256 + i] = perm[i] = permutation[i];
}

inline float grad(int hash, float x, float y) {
  int first = (hash & 1) << 1;
  int second = hash & 2;
  return  x - first * x +
          y - second * y;
}

inline float lerp(float a, float b, float s) {
  return a + s * (b - a);
}

float noise(float x, float y) {
  int xi, yi, xg,yg, a, b, aa, ab, ba, bb;
  float xd, yd, u, v, g0, g1, g2, g3, i0, i1;

  /*    we have a grid like this

          0    1    2    3    4    5
        0 ┌────┬────┬────┬────┬────┬
          │    │    │    │    │    │
        1 ├────┼────*────┼────┼────┼
          │    │    │ •  │    │    │
        2 ├────┼────┼────┼────┼────┼
          │    │    │    │    │    │
        3 ├────┼────┼────┼────┼────┼

        in this example we have a point at (2.38, 1.5)

        x = 2.38
        y = 1.5

        we need to get the top left corner (the '*')
        to do this we just floor the x and y (i.e round down)

        xi = floor(2.38) = 2
        yi = floor(1.5)  = 1
  */
  xi = (int)x;
  yi = (int)y;



  /*    then we need to get the distance of the point to
        the corner

        xd = x - floor(x) = 2.38 - 2 = 0.38
        yd = y - floor(y) = 1.5  - 1 = 0.5
  */
  xd = x - (int)x;
  yd = y - (int)y;



  /*    we have to make the point fit in the
        permutation array of 256 so we take
        p % 256 (i.e. x & 255 and y & 255)

        xg = xi % 256 = 2 % 256 = 2
        yg = yi % 256 = 1 % 256 = 1
  */
  xg = xi & 255;
  yg = yi & 255;



  /*    then we need to smooth the distance
        so we use the smoothing function

        6t^5 - 15t^4 + 10t^3

        u = smooth(xd) ~ 0.2835
        v = smooth(yd) ~ 0.5
  */
  u = xd * xd * xd * (xd * (6 * xd - 15) + 10);
  v = yd * yd * yd * (yd * (6 * yd - 15) + 10);



  /*    next we take permutations of points
        by doing perm[perm[x] + y] to find the hash
        but we have to do it for all four points

        depends on your perm in the beginning
        for ours it is
        perm[perm[2]+1] = perm[137 + 1] = 118
        perm[perm[2]+2] = perm[137 + 2] = 126
        perm[perm[3]+1] = perm[91  + 1] = 244
        perm[perm[3]+2] = perm[91  + 2] = 102
  */
  a = perm[xg];
  b = perm[xg+1];
  aa = perm[a+yg];     // (x,y)
  ab = perm[a+yg+1];   // (x,y+1)
  ba = perm[b+yg];     // (x+1,y)
  bb = perm[b+yg+1];   // (x+1,y+2)



  /*     now we get the gradient for all
         four corners by adding/subtracting x and y
         first we make sure it is 0 through 3 by
         modding by 4 (hash & 3)
            hash & 3 == 0
               x + y
            hash & 3 == 1
              -x + y
            hash & 3 == 2
               x - y
            hash & 3 == 3
              -x - y

         g0 = grad(118 % 4, 2.38, 1.5) = grad(2, 2.38, 1.5) 
            = 2.38 - 1.5 = 0.88
         g1 = grad(126 % 4, 2.38, 0.5) = grad(2, 2.38, 0.5) 
            = 2.38 - 0.5 = 1.88
         g2 = grad(244 % 4, 1.38, 1.5) = grad(0, 1.38, 1.5) 
            = 1.38 + 1.5 = 2.88
         g3 = grad(102 % 4, 1.38, 0.5) = grad(2, 1.38, 0.5) 
            = 1.38 - 1.5 = -0.12
  */
  g0 = grad(aa, x,   y);
  g1 = grad(ab, x,   y-1);
  g2 = grad(ba, x-1, y);
  g3 = grad(bb, x-1, y-1);




  /*      now we linearly interpolate between the
          gradients for the two points on the left and
          right side with smoothed distance

          lerp(a, b, s) = a + s * (b - a)

          i0 = lerp(g0, g2, u) = 0.88 + 0.2835 * (2.88 - 0.88) = 1.447
          i1 = lerp(g1, g3, u) = 1.88 + 0.2835 * (-0.12 - 1.88) = 1.313
  */
  i0 = lerp(g0, g2, u);
  i1 = lerp(g1, g3, u);




  /*      now we linearly interpolate between the
          two linear interpolations

          lerp(a, b, s) = a + s * (b - a)

          final = lerp(i0, i1, v) = 1.447 + 0.5 * (1.313 - 1.447) = 1.38
  */
  return lerp(i0, i1, v);
}