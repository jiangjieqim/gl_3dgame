#ifndef matrix4_h
#define matrix4_h
/*
Copyright (c) 2016 Tim Gover

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* Very simple (and unoptimized) matrix transformations library for OpenGL.
 * OpenGL and this library use COLUMN-MAJOR layout for matrices.
 */


typedef float *vector3f;
typedef float vector3fa[3];
#define VECTOR3F(name) \
    vector3fa name = {0}

typedef float (*matrix4f)[4];
typedef float matrix4fa[4][4];

/* Helper macros for declaring an initialised matrix */
#define MATRIX4F(NAME) \
   matrix4fa NAME = {{0}}

#define MATRIX4F_IDENTITY(NAME) \
   matrix4fa NAME = { \
       1.f, 0.f, 0.f, 0.f, \
       0.f, 1.f, 0.f, 0.f, \
       0.f, 0.f, 1.f, 0.f, \
       0.f, 0.f, 0.f, 1.f }

#define MATRIX4F_SCALING(NAME, S) \
   matrix4fa NAME = { \
      (S), 0.f, 0.f, 0.f, \
      0.f, (S), 0.f, 0.f, \
      0.f, 0.f, (S), 0.f, \
      0.f, 0.f, 0.f, 1.f }

#define MATRIX4F_TRANSLATION(NAME, X, Y, Z) \
   matrix4fa NAME = { \
      1.f, 0.f, 0.f, 0.f, \
      0.f, 1.f, 0.f, 0.f, \
      0.f, 0.f, 1.f, 0.f, \
      (X), (Y), (Z), 1.f }

/* Useful transformation matrices for OpenGL */
matrix4f matrix4f_identity(matrix4f m);
matrix4f matrix4f_scaling(matrix4f m, float s);
matrix4f matrix4f_translation(matrix4f m, vector3f v);

/* right handed rotations so +ve angle is counter-clockwise assuming camera points down -ve z-axis */
matrix4f matrix4f_rotation_x(matrix4f m, float rad);
matrix4f matrix4f_rotation_y(matrix4f m, float rad);
matrix4f matrix4f_rotation_z(matrix4f m, float rad);

/* Perspective */
matrix4f matrix4f_orthographic(matrix4f m, float left, float right, float top, float bottom, float near, float far);
matrix4f matrix4f_orthographic_2d(matrix4f m, float left, float right, float top, float bottom);
matrix4f matrix4f_2d_viewport(matrix4f m, float width, float height);

/* Matrix maths */
/* m1 *= m2 */
matrix4f matrix4f_product(matrix4f m1, const matrix4f m2);

/* Utilities */
matrix4f matrix4f_transpose(matrix4f m);
matrix4f matrix4f_swap(matrix4f m1, matrix4f m2);
#define matrix4f_assign(m1, m2) memcpy((m1), (m2), sizeof(matrix4fa))
void matrix4f_print(const matrix4f m);

/* Vector 3f */
vector3f vector3f_normalize(vector3f v);
float vector3f_length(const vector3f v);
float vector3f_dot(const vector3f v1, const vector3f v2);
vector3f vector3d_cross(vector3f v1, const vector3f v2);
vector3f vector3f_add(vector3f v1, const vector3f v2);
vector3f vector3f_sub(vector3f v1, const vector3f v2);
vector3f vector3f_mul(vector3f v, float f);
vector3f vector3f_div(vector3f v, float f);
int vector3f_equals(const vector3f v1, const vector3f v2);
vector3f vector3f_set(vector3f v, float x, float y, float z);
vector3f vector3f_assign(vector3f v1, vector3f v2);

#endif 
