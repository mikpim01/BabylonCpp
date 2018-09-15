#include <babylon/extensions/noisegeneration/simplex_noise.h>

#include <random>

#include <babylon/math/vector2.h>
#include <babylon/math/vector3.h>
#include <babylon/math/vector4.h>

namespace BABYLON {
namespace Extensions {

std::array<std::array<float, 2>, 8> SimplexNoise::grad2lut{{{{-1.0f, -1.0f}},
                                                            {{1.0f, 0.0f}},
                                                            {{-1.0f, 0.0f}},
                                                            {{1.0f, 1.0f}},
                                                            {{-1.0f, 1.0f}},
                                                            {{0.0f, -1.0f}},
                                                            {{0.0f, 1.0f}},
                                                            {{1.0f, -1.0f}}}};
std::array<std::array<float, 3>, 16> SimplexNoise::grad3lut{
  {{{1.0f, 0.0f, 1.0f}},
   {{0.0f, 1.0f, 1.0f}}, // 12 cube edges
   {{-1.0f, 0.0f, 1.0f}},
   {{0.0f, -1.0f, 1.0f}},
   {{1.0f, 0.0f, -1.0f}},
   {{0.0f, 1.0f, -1.0f}},
   {{-1.0f, 0.0f, -1.0f}},
   {{0.0f, -1.0f, -1.0f}},
   {{1.0f, -1.0f, 0.0f}},
   {{1.0f, 1.0f, 0.0f}},
   {{-1.0f, 1.0f, 0.0f}},
   {{-1.0f, -1.0f, 0.0f}},
   {{1.0f, 0.0f, 1.0f}},
   {{-1.0f, 0.0f, 1.0f}}, // 4 repeats to make 16
   {{0.0f, 1.0f, -1.0f}},
   {{0.0f, -1.0f, -1.0f}}}};
std::array<std::array<float, 4>, 32> SimplexNoise::grad4lut{
  {{{0.0f, 1.0f, 1.0f, 1.0f}},
   {{0.0f, 1.0f, 1.0f, -1.0f}},
   {{0.0f, 1.0f, -1.0f, 1.0f}},
   {{0.0f, 1.0f, -1.0f, -1.0f}}, // 32 tesseract edges
   {{0.0f, -1.0f, 1.0f, 1.0f}},
   {{0.0f, -1.0f, 1.0f, -1.0f}},
   {{0.0f, -1.0f, -1.0f, 1.0f}},
   {{0.0f, -1.0f, -1.0f, -1.0f}},
   {{1.0f, 0.0f, 1.0f, 1.0f}},
   {{1.0f, 0.0f, 1.0f, -1.0f}},
   {{1.0f, 0.0f, -1.0f, 1.0f}},
   {{1.0f, 0.0f, -1.0f, -1.0f}},
   {{-1.0f, 0.0f, 1.0f, 1.0f}},
   {{-1.0f, 0.0f, 1.0f, -1.0f}},
   {{-1.0f, 0.0f, -1.0f, 1.0f}},
   {{-1.0f, 0.0f, -1.0f, -1.0f}},
   {{1.0f, 1.0f, 0.0f, 1.0f}},
   {{1.0f, 1.0f, 0.0f, -1.0f}},
   {{1.0f, -1.0f, 0.0f, 1.0f}},
   {{1.0f, -1.0f, 0.0f, -1.0f}},
   {{-1.0f, 1.0f, 0.0f, 1.0f}},
   {{-1.0f, 1.0f, 0.0f, -1.0f}},
   {{-1.0f, -1.0f, 0.0f, 1.0f}},
   {{-1.0f, -1.0f, 0.0f, -1.0f}},
   {{1.0f, 1.0f, 1.0f, 0.0f}},
   {{1.0f, 1.0f, -1.0f, 0.0f}},
   {{1.0f, -1.0f, 1.0f, 0.0f}},
   {{1.0f, -1.0f, -1.0f, 0.0f}},
   {{-1.0f, 1.0f, 1.0f, 0.0f}},
   {{-1.0f, 1.0f, -1.0f, 0.0f}},
   {{-1.0f, -1.0f, 1.0f, 0.0f}},
   {{-1.0f, -1.0f, -1.0f, 0.0f}}}};

std::array<std::array<float, 3>, 16> SimplexNoise::grad3u{
  {{{1.0f, 0.0f, 1.0f}},
   {{0.0f, 1.0f, 1.0f}}, // 12 cube edges
   {{-1.0f, 0.0f, 1.0f}},
   {{0.0f, -1.0f, 1.0f}},
   {{1.0f, 0.0f, -1.0f}},
   {{0.0f, 1.0f, -1.0f}},
   {{-1.0f, 0.0f, -1.0f}},
   {{0.0f, -1.0f, -1.0f}},
   {{a, a, a}},
   {{-a, a, -a}},
   {{-a, -a, a}},
   {{a, -a, -a}},
   {{-a, a, a}},
   {{a, -a, a}},
   {{a, -a, -a}},
   {{-a, a, -a}}}};
std::array<std::array<float, 3>, 32> SimplexNoise::grad3v{
  {{{-a, a, a}},
   {{-a, -a, a}},
   {{a, -a, a}},
   {{a, a, a}},
   {{-a, -a, -a}},
   {{a, -a, -a}},
   {{a, a, -a}},
   {{-a, a, -a}},
   {{1.0f, -1.0f, 0.0f}},
   {{1.0f, 1.0f, 0.0f}},
   {{-1.0f, 1.0f, 0.0f}},
   {{-1.0f, -1.0f, 0.0f}},
   {{1.0f, 0.0f, 1.0f}},
   {{-1.0f, 0.0f, 1.0f}}, // 4 repeats to make 16
   {{0.0f, 1.0f, -1.0f}},
   {{0.0f, -1.0f, -1.0f}}}};

std::array<std::array<float, 4>, 64> SimplexNoise::sSimplexLut{
  {{{0, 1, 2, 3}}, {{0, 1, 3, 2}}, {{0, 0, 0, 0}}, {{0, 2, 3, 1}},
   {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{1, 2, 3, 0}},
   {{0, 2, 1, 3}}, {{0, 0, 0, 0}}, {{0, 3, 1, 2}}, {{0, 3, 2, 1}},
   {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{1, 3, 2, 0}},
   {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}},
   {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}},
   {{1, 2, 0, 3}}, {{0, 0, 0, 0}}, {{1, 3, 0, 2}}, {{0, 0, 0, 0}},
   {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{2, 3, 0, 1}}, {{2, 3, 1, 0}},
   {{1, 0, 2, 3}}, {{1, 0, 3, 2}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}},
   {{0, 0, 0, 0}}, {{2, 0, 3, 1}}, {{0, 0, 0, 0}}, {{2, 1, 3, 0}},
   {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}},
   {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}},
   {{2, 0, 1, 3}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}},
   {{3, 0, 1, 2}}, {{3, 0, 2, 1}}, {{0, 0, 0, 0}}, {{3, 1, 2, 0}},
   {{2, 1, 0, 3}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}},
   {{3, 1, 0, 2}}, {{0, 0, 0, 0}}, {{3, 2, 0, 1}}, {{3, 2, 1, 0}}}};

SimplexNoise::SimplexNoise()
{
  perm = {
    {151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,
     225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190,
     6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117,
     35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
     171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158,
     231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
     245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209,
     76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
     164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
     202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
     58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,
     154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
     19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
     228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
     145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184,
     84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
     222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156,
     180, 151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233,
     7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,
     190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203,
     117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125,
     136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146,
     158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,
     46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,
     209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159,
     86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123,
     5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,
     16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,
     44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,
     253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246,
     97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,
     51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157,
     184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205,
     93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,
     156, 180}};
}

SimplexNoise::~SimplexNoise()
{
}

// ---------------------------------------------------------------------------

float SimplexNoise::noise(float x)
{
  int i0   = fastfloor(x);
  int i1   = i0 + 1;
  float x0 = x - i0;
  float x1 = x0 - 1.0f;

  float n0, n1;

  float t0 = 1.0f - x0 * x0;
  //  if(t0 < 0.0f) t0 = 0.0f;
  t0 *= t0;
  n0 = t0 * t0 * grad(perm[i0 & 0xff], x0);

  float t1 = 1.0f - x1 * x1;
  //  if(t1 < 0.0f) t1 = 0.0f;
  t1 *= t1;
  n1 = t1 * t1 * grad(perm[i1 & 0xff], x1);
  // The maximum value of this noise is 8*(3/4)^4 = 2.53125
  // A factor of 0.395 would scale to fit exactly within [-1,1], but
  // we want to match PRMan's 1D noise, so we scale it down some more.
  return 0.25f * (n0 + n1);
}

// 2D simplex noise
float SimplexNoise::noise(const Vector2& v)
{
  float n0, n1, n2; // Noise contributions from the three corners

  // Skew the input space to determine which simplex cell we're in
  float s  = (v.x + v.y) * F2; // Hairy factor for 2D
  float xs = v.x + s;
  float ys = v.y + s;
  int i    = fastfloor(xs);
  int j    = fastfloor(ys);

  float t  = static_cast<float>(i + j) * G2;
  float X0 = i - t; // Unskew the cell origin back to (x,y) space
  float Y0 = j - t;
  float x0 = v.x - X0; // The x,y distances from the cell origin
  float y0 = v.y - Y0;

  // For the 2D case, the simplex shape is an equilateral triangle.
  // Determine which simplex we are in.
  unsigned int i1,
    j1; // Offsets for second (middle) corner of simplex in (i,j) coords
  if (x0 > y0) {
    i1 = 1;
    j1 = 0;
  } // lower triangle, XY order: (0,0)->(1,0)->(1,1)
  else {
    i1 = 0;
    j1 = 1;
  } // upper triangle, YX order: (0,0)->(0,1)->(1,1)

  // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
  // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
  // c = (3-sqrt(3))/6

  float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
  float y1 = y0 - j1 + G2;
  float x2
    = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
  float y2 = y0 - 1.0f + 2.0f * G2;

  // Wrap the integer indices at 256, to avoid indexing perm[] out of
  // bounds
  unsigned int ii = i & 0xff;
  unsigned int jj = j & 0xff;

  // Calculate the contribution from the three corners
  float t0 = 0.5f - x0 * x0 - y0 * y0;
  if (t0 < 0.0f)
    n0 = 0.0f;
  else {
    t0 *= t0;
    n0 = t0 * t0 * grad(perm[ii + perm[jj]], x0, y0);
  }

  float t1 = 0.5f - x1 * x1 - y1 * y1;
  if (t1 < 0.0f)
    n1 = 0.0f;
  else {
    t1 *= t1;
    n1 = t1 * t1 * grad(perm[ii + i1 + perm[jj + j1]], x1, y1);
  }

  float t2 = 0.5f - x2 * x2 - y2 * y2;
  if (t2 < 0.0f)
    n2 = 0.0f;
  else {
    t2 *= t2;
    n2 = t2 * t2 * grad(perm[ii + 1 + perm[jj + 1]], x2, y2);
  }

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  return 40.0f * (n0 + n1 + n2); // TODO: The scale factor is preliminary!
}

// 3D simplex noise
float SimplexNoise::noise(const Vector3& v)
{
  float n0, n1, n2, n3; // Noise contributions from the four corners

  // Skew the input space to determine which simplex cell we're in
  float s  = (v.x + v.y + v.z) * F3; // Very nice and simple skew factor for 3D
  float xs = v.x + s;
  float ys = v.y + s;
  float zs = v.z + s;
  int i    = fastfloor(xs);
  int j    = fastfloor(ys);
  int k    = fastfloor(zs);

  float t  = static_cast<float>(i + j + k) * G3;
  float X0 = i - t; // Unskew the cell origin back to (x,y,z) space
  float Y0 = j - t;
  float Z0 = k - t;
  float x0 = v.x - X0; // The x,y,z distances from the cell origin
  float y0 = v.y - Y0;
  float z0 = v.z - Z0;

  // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
  // Determine which simplex we are in.
  unsigned int i1, j1,
    k1; // Offsets for second corner of simplex in (i,j,k) coords
  unsigned int i2, j2,
    k2; // Offsets for third corner of simplex in (i,j,k) coords

  /* This code would benefit from a backport from the GLSL version! */
  if (x0 >= y0) {
    if (y0 >= z0) {
      i1 = 1;
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    } // X Y Z order
    else if (x0 >= z0) {
      i1 = 1;
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    } // X Z Y order
    else {
      i1 = 0;
      j1 = 0;
      k1 = 1;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    } // Z X Y order
  }
  else { // x0<y0
    if (y0 < z0) {
      i1 = 0;
      j1 = 0;
      k1 = 1;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    } // Z Y X order
    else if (x0 < z0) {
      i1 = 0;
      j1 = 1;
      k1 = 0;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    } // Y Z X order
    else {
      i1 = 0;
      j1 = 1;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    } // Y X Z order
  }

  // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
  // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
  // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
  // c = 1/6.

  float x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
  float y1 = y0 - j1 + G3;
  float z1 = z0 - k1 + G3;
  float x2 = x0 - i2 + 2.0f * G3; // Offsets for third corner in (x,y,z) coords
  float y2 = y0 - j2 + 2.0f * G3;
  float z2 = z0 - k2 + 2.0f * G3;
  float x3 = x0 - 1.0f + 3.0f * G3; // Offsets for last corner in (x,y,z) coords
  float y3 = y0 - 1.0f + 3.0f * G3;
  float z3 = z0 - 1.0f + 3.0f * G3;

  // Wrap the integer indices at 256, to avoid indexing perm[] out of
  // bounds
  unsigned int ii = i & 0xff;
  unsigned int jj = j & 0xff;
  unsigned int kk = k & 0xff;

  // Calculate the contribution from the four corners
  float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
  if (t0 < 0.0f)
    n0 = 0.0f;
  else {
    t0 *= t0;
    n0 = t0 * t0 * grad(perm[ii + perm[jj + perm[kk]]], x0, y0, z0);
  }

  float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
  if (t1 < 0.0f)
    n1 = 0.0f;
  else {
    t1 *= t1;
    n1 = t1 * t1
         * grad(perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]], x1, y1, z1);
  }

  float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
  if (t2 < 0.0f)
    n2 = 0.0f;
  else {
    t2 *= t2;
    n2 = t2 * t2
         * grad(perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]], x2, y2, z2);
  }

  float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
  if (t3 < 0.0f)
    n3 = 0.0f;
  else {
    t3 *= t3;
    n3 = t3 * t3 * grad(perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]], x3, y3, z3);
  }

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to stay just inside [-1,1]
  return 32.0f * (n0 + n1 + n2 + n3); // TODO: The scale factor is preliminary!
}

float SimplexNoise::noise(const Vector4& v)
{
  float n0, n1, n2, n3, n4; // Noise contributions from the five corners

  // Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
  float s  = (v.x + v.y + v.z + v.w) * F4; // Factor for 4D skewing
  float xs = v.x + s;
  float ys = v.y + s;
  float zs = v.z + s;
  float ws = v.w + s;
  int i    = fastfloor(xs);
  int j    = fastfloor(ys);
  int k    = fastfloor(zs);
  int l    = fastfloor(ws);

  float t  = (i + j + k + l) * G4; // Factor for 4D unskewing
  float X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
  float Y0 = j - t;
  float Z0 = k - t;
  float W0 = l - t;

  float x0 = v.x - X0; // The x,y,z,w distances from the cell origin
  float y0 = v.y - Y0;
  float z0 = v.z - Z0;
  float w0 = v.w - W0;

  // For the 4D case, the simplex is a 4D shape I won't even try to describe.
  // To find out which of the 24 possible simplices we're in, we need to
  // determine the magnitude ordering of x0, y0, z0 and w0.
  // The method below is a good way of finding the ordering of x,y,z,w and
  // then find the correct traversal order for the simplex weíre in.
  // First, six pair-wise comparisons are performed between each possible pair
  // of the four coordinates, and the results are used to add up binary bits
  // for an integer index.
  unsigned int c1 = (x0 > y0) ? 32 : 0;
  unsigned int c2 = (x0 > z0) ? 16 : 0;
  unsigned int c3 = (y0 > z0) ? 8 : 0;
  unsigned int c4 = (x0 > w0) ? 4 : 0;
  unsigned int c5 = (y0 > w0) ? 2 : 0;
  unsigned int c6 = (z0 > w0) ? 1 : 0;
  unsigned int c  = c1 + c2 + c3 + c4 + c5 + c6;

  unsigned int i1, j1, k1,
    l1; // The integer offsets for the second simplex corner
  unsigned int i2, j2, k2,
    l2; // The integer offsets for the third simplex corner
  unsigned int i3, j3, k3,
    l3; // The integer offsets for the fourth simplex corner

  // sSimplexLut[c] is a 4-vector with the numbers 0, 1, 2 and 3 in
  // some order.
  // Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and
  // x<w
  // impossible. Only the 24 indices which have non-zero entries make any sense.
  // We use a thresholding to set the coordinates in turn from the largest
  // magnitude.
  // The number 3 in the "simplex" array is at the position of the largest
  // coordinate.
  i1 = sSimplexLut[c][0] >= 3 ? 1 : 0;
  j1 = sSimplexLut[c][1] >= 3 ? 1 : 0;
  k1 = sSimplexLut[c][2] >= 3 ? 1 : 0;
  l1 = sSimplexLut[c][3] >= 3 ? 1 : 0;
  // The number 2 in the "simplex" array is at the second largest coordinate.
  i2 = sSimplexLut[c][0] >= 2 ? 1 : 0;
  j2 = sSimplexLut[c][1] >= 2 ? 1 : 0;
  k2 = sSimplexLut[c][2] >= 2 ? 1 : 0;
  l2 = sSimplexLut[c][3] >= 2 ? 1 : 0;
  // The number 1 in the "simplex" array is at the second smallest coordinate.
  i3 = sSimplexLut[c][0] >= 1 ? 1 : 0;
  j3 = sSimplexLut[c][1] >= 1 ? 1 : 0;
  k3 = sSimplexLut[c][2] >= 1 ? 1 : 0;
  l3 = sSimplexLut[c][3] >= 1 ? 1 : 0;
  // The fifth corner has all coordinate offsets = 1, so no need to look that
  // up.

  float x1 = x0 - i1 + G4; // Offsets for second corner in (x,y,z,w) coords
  float y1 = y0 - j1 + G4;
  float z1 = z0 - k1 + G4;
  float w1 = w0 - l1 + G4;
  float x2
    = x0 - i2 + 2.0f * G4; // Offsets for third corner in (x,y,z,w) coords
  float y2 = y0 - j2 + 2.0f * G4;
  float z2 = z0 - k2 + 2.0f * G4;
  float w2 = w0 - l2 + 2.0f * G4;
  float x3
    = x0 - i3 + 3.0f * G4; // Offsets for fourth corner in (x,y,z,w) coords
  float y3 = y0 - j3 + 3.0f * G4;
  float z3 = z0 - k3 + 3.0f * G4;
  float w3 = w0 - l3 + 3.0f * G4;
  float x4
    = x0 - 1.0f + 4.0f * G4; // Offsets for last corner in (x,y,z,w) coords
  float y4 = y0 - 1.0f + 4.0f * G4;
  float z4 = z0 - 1.0f + 4.0f * G4;
  float w4 = w0 - 1.0f + 4.0f * G4;

  // Wrap the integer indices at 256, to avoid indexing perm[] out of
  // bounds
  unsigned int ii = i & 0xff;
  unsigned int jj = j & 0xff;
  unsigned int kk = k & 0xff;
  unsigned int ll = l & 0xff;

  // Calculate the contribution from the five corners
  float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0 - w0 * w0;
  if (t0 < 0.0f)
    n0 = 0.0f;
  else {
    t0 *= t0;
    n0 = t0 * t0
         * grad(perm[ii + perm[jj + perm[kk + perm[ll]]]], x0, y0, z0, w0);
  }

  float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1 - w1 * w1;
  if (t1 < 0.0f)
    n1 = 0.0f;
  else {
    t1 *= t1;
    n1 = t1 * t1
         * grad(perm[ii + i1 + perm[jj + j1 + perm[kk + k1 + perm[ll + l1]]]],
                x1, y1, z1, w1);
  }

  float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2 - w2 * w2;
  if (t2 < 0.0f)
    n2 = 0.0f;
  else {
    t2 *= t2;
    n2 = t2 * t2
         * grad(perm[ii + i2 + perm[jj + j2 + perm[kk + k2 + perm[ll + l2]]]],
                x2, y2, z2, w2);
  }

  float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3 - w3 * w3;
  if (t3 < 0.0f)
    n3 = 0.0f;
  else {
    t3 *= t3;
    n3 = t3 * t3
         * grad(perm[ii + i3 + perm[jj + j3 + perm[kk + k3 + perm[ll + l3]]]],
                x3, y3, z3, w3);
  }

  float t4 = 0.6f - x4 * x4 - y4 * y4 - z4 * z4 - w4 * w4;
  if (t4 < 0.0f)
    n4 = 0.0f;
  else {
    t4 *= t4;
    n4 = t4 * t4
         * grad(perm[ii + 1 + perm[jj + 1 + perm[kk + 1 + perm[ll + 1]]]], x4,
                y4, z4, w4);
  }

  // Sum up and scale the result to cover the range [-1,1]
  return 27.0f
         * (n0 + n1 + n2 + n3 + n4); // TODO: The scale factor is preliminary!
}

// -----------------------------------------------------------------------------

float SimplexNoise::ridgedNoise(float x)
{
  return ridgedNoise_t(x);
}
float SimplexNoise::ridgedNoise(const Vector2& v)
{
  return ridgedNoise_t(v);
}
float SimplexNoise::ridgedNoise(const Vector3& v)
{
  return ridgedNoise_t(v);
}
float SimplexNoise::ridgedNoise(const Vector4& v)
{
  return ridgedNoise_t(v);
}

// -----------------------------------------------------------------------------

Vector2 SimplexNoise::dnoise(float x)
{
  int i0   = fastfloor(x);
  int i1   = i0 + 1;
  float x0 = x - i0;
  float x1 = x0 - 1.0f;

  float gx0, gx1;
  float n0, n1;
  float t20, t40, t21, t41;

  float x20 = x0 * x0;
  float t0  = 1.0f - x20;
  //  if(t0 < 0.0f) t0 = 0.0f; // Never happens for 1D: x0<=1 always
  t20 = t0 * t0;
  t40 = t20 * t20;
  grad1(perm[i0 & 0xff], &gx0);
  n0 = t40 * gx0 * x0;

  float x21 = x1 * x1;
  float t1  = 1.0f - x21;
  //  if(t1 < 0.0f) t1 = 0.0f; // Never happens for 1D: |x1|<=1 always
  t21 = t1 * t1;
  t41 = t21 * t21;
  grad1(perm[i1 & 0xff], &gx1);
  n1 = t41 * gx1 * x1;

  /* Compute derivative according to:
   *  *dnoise_dx = -8.0f * t20 * t0 * x0 * (gx0 * x0) + t40 * gx0;
   *  *dnoise_dx += -8.0f * t21 * t1 * x1 * (gx1 * x1) + t41 * gx1;
   */
  float dnoise_dx = t20 * t0 * gx0 * x20;
  dnoise_dx += t21 * t1 * gx1 * x21;
  dnoise_dx *= -8.0f;
  dnoise_dx += t40 * gx0 + t41 * gx1;
  dnoise_dx *= 0.25f; /* Scale derivative to match the noise scaling */

// The maximum value of this noise is 8*(3/4)^4 = 2.53125
// A factor of 0.395 would scale to fit exactly within [-1,1], but
// to better match classic Perlin noise, we scale it down some more.
#ifdef SIMPLEX_DERIVATIVES_RESCALE
  return Vector2(0.3961965135f * (n0 + n1), dnoise_dx);
#else
  return Vector2(0.25f * (n0 + n1), dnoise_dx);
#endif
}

Vector3 SimplexNoise::dnoise(const Vector2& v)
{
  float n0, n1, n2; // Noise contributions from the three corners

  // Skew the input space to determine which simplex cell we're in
  float s  = (v.x + v.y) * F2; // Hairy factor for 2D
  float xs = v.x + s;
  float ys = v.y + s;
  int i    = fastfloor(xs);
  int j    = fastfloor(ys);

  float t  = static_cast<float>(i + j) * G2;
  float X0 = i - t; // Unskew the cell origin back to (x,y) space
  float Y0 = j - t;
  float x0 = v.x - X0; // The x,y distances from the cell origin
  float y0 = v.y - Y0;

  // For the 2D case, the simplex shape is an equilateral triangle.
  // Determine which simplex we are in.
  unsigned int i1,
    j1; // Offsets for second (middle) corner of simplex in (i,j) coords
  if (x0 > y0) {
    i1 = 1;
    j1 = 0;
  } // lower triangle, XY order: (0,0)->(1,0)->(1,1)
  else {
    i1 = 0;
    j1 = 1;
  } // upper triangle, YX order: (0,0)->(0,1)->(1,1)

  // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
  // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
  // c = (3-sqrt(3))/6

  float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
  float y1 = y0 - j1 + G2;
  float x2
    = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
  float y2 = y0 - 1.0f + 2.0f * G2;

  // Wrap the integer indices at 256, to avoid indexing perm[] out of
  // bounds
  unsigned int ii = i & 0xff;
  unsigned int jj = j & 0xff;

  float gx0, gy0, gx1, gy1, gx2, gy2; /* Gradients at simplex corners */

  /* Calculate the contribution from the three corners */
  float t0 = 0.5f - x0 * x0 - y0 * y0;
  float t20, t40;
  if (t0 < 0.0f)
    t40 = t20 = t0 = n0 = gx0 = gy0 = 0.0f; /* No influence */
  else {
    grad2(perm[ii + perm[jj]], &gx0, &gy0);
    t20 = t0 * t0;
    t40 = t20 * t20;
    n0  = t40 * (gx0 * x0 + gy0 * y0);
  }

  float t1 = 0.5f - x1 * x1 - y1 * y1;
  float t21, t41;
  if (t1 < 0.0f)
    t21 = t41 = t1 = n1 = gx1 = gy1 = 0.0f; /* No influence */
  else {
    grad2(perm[ii + i1 + perm[jj + j1]], &gx1, &gy1);
    t21 = t1 * t1;
    t41 = t21 * t21;
    n1  = t41 * (gx1 * x1 + gy1 * y1);
  }

  float t2 = 0.5f - x2 * x2 - y2 * y2;
  float t22, t42;
  if (t2 < 0.0f)
    t42 = t22 = t2 = n2 = gx2 = gy2 = 0.0f; /* No influence */
  else {
    grad2(perm[ii + 1 + perm[jj + 1]], &gx2, &gy2);
    t22 = t2 * t2;
    t42 = t22 * t22;
    n2  = t42 * (gx2 * x2 + gy2 * y2);
  }

  /* Compute derivative, if requested by supplying non-null pointers
   * for the last two arguments */
  /*  A straight, unoptimised calculation would be like:
   *    *dnoise_dx = -8.0f * t20 * t0 * x0 * ( gx0 * x0 + gy0 * y0 ) + t40 *
   * gx0;
   *    *dnoise_dy = -8.0f * t20 * t0 * y0 * ( gx0 * x0 + gy0 * y0 ) + t40 *
   * gy0;
   *    *dnoise_dx += -8.0f * t21 * t1 * x1 * ( gx1 * x1 + gy1 * y1 ) + t41 *
   * gx1;
   *    *dnoise_dy += -8.0f * t21 * t1 * y1 * ( gx1 * x1 + gy1 * y1 ) + t41 *
   * gy1;
   *    *dnoise_dx += -8.0f * t22 * t2 * x2 * ( gx2 * x2 + gy2 * y2 ) + t42 *
   * gx2;
   *    *dnoise_dy += -8.0f * t22 * t2 * y2 * ( gx2 * x2 + gy2 * y2 ) + t42 *
   * gy2;
   */
  float temp0     = t20 * t0 * (gx0 * x0 + gy0 * y0);
  float dnoise_dx = temp0 * x0;
  float dnoise_dy = temp0 * y0;
  float temp1     = t21 * t1 * (gx1 * x1 + gy1 * y1);
  dnoise_dx += temp1 * x1;
  dnoise_dy += temp1 * y1;
  float temp2 = t22 * t2 * (gx2 * x2 + gy2 * y2);
  dnoise_dx += temp2 * x2;
  dnoise_dy += temp2 * y2;
  dnoise_dx *= -8.0f;
  dnoise_dy *= -8.0f;
  dnoise_dx += t40 * gx0 + t41 * gx1 + t42 * gx2;
  dnoise_dy += t40 * gy0 + t41 * gy1 + t42 * gy2;
  dnoise_dx *= 40.0f; /* Scale derivative to match the noise scaling */
  dnoise_dy *= 40.0f;

// Add contributions from each corner to get the final noise value.
// The result is scaled to return values in the interval [-1,1].
#ifdef SIMPLEX_DERIVATIVES_RESCALE
  return Vector3(70.175438596f * (n0 + n1 + n2), dnoise_dx,
                 dnoise_dy); // TODO: The scale factor is preliminary!
#else
  return Vector3(40.0f * (n0 + n1 + n2), dnoise_dx,
                 dnoise_dy); // TODO: The scale factor is preliminary!
#endif
}

Vector4 SimplexNoise::dnoise(const Vector3& v)
{
  float n0, n1, n2, n3; /* Noise contributions from the four simplex corners */
  float noise;          /* Return value */
  float gx0, gy0, gz0, gx1, gy1, gz1; /* Gradients at simplex corners */
  float gx2, gy2, gz2, gx3, gy3, gz3;

  /* Skew the input space to determine which simplex cell we're in */
  float s
    = (v.x + v.y + v.z) * F3; /* Very nice and simple skew factor for 3D */
  float xs = v.x + s;
  float ys = v.y + s;
  float zs = v.z + s;
  int i    = fastfloor(xs);
  int j    = fastfloor(ys);
  int k    = fastfloor(zs);

  float t  = static_cast<float>(i + j + k) * G3;
  float X0 = i - t; /* Unskew the cell origin back to (x,y,z) space */
  float Y0 = j - t;
  float Z0 = k - t;
  float x0 = v.x - X0; /* The x,y,z distances from the cell origin */
  float y0 = v.y - Y0;
  float z0 = v.z - Z0;

  /* For the 3D case, the simplex shape is a slightly irregular tetrahedron.
   * Determine which simplex we are in. */
  unsigned int i1, j1,
    k1; /* Offsets for second corner of simplex in (i,j,k) coords */
  unsigned int i2, j2,
    k2; /* Offsets for third corner of simplex in (i,j,k) coords */

  /* TODO: This code would benefit from a backport from the GLSL version! */
  if (x0 >= y0) {
    if (y0 >= z0) {
      i1 = 1;
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    } /* X Y Z order */
    else if (x0 >= z0) {
      i1 = 1;
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    } /* X Z Y order */
    else {
      i1 = 0;
      j1 = 0;
      k1 = 1;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    } /* Z X Y order */
  }
  else { // x0<y0
    if (y0 < z0) {
      i1 = 0;
      j1 = 0;
      k1 = 1;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    } /* Z Y X order */
    else if (x0 < z0) {
      i1 = 0;
      j1 = 1;
      k1 = 0;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    } /* Y Z X order */
    else {
      i1 = 0;
      j1 = 1;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    } /* Y X Z order */
  }

  /* A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
   * a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
   * a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
   * c = 1/6.   */

  float x1 = x0 - i1 + G3; /* Offsets for second corner in (x,y,z) coords */
  float y1 = y0 - j1 + G3;
  float z1 = z0 - k1 + G3;
  float x2
    = x0 - i2 + 2.0f * G3; /* Offsets for third corner in (x,y,z) coords */
  float y2 = y0 - j2 + 2.0f * G3;
  float z2 = z0 - k2 + 2.0f * G3;
  float x3
    = x0 - 1.0f + 3.0f * G3; /* Offsets for last corner in (x,y,z) coords */
  float y3 = y0 - 1.0f + 3.0f * G3;
  float z3 = z0 - 1.0f + 3.0f * G3;

  /* Wrap the integer indices at 256, to avoid indexing perm[] out of
   * bounds */
  unsigned int ii = i & 0xff;
  unsigned int jj = j & 0xff;
  unsigned int kk = k & 0xff;

  /* Calculate the contribution from the four corners */
  float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
  float t20, t40;
  if (t0 < 0.0f)
    n0 = t0 = t20 = t40 = gx0 = gy0 = gz0 = 0.0f;
  else {
    grad3(perm[ii + perm[jj + perm[kk]]], &gx0, &gy0, &gz0);
    t20 = t0 * t0;
    t40 = t20 * t20;
    n0  = t40 * (gx0 * x0 + gy0 * y0 + gz0 * z0);
  }

  float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
  float t21, t41;
  if (t1 < 0.0f)
    n1 = t1 = t21 = t41 = gx1 = gy1 = gz1 = 0.0f;
  else {
    grad3(perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]], &gx1, &gy1, &gz1);
    t21 = t1 * t1;
    t41 = t21 * t21;
    n1  = t41 * (gx1 * x1 + gy1 * y1 + gz1 * z1);
  }

  float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
  float t22, t42;
  if (t2 < 0.0f)
    n2 = t2 = t22 = t42 = gx2 = gy2 = gz2 = 0.0f;
  else {
    grad3(perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]], &gx2, &gy2, &gz2);
    t22 = t2 * t2;
    t42 = t22 * t22;
    n2  = t42 * (gx2 * x2 + gy2 * y2 + gz2 * z2);
  }

  float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
  float t23, t43;
  if (t3 < 0.0f)
    n3 = t3 = t23 = t43 = gx3 = gy3 = gz3 = 0.0f;
  else {
    grad3(perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]], &gx3, &gy3, &gz3);
    t23 = t3 * t3;
    t43 = t23 * t23;
    n3  = t43 * (gx3 * x3 + gy3 * y3 + gz3 * z3);
  }

/*  Add contributions from each corner to get the final noise value.
 * The result is scaled to return values in the range [-1,1] */
#ifdef SIMPLEX_DERIVATIVES_RESCALE
  noise = 34.525277436f * (n0 + n1 + n2 + n3);
#else
  noise = 28.0f * (n0 + n1 + n2 + n3);
#endif

  /* Compute derivative, if requested by supplying non-null pointers
   * for the last three arguments */
  /*  A straight, unoptimised calculation would be like:
   *     *dnoise_dx = -8.0f * t20 * t0 * x0 * dot(gx0, gy0, gz0, x0, y0, z0) +
   * t40 * gx0;
   *    *dnoise_dy = -8.0f * t20 * t0 * y0 * dot(gx0, gy0, gz0, x0, y0, z0) +
   * t40 * gy0;
   *    *dnoise_dz = -8.0f * t20 * t0 * z0 * dot(gx0, gy0, gz0, x0, y0, z0) +
   * t40 * gz0;
   *    *dnoise_dx += -8.0f * t21 * t1 * x1 * dot(gx1, gy1, gz1, x1, y1, z1) +
   * t41 * gx1;
   *    *dnoise_dy += -8.0f * t21 * t1 * y1 * dot(gx1, gy1, gz1, x1, y1, z1) +
   * t41 * gy1;
   *    *dnoise_dz += -8.0f * t21 * t1 * z1 * dot(gx1, gy1, gz1, x1, y1, z1) +
   * t41 * gz1;
   *    *dnoise_dx += -8.0f * t22 * t2 * x2 * dot(gx2, gy2, gz2, x2, y2, z2) +
   * t42 * gx2;
   *    *dnoise_dy += -8.0f * t22 * t2 * y2 * dot(gx2, gy2, gz2, x2, y2, z2) +
   * t42 * gy2;
   *    *dnoise_dz += -8.0f * t22 * t2 * z2 * dot(gx2, gy2, gz2, x2, y2, z2) +
   * t42 * gz2;
   *    *dnoise_dx += -8.0f * t23 * t3 * x3 * dot(gx3, gy3, gz3, x3, y3, z3) +
   * t43 * gx3;
   *    *dnoise_dy += -8.0f * t23 * t3 * y3 * dot(gx3, gy3, gz3, x3, y3, z3) +
   * t43 * gy3;
   *    *dnoise_dz += -8.0f * t23 * t3 * z3 * dot(gx3, gy3, gz3, x3, y3, z3) +
   * t43 * gz3;
   */
  float temp0     = t20 * t0 * (gx0 * x0 + gy0 * y0 + gz0 * z0);
  float dnoise_dx = temp0 * x0;
  float dnoise_dy = temp0 * y0;
  float dnoise_dz = temp0 * z0;
  float temp1     = t21 * t1 * (gx1 * x1 + gy1 * y1 + gz1 * z1);
  dnoise_dx += temp1 * x1;
  dnoise_dy += temp1 * y1;
  dnoise_dz += temp1 * z1;
  float temp2 = t22 * t2 * (gx2 * x2 + gy2 * y2 + gz2 * z2);
  dnoise_dx += temp2 * x2;
  dnoise_dy += temp2 * y2;
  dnoise_dz += temp2 * z2;
  float temp3 = t23 * t3 * (gx3 * x3 + gy3 * y3 + gz3 * z3);
  dnoise_dx += temp3 * x3;
  dnoise_dy += temp3 * y3;
  dnoise_dz += temp3 * z3;
  dnoise_dx *= -8.0f;
  dnoise_dy *= -8.0f;
  dnoise_dz *= -8.0f;
  dnoise_dx += t40 * gx0 + t41 * gx1 + t42 * gx2 + t43 * gx3;
  dnoise_dy += t40 * gy0 + t41 * gy1 + t42 * gy2 + t43 * gy3;
  dnoise_dz += t40 * gz0 + t41 * gz1 + t42 * gz2 + t43 * gz3;
  dnoise_dx *= 28.0f; /* Scale derivative to match the noise scaling */
  dnoise_dy *= 28.0f;
  dnoise_dz *= 28.0f;

  return Vector4(noise, dnoise_dx, dnoise_dy, dnoise_dz);
}

SimplexNoise::vec5 SimplexNoise::dnoise(const Vector4& v)
{
  float n0, n1, n2, n3, n4; // Noise contributions from the five corners
  float noise;              // Return value
  float gx0, gy0, gz0, gw0, gx1, gy1, gz1,
    gw1; /* Gradients at simplex corners */
  float gx2, gy2, gz2, gw2, gx3, gy3, gz3, gw3, gx4, gy4, gz4, gw4;
  float t20, t21, t22, t23, t24;
  float t40, t41, t42, t43, t44;

  // Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
  float s  = (v.x + v.y + v.z + v.w) * F4; // Factor for 4D skewing
  float xs = v.x + s;
  float ys = v.y + s;
  float zs = v.z + s;
  float ws = v.w + s;
  int i    = fastfloor(xs);
  int j    = fastfloor(ys);
  int k    = fastfloor(zs);
  int l    = fastfloor(ws);

  float t  = (i + j + k + l) * G4; // Factor for 4D unskewing
  float X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
  float Y0 = j - t;
  float Z0 = k - t;
  float W0 = l - t;

  float x0 = v.x - X0; // The x,y,z,w distances from the cell origin
  float y0 = v.y - Y0;
  float z0 = v.z - Z0;
  float w0 = v.w - W0;

  // For the 4D case, the simplex is a 4D shape I won't even try to describe.
  // To find out which of the 24 possible simplices we're in, we need to
  // determine the magnitude ordering of x0, y0, z0 and w0.
  // The method below is a reasonable way of finding the ordering of x,y,z,w
  // and then find the correct traversal order for the simplex weíre in.
  // First, six pair-wise comparisons are performed between each possible pair
  // of the four coordinates, and then the results are used to add up binary
  // bits for an integer index into a precomputed lookup table, sSimplexLut[].
  unsigned int c1 = (x0 > y0) ? 32 : 0;
  unsigned int c2 = (x0 > z0) ? 16 : 0;
  unsigned int c3 = (y0 > z0) ? 8 : 0;
  unsigned int c4 = (x0 > w0) ? 4 : 0;
  unsigned int c5 = (y0 > w0) ? 2 : 0;
  unsigned int c6 = (z0 > w0) ? 1 : 0;
  unsigned int c = c1 | c2 | c3 | c4 | c5 | c6; // '|' is mostly faster than '+'

  unsigned int i1, j1, k1,
    l1; // The integer offsets for the second simplex corner
  unsigned int i2, j2, k2,
    l2; // The integer offsets for the third simplex corner
  unsigned int i3, j3, k3,
    l3; // The integer offsets for the fourth simplex corner

  // sSimplexLut[c] is a 4-vector with the numbers 0, 1, 2 and 3 in
  // some order. Many values of c will never occur, since e.g. x>y>z>w makes
  // x<z, y<w and x<w impossible. Only the 24 indices which have non-zero
  // entries make any sense. We use a thresholding to set the coordinates in
  // turn from the largest magnitude. The number 3 in the "simplex" array is at
  // the position of the largest coordinate.
  i1 = sSimplexLut[c][0] >= 3 ? 1 : 0;
  j1 = sSimplexLut[c][1] >= 3 ? 1 : 0;
  k1 = sSimplexLut[c][2] >= 3 ? 1 : 0;
  l1 = sSimplexLut[c][3] >= 3 ? 1 : 0;
  // The number 2 in the "simplex" array is at the second largest coordinate.
  i2 = sSimplexLut[c][0] >= 2 ? 1 : 0;
  j2 = sSimplexLut[c][1] >= 2 ? 1 : 0;
  k2 = sSimplexLut[c][2] >= 2 ? 1 : 0;
  l2 = sSimplexLut[c][3] >= 2 ? 1 : 0;
  // The number 1 in the "simplex" array is at the second smallest coordinate.
  i3 = sSimplexLut[c][0] >= 1 ? 1 : 0;
  j3 = sSimplexLut[c][1] >= 1 ? 1 : 0;
  k3 = sSimplexLut[c][2] >= 1 ? 1 : 0;
  l3 = sSimplexLut[c][3] >= 1 ? 1 : 0;
  // The fifth corner has all coordinate offsets = 1, so no need to look that
  // up.

  float x1 = x0 - i1 + G4; // Offsets for second corner in (x,y,z,w) coords
  float y1 = y0 - j1 + G4;
  float z1 = z0 - k1 + G4;
  float w1 = w0 - l1 + G4;
  float x2
    = x0 - i2 + 2.0f * G4; // Offsets for third corner in (x,y,z,w) coords
  float y2 = y0 - j2 + 2.0f * G4;
  float z2 = z0 - k2 + 2.0f * G4;
  float w2 = w0 - l2 + 2.0f * G4;
  float x3
    = x0 - i3 + 3.0f * G4; // Offsets for fourth corner in (x,y,z,w) coords
  float y3 = y0 - j3 + 3.0f * G4;
  float z3 = z0 - k3 + 3.0f * G4;
  float w3 = w0 - l3 + 3.0f * G4;
  float x4
    = x0 - 1.0f + 4.0f * G4; // Offsets for last corner in (x,y,z,w) coords
  float y4 = y0 - 1.0f + 4.0f * G4;
  float z4 = z0 - 1.0f + 4.0f * G4;
  float w4 = w0 - 1.0f + 4.0f * G4;

  // Wrap the integer indices at 256, to avoid indexing perm[] out of
  // bounds
  unsigned int ii = i & 0xff;
  unsigned int jj = j & 0xff;
  unsigned int kk = k & 0xff;
  unsigned int ll = l & 0xff;

  // Calculate the contribution from the five corners
  float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0 - w0 * w0;
  if (t0 < 0.0f)
    n0 = t0 = t20 = t40 = gx0 = gy0 = gz0 = gw0 = 0.0f;
  else {
    t20 = t0 * t0;
    t40 = t20 * t20;
    grad4(perm[ii + perm[jj + perm[kk + perm[ll]]]], &gx0, &gy0, &gz0, &gw0);
    n0 = t40 * (gx0 * x0 + gy0 * y0 + gz0 * z0 + gw0 * w0);
  }

  float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1 - w1 * w1;
  if (t1 < 0.0f)
    n1 = t1 = t21 = t41 = gx1 = gy1 = gz1 = gw1 = 0.0f;
  else {
    t21 = t1 * t1;
    t41 = t21 * t21;
    grad4(perm[ii + i1 + perm[jj + j1 + perm[kk + k1 + perm[ll + l1]]]], &gx1,
          &gy1, &gz1, &gw1);
    n1 = t41 * (gx1 * x1 + gy1 * y1 + gz1 * z1 + gw1 * w1);
  }

  float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2 - w2 * w2;
  if (t2 < 0.0f)
    n2 = t2 = t22 = t42 = gx2 = gy2 = gz2 = gw2 = 0.0f;
  else {
    t22 = t2 * t2;
    t42 = t22 * t22;
    grad4(perm[ii + i2 + perm[jj + j2 + perm[kk + k2 + perm[ll + l2]]]], &gx2,
          &gy2, &gz2, &gw2);
    n2 = t42 * (gx2 * x2 + gy2 * y2 + gz2 * z2 + gw2 * w2);
  }

  float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3 - w3 * w3;
  if (t3 < 0.0f)
    n3 = t3 = t23 = t43 = gx3 = gy3 = gz3 = gw3 = 0.0f;
  else {
    t23 = t3 * t3;
    t43 = t23 * t23;
    grad4(perm[ii + i3 + perm[jj + j3 + perm[kk + k3 + perm[ll + l3]]]], &gx3,
          &gy3, &gz3, &gw3);
    n3 = t43 * (gx3 * x3 + gy3 * y3 + gz3 * z3 + gw3 * w3);
  }

  float t4 = 0.6f - x4 * x4 - y4 * y4 - z4 * z4 - w4 * w4;
  if (t4 < 0.0f)
    n4 = t4 = t24 = t44 = gx4 = gy4 = gz4 = gw4 = 0.0f;
  else {
    t24 = t4 * t4;
    t44 = t24 * t24;
    grad4(perm[ii + 1 + perm[jj + 1 + perm[kk + 1 + perm[ll + 1]]]], &gx4, &gy4,
          &gz4, &gw4);
    n4 = t44 * (gx4 * x4 + gy4 * y4 + gz4 * z4 + gw4 * w4);
  }

  // Sum up and scale the result to cover the range [-1,1]
  noise = 27.0f
          * (n0 + n1 + n2 + n3 + n4); // TODO: The scale factor is preliminary!

  /* Compute derivative, if requested by supplying non-null pointers
   * for the last four arguments */
  /*  A straight, unoptimised calculation would be like:
   *     *dnoise_dx = -8.0f * t20 * t0 * x0 * dot(gx0, gy0, gz0, gw0, x0, y0,
   * z0, w0) + t40 * gx0;
   *    *dnoise_dy = -8.0f * t20 * t0 * y0 * dot(gx0, gy0, gz0, gw0, x0, y0, z0,
   * w0) + t40 * gy0;
   *    *dnoise_dz = -8.0f * t20 * t0 * z0 * dot(gx0, gy0, gz0, gw0, x0, y0, z0,
   * w0) + t40 * gz0;
   *    *dnoise_dw = -8.0f * t20 * t0 * w0 * dot(gx0, gy0, gz0, gw0, x0, y0, z0,
   * w0) + t40 * gw0;
   *    *dnoise_dx += -8.0f * t21 * t1 * x1 * dot(gx1, gy1, gz1, gw1, x1, y1,
   * z1, w1) + t41 * gx1;
   *    *dnoise_dy += -8.0f * t21 * t1 * y1 * dot(gx1, gy1, gz1, gw1, x1, y1,
   * z1, w1) + t41 * gy1;
   *    *dnoise_dz += -8.0f * t21 * t1 * z1 * dot(gx1, gy1, gz1, gw1, x1, y1,
   * z1, w1) + t41 * gz1;
   *    *dnoise_dw = -8.0f * t21 * t1 * w1 * dot(gx1, gy1, gz1, gw1, x1, y1, z1,
   * w1) + t41 * gw1;
   *    *dnoise_dx += -8.0f * t22 * t2 * x2 * dot(gx2, gy2, gz2, gw2, x2, y2,
   * z2, w2) + t42 * gx2;
   *    *dnoise_dy += -8.0f * t22 * t2 * y2 * dot(gx2, gy2, gz2, gw2, x2, y2,
   * z2, w2) + t42 * gy2;
   *    *dnoise_dz += -8.0f * t22 * t2 * z2 * dot(gx2, gy2, gz2, gw2, x2, y2,
   * z2, w2) + t42 * gz2;
   *    *dnoise_dw += -8.0f * t22 * t2 * w2 * dot(gx2, gy2, gz2, gw2, x2, y2,
   * z2, w2) + t42 * gw2;
   *    *dnoise_dx += -8.0f * t23 * t3 * x3 * dot(gx3, gy3, gz3, gw3, x3, y3,
   * z3, w3) + t43 * gx3;
   *    *dnoise_dy += -8.0f * t23 * t3 * y3 * dot(gx3, gy3, gz3, gw3, x3, y3,
   * z3, w3) + t43 * gy3;
   *    *dnoise_dz += -8.0f * t23 * t3 * z3 * dot(gx3, gy3, gz3, gw3, x3, y3,
   * z3, w3) + t43 * gz3;
   *    *dnoise_dw += -8.0f * t23 * t3 * w3 * dot(gx3, gy3, gz3, gw3, x3, y3,
   * z3, w3) + t43 * gw3;
   *    *dnoise_dx += -8.0f * t24 * t4 * x4 * dot(gx4, gy4, gz4, gw4, x4, y4,
   * z4, w4) + t44 * gx4;
   *    *dnoise_dy += -8.0f * t24 * t4 * y4 * dot(gx4, gy4, gz4, gw4, x4, y4,
   * z4, w4) + t44 * gy4;
   *    *dnoise_dz += -8.0f * t24 * t4 * z4 * dot(gx4, gy4, gz4, gw4, x4, y4,
   * z4, w4) + t44 * gz4;
   *    *dnoise_dw += -8.0f * t24 * t4 * w4 * dot(gx4, gy4, gz4, gw4, x4, y4,
   * z4, w4) + t44 * gw4;
   */
  float temp0     = t20 * t0 * (gx0 * x0 + gy0 * y0 + gz0 * z0 + gw0 * w0);
  float dnoise_dx = temp0 * x0;
  float dnoise_dy = temp0 * y0;
  float dnoise_dz = temp0 * z0;
  float dnoise_dw = temp0 * w0;
  float temp1     = t21 * t1 * (gx1 * x1 + gy1 * y1 + gz1 * z1 + gw1 * w1);
  dnoise_dx += temp1 * x1;
  dnoise_dy += temp1 * y1;
  dnoise_dz += temp1 * z1;
  dnoise_dw += temp1 * w1;
  float temp2 = t22 * t2 * (gx2 * x2 + gy2 * y2 + gz2 * z2 + gw2 * w2);
  dnoise_dx += temp2 * x2;
  dnoise_dy += temp2 * y2;
  dnoise_dz += temp2 * z2;
  dnoise_dw += temp2 * w2;
  float temp3 = t23 * t3 * (gx3 * x3 + gy3 * y3 + gz3 * z3 + gw3 * w3);
  dnoise_dx += temp3 * x3;
  dnoise_dy += temp3 * y3;
  dnoise_dz += temp3 * z3;
  dnoise_dw += temp3 * w3;
  float temp4 = t24 * t4 * (gx4 * x4 + gy4 * y4 + gz4 * z4 + gw4 * w4);
  dnoise_dx += temp4 * x4;
  dnoise_dy += temp4 * y4;
  dnoise_dz += temp4 * z4;
  dnoise_dw += temp4 * w4;
  dnoise_dx *= -8.0f;
  dnoise_dy *= -8.0f;
  dnoise_dz *= -8.0f;
  dnoise_dw *= -8.0f;
  dnoise_dx += t40 * gx0 + t41 * gx1 + t42 * gx2 + t43 * gx3 + t44 * gx4;
  dnoise_dy += t40 * gy0 + t41 * gy1 + t42 * gy2 + t43 * gy3 + t44 * gy4;
  dnoise_dz += t40 * gz0 + t41 * gz1 + t42 * gz2 + t43 * gz3 + t44 * gz4;
  dnoise_dw += t40 * gw0 + t41 * gw1 + t42 * gw2 + t43 * gw3 + t44 * gw4;

  dnoise_dx *= 28.0f; /* Scale derivative to match the noise scaling */
  dnoise_dy *= 28.0f;
  dnoise_dz *= 28.0f;
  dnoise_dw *= 28.0f;

  return {{noise, dnoise_dx, dnoise_dy, dnoise_dz, dnoise_dw}};
}

// -----------------------------------------------------------------------------

float SimplexNoise::flowNoise(const Vector2& v, float angle)
{
  float n0, n1, n2; /* Noise contributions from the three simplex corners */
  float gx0, gy0, gx1, gy1, gx2, gy2; /* Gradients at simplex corners */
  float sin_t, cos_t; /* Sine and cosine for the gradient rotation angle */
  sin_t = std::sin(angle);
  cos_t = std::cos(angle);

  /* Skew the input space to determine which simplex cell we're in */
  float s  = (v.x + v.y) * F2; /* Hairy factor for 2D */
  float xs = v.x + s;
  float ys = v.y + s;
  int i    = fastfloor(xs);
  int j    = fastfloor(ys);

  float t  = static_cast<float>(i + j) * G2;
  float X0 = i - t; /* Unskew the cell origin back to (x,y) space */
  float Y0 = j - t;
  float x0 = v.x - X0; /* The x,y distances from the cell origin */
  float y0 = v.y - Y0;

  /* For the 2D case, the simplex shape is an equilateral triangle.
   * Determine which simplex we are in. */
  unsigned int i1,
    j1; /* Offsets for second (middle) corner of simplex in (i,j) coords */
  if (x0 > y0) {
    i1 = 1;
    j1 = 0;
  } /* lower triangle, XY order: (0,0)->(1,0)->(1,1) */
  else {
    i1 = 0;
    j1 = 1;
  } /* upper triangle, YX order: (0,0)->(0,1)->(1,1) */

  /* A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
   * a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
   * c = (3-sqrt(3))/6   */
  float x1
    = x0 - i1 + G2; /* Offsets for middle corner in (x,y) unskewed coords */
  float y1 = y0 - j1 + G2;
  float x2 = x0 - 1.0f
             + 2.0f * G2; /* Offsets for last corner in (x,y) unskewed coords */
  float y2 = y0 - 1.0f + 2.0f * G2;

  /* Wrap the integer indices at 256, to avoid indexing perm[] out of
   * bounds */
  unsigned int ii = i & 0xff;
  unsigned int jj = j & 0xff;

  /* Calculate the contribution from the three corners */
  float t0 = 0.5f - x0 * x0 - y0 * y0;
  float t20, t40;
  if (t0 < 0.0f)
    t40 = t20 = t0 = n0 = gx0 = gy0 = 0.0f; /* No influence */
  else {
    gradrot2(perm[ii + perm[jj]], sin_t, cos_t, &gx0, &gy0);
    t20 = t0 * t0;
    t40 = t20 * t20;
    n0  = t40 * graddotp2(gx0, gy0, x0, y0);
  }

  float t1 = 0.5f - x1 * x1 - y1 * y1;
  float t21, t41;
  if (t1 < 0.0f)
    t21 = t41 = t1 = n1 = gx1 = gy1 = 0.0f; /* No influence */
  else {
    gradrot2(perm[ii + i1 + perm[jj + j1]], sin_t, cos_t, &gx1, &gy1);
    t21 = t1 * t1;
    t41 = t21 * t21;
    n1  = t41 * graddotp2(gx1, gy1, x1, y1);
  }

  float t2 = 0.5f - x2 * x2 - y2 * y2;
  float t22, t42;
  if (t2 < 0.0f)
    t42 = t22 = t2 = n2 = gx2 = gy2 = 0.0f; /* No influence */
  else {
    gradrot2(perm[ii + 1 + perm[jj + 1]], sin_t, cos_t, &gx2, &gy2);
    t22 = t2 * t2;
    t42 = t22 * t22;
    n2  = t42 * graddotp2(gx2, gy2, x2, y2);
  }

  /* Add contributions from each corner to get the final noise value.
   * The result is scaled to return values in the interval [-1,1]. */
  return 40.0f * (n0 + n1 + n2);
}
float SimplexNoise::flowNoise(const Vector3& v, float angle)
{
  float n0, n1, n2, n3; /* Noise contributions from the four simplex corners */
  float gx0, gy0, gz0, gx1, gy1, gz1; /* Gradients at simplex corners */
  float gx2, gy2, gz2, gx3, gy3, gz3;
  float sin_t, cos_t; /* Sine and cosine for the gradient rotation angle */
  sin_t = std::sin(angle);
  cos_t = std::cos(angle);

  /* Skew the input space to determine which simplex cell we're in */
  float s
    = (v.x + v.y + v.z) * F3; /* Very nice and simple skew factor for 3D */
  float xs = v.x + s;
  float ys = v.y + s;
  float zs = v.z + s;
  int i    = fastfloor(xs);
  int j    = fastfloor(ys);
  int k    = fastfloor(zs);

  float t  = static_cast<float>(i + j + k) * G3;
  float X0 = i - t; /* Unskew the cell origin back to (x,y,z) space */
  float Y0 = j - t;
  float Z0 = k - t;
  float x0 = v.x - X0; /* The x,y,z distances from the cell origin */
  float y0 = v.y - Y0;
  float z0 = v.z - Z0;

  /* For the 3D case, the simplex shape is a slightly irregular tetrahedron.
   * Determine which simplex we are in. */
  unsigned int i1, j1,
    k1; /* Offsets for second corner of simplex in (i,j,k) coords */
  unsigned int i2, j2,
    k2; /* Offsets for third corner of simplex in (i,j,k) coords */

  /* TODO: This code would benefit from a backport from the GLSL version! */
  if (x0 >= y0) {
    if (y0 >= z0) {
      i1 = 1;
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    } /* X Y Z order */
    else if (x0 >= z0) {
      i1 = 1;
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    } /* X Z Y order */
    else {
      i1 = 0;
      j1 = 0;
      k1 = 1;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    } /* Z X Y order */
  }
  else { // x0<y0
    if (y0 < z0) {
      i1 = 0;
      j1 = 0;
      k1 = 1;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    } /* Z Y X order */
    else if (x0 < z0) {
      i1 = 0;
      j1 = 1;
      k1 = 0;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    } /* Y Z X order */
    else {
      i1 = 0;
      j1 = 1;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    } /* Y X Z order */
  }

  /* A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
   * a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
   * a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
   * c = 1/6.   */

  float x1 = x0 - i1 + G3; /* Offsets for second corner in (x,y,z) coords */
  float y1 = y0 - j1 + G3;
  float z1 = z0 - k1 + G3;
  float x2
    = x0 - i2 + 2.0f * G3; /* Offsets for third corner in (x,y,z) coords */
  float y2 = y0 - j2 + 2.0f * G3;
  float z2 = z0 - k2 + 2.0f * G3;
  float x3
    = x0 - 1.0f + 3.0f * G3; /* Offsets for last corner in (x,y,z) coords */
  float y3 = y0 - 1.0f + 3.0f * G3;
  float z3 = z0 - 1.0f + 3.0f * G3;

  /* Wrap the integer indices at 256, to avoid indexing perm[] out of
   * bounds */
  unsigned int ii = i & 0xff;
  unsigned int jj = j & 0xff;
  unsigned int kk = k & 0xff;

  /* Calculate the contribution from the four corners */
  float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
  float t20, t40;
  if (t0 < 0.0f)
    n0 = t0 = t20 = t40 = gx0 = gy0 = gz0 = 0.0f;
  else {
    gradrot3(perm[ii + perm[jj + perm[kk]]], sin_t, cos_t, &gx0, &gy0, &gz0);
    t20 = t0 * t0;
    t40 = t20 * t20;
    n0  = t40 * graddotp3(gx0, gy0, gz0, x0, y0, z0);
  }

  float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
  float t21, t41;
  if (t1 < 0.0f)
    n1 = t1 = t21 = t41 = gx1 = gy1 = gz1 = 0.0f;
  else {
    gradrot3(perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]], sin_t, cos_t, &gx1,
             &gy1, &gz1);
    t21 = t1 * t1;
    t41 = t21 * t21;
    n1  = t41 * graddotp3(gx1, gy1, gz1, x1, y1, z1);
  }

  float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
  float t22, t42;
  if (t2 < 0.0f)
    n2 = t2 = t22 = t42 = gx2 = gy2 = gz2 = 0.0f;
  else {
    gradrot3(perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]], sin_t, cos_t, &gx2,
             &gy2, &gz2);
    t22 = t2 * t2;
    t42 = t22 * t22;
    n2  = t42 * graddotp3(gx2, gy2, gz2, x2, y2, z2);
  }

  float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
  float t23, t43;
  if (t3 < 0.0f)
    n3 = t3 = t23 = t43 = gx3 = gy3 = gz3 = 0.0f;
  else {
    gradrot3(perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]], sin_t, cos_t, &gx3,
             &gy3, &gz3);
    t23 = t3 * t3;
    t43 = t23 * t23;
    n3  = t43 * graddotp3(gx3, gy3, gz3, x3, y3, z3);
  }

  /*  Add contributions from each corner to get the final noise value.
   * The result is scaled to return values in the range [-1,1] */
  return 28.0f * (n0 + n1 + n2 + n3);
}

// -----------------------------------------------------------------------------

Vector3 SimplexNoise::dFlowNoise(const Vector2& v, float angle)
{

  float n0, n1, n2; /* Noise contributions from the three simplex corners */
  float gx0, gy0, gx1, gy1, gx2, gy2; /* Gradients at simplex corners */
  float sin_t, cos_t; /* Sine and cosine for the gradient rotation angle */
  sin_t = std::sin(angle);
  cos_t = std::cos(angle);

  /* Skew the input space to determine which simplex cell we're in */
  float s  = (v.x + v.y) * F2; /* Hairy factor for 2D */
  float xs = v.x + s;
  float ys = v.y + s;
  int i    = fastfloor(xs);
  int j    = fastfloor(ys);

  float t  = static_cast<float>(i + j) * G2;
  float X0 = i - t; /* Unskew the cell origin back to (x,y) space */
  float Y0 = j - t;
  float x0 = v.x - X0; /* The x,y distances from the cell origin */
  float y0 = v.y - Y0;

  /* For the 2D case, the simplex shape is an equilateral triangle.
   * Determine which simplex we are in. */
  unsigned int i1,
    j1; /* Offsets for second (middle) corner of simplex in (i,j) coords */
  if (x0 > y0) {
    i1 = 1;
    j1 = 0;
  } /* lower triangle, XY order: (0,0)->(1,0)->(1,1) */
  else {
    i1 = 0;
    j1 = 1;
  } /* upper triangle, YX order: (0,0)->(0,1)->(1,1) */

  /* A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
   * a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
   * c = (3-sqrt(3))/6   */
  float x1
    = x0 - i1 + G2; /* Offsets for middle corner in (x,y) unskewed coords */
  float y1 = y0 - j1 + G2;
  float x2 = x0 - 1.0f
             + 2.0f * G2; /* Offsets for last corner in (x,y) unskewed coords */
  float y2 = y0 - 1.0f + 2.0f * G2;

  /* Wrap the integer indices at 256, to avoid indexing perm[] out of
   * bounds */
  unsigned int ii = i & 0xff;
  unsigned int jj = j & 0xff;

  /* Calculate the contribution from the three corners */
  float t0 = 0.5f - x0 * x0 - y0 * y0;
  float t20, t40;
  if (t0 < 0.0f)
    t40 = t20 = t0 = n0 = gx0 = gy0 = 0.0f; /* No influence */
  else {
    gradrot2(perm[ii + perm[jj]], sin_t, cos_t, &gx0, &gy0);
    t20 = t0 * t0;
    t40 = t20 * t20;
    n0  = t40 * graddotp2(gx0, gy0, x0, y0);
  }

  float t1 = 0.5f - x1 * x1 - y1 * y1;
  float t21, t41;
  if (t1 < 0.0f)
    t21 = t41 = t1 = n1 = gx1 = gy1 = 0.0f; /* No influence */
  else {
    gradrot2(perm[ii + i1 + perm[jj + j1]], sin_t, cos_t, &gx1, &gy1);
    t21 = t1 * t1;
    t41 = t21 * t21;
    n1  = t41 * graddotp2(gx1, gy1, x1, y1);
  }

  float t2 = 0.5f - x2 * x2 - y2 * y2;
  float t22, t42;
  if (t2 < 0.0f)
    t42 = t22 = t2 = n2 = gx2 = gy2 = 0.0f; /* No influence */
  else {
    gradrot2(perm[ii + 1 + perm[jj + 1]], sin_t, cos_t, &gx2, &gy2);
    t22 = t2 * t2;
    t42 = t22 * t22;
    n2  = t42 * graddotp2(gx2, gy2, x2, y2);
  }

  /* Add contributions from each corner to get the final noise value.
   * The result is scaled to return values in the interval [-1,1]. */
  float noise = 40.0f * (n0 + n1 + n2);

  /* Compute derivative, if requested by supplying non-null pointers
   * for the last two arguments */
  float dnoise_dx, dnoise_dy;

  /*  A straight, unoptimised calculation would be like:
   *    *dnoise_dx = -8.0f * t20 * t0 * x0 * graddotp2(gx0, gy0, x0,
   * y0) + t40 * gx0;
   *    *dnoise_dy = -8.0f * t20 * t0 * y0 * graddotp2(gx0, gy0, x0,
   * y0) + t40 * gy0;
   *    *dnoise_dx += -8.0f * t21 * t1 * x1 * graddotp2(gx1, gy1, x1,
   * y1) + t41 * gx1;
   *    *dnoise_dy += -8.0f * t21 * t1 * y1 * graddotp2(gx1, gy1, x1,
   * y1) + t41 * gy1;
   *    *dnoise_dx += -8.0f * t22 * t2 * x2 * graddotp2(gx2, gy2, x2,
   * y2) + t42 * gx2;
   *    *dnoise_dy += -8.0f * t22 * t2 * y2 * graddotp2(gx2, gy2, x2,
   * y2) + t42 * gy2;
   */
  float temp0 = t20 * t0 * graddotp2(gx0, gy0, x0, y0);
  dnoise_dx   = temp0 * x0;
  dnoise_dy   = temp0 * y0;
  float temp1 = t21 * t1 * graddotp2(gx1, gy1, x1, y1);
  dnoise_dx += temp1 * x1;
  dnoise_dy += temp1 * y1;
  float temp2 = t22 * t2 * graddotp2(gx2, gy2, x2, y2);
  dnoise_dx += temp2 * x2;
  dnoise_dy += temp2 * y2;
  dnoise_dx *= -8.0f;
  dnoise_dy *= -8.0f;
  /* This corrects a bug in the original implementation */
  dnoise_dx += t40 * gx0 + t41 * gx1 + t42 * gx2;
  dnoise_dy += t40 * gy0 + t41 * gy1 + t42 * gy2;
  dnoise_dx *= 40.0f; /* Scale derivative to match the noise scaling */
  dnoise_dy *= 40.0f;

  return Vector3(noise, dnoise_dx, dnoise_dy);
}

Vector4 SimplexNoise::dFlowNoise(const Vector3& v, float angle)
{
  float n0, n1, n2, n3; /* Noise contributions from the four simplex corners */
  float noise;          /* Return value */
  float gx0, gy0, gz0, gx1, gy1, gz1; /* Gradients at simplex corners */
  float gx2, gy2, gz2, gx3, gy3, gz3;
  float sin_t, cos_t; /* Sine and cosine for the gradient rotation angle */
  sin_t = std::sin(angle);
  cos_t = std::cos(angle);

  /* Skew the input space to determine which simplex cell we're in */
  float s
    = (v.x + v.y + v.z) * F3; /* Very nice and simple skew factor for 3D */
  float xs = v.x + s;
  float ys = v.y + s;
  float zs = v.z + s;
  int i    = fastfloor(xs);
  int j    = fastfloor(ys);
  int k    = fastfloor(zs);

  float t  = static_cast<float>(i + j + k) * G3;
  float X0 = i - t; /* Unskew the cell origin back to (x,y,z) space */
  float Y0 = j - t;
  float Z0 = k - t;
  float x0 = v.x - X0; /* The x,y,z distances from the cell origin */
  float y0 = v.y - Y0;
  float z0 = v.z - Z0;

  /* For the 3D case, the simplex shape is a slightly irregular tetrahedron.
   * Determine which simplex we are in. */
  unsigned int i1, j1,
    k1; /* Offsets for second corner of simplex in (i,j,k) coords */
  unsigned int i2, j2,
    k2; /* Offsets for third corner of simplex in (i,j,k) coords */

  /* TODO: This code would benefit from a backport from the GLSL version! */
  if (x0 >= y0) {
    if (y0 >= z0) {
      i1 = 1;
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    } /* X Y Z order */
    else if (x0 >= z0) {
      i1 = 1;
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    } /* X Z Y order */
    else {
      i1 = 0;
      j1 = 0;
      k1 = 1;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    } /* Z X Y order */
  }
  else { // x0<y0
    if (y0 < z0) {
      i1 = 0;
      j1 = 0;
      k1 = 1;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    } /* Z Y X order */
    else if (x0 < z0) {
      i1 = 0;
      j1 = 1;
      k1 = 0;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    } /* Y Z X order */
    else {
      i1 = 0;
      j1 = 1;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    } /* Y X Z order */
  }

  /* A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
   * a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
   * a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
   * c = 1/6.   */

  float x1 = x0 - i1 + G3; /* Offsets for second corner in (x,y,z) coords */
  float y1 = y0 - j1 + G3;
  float z1 = z0 - k1 + G3;
  float x2
    = x0 - i2 + 2.0f * G3; /* Offsets for third corner in (x,y,z) coords */
  float y2 = y0 - j2 + 2.0f * G3;
  float z2 = z0 - k2 + 2.0f * G3;
  float x3
    = x0 - 1.0f + 3.0f * G3; /* Offsets for last corner in (x,y,z) coords */
  float y3 = y0 - 1.0f + 3.0f * G3;
  float z3 = z0 - 1.0f + 3.0f * G3;

  /* Wrap the integer indices at 256, to avoid indexing perm[] out of
   * bounds */
  unsigned int ii = i & 0xff;
  unsigned int jj = j & 0xff;
  unsigned int kk = k & 0xff;

  /* Calculate the contribution from the four corners */
  float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
  float t20, t40;
  if (t0 < 0.0f)
    n0 = t0 = t20 = t40 = gx0 = gy0 = gz0 = 0.0f;
  else {
    gradrot3(perm[ii + perm[jj + perm[kk]]], sin_t, cos_t, &gx0, &gy0, &gz0);
    t20 = t0 * t0;
    t40 = t20 * t20;
    n0  = t40 * graddotp3(gx0, gy0, gz0, x0, y0, z0);
  }

  float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
  float t21, t41;
  if (t1 < 0.0f)
    n1 = t1 = t21 = t41 = gx1 = gy1 = gz1 = 0.0f;
  else {
    gradrot3(perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]], sin_t, cos_t, &gx1,
             &gy1, &gz1);
    t21 = t1 * t1;
    t41 = t21 * t21;
    n1  = t41 * graddotp3(gx1, gy1, gz1, x1, y1, z1);
  }

  float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
  float t22, t42;
  if (t2 < 0.0f)
    n2 = t2 = t22 = t42 = gx2 = gy2 = gz2 = 0.0f;
  else {
    gradrot3(perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]], sin_t, cos_t, &gx2,
             &gy2, &gz2);
    t22 = t2 * t2;
    t42 = t22 * t22;
    n2  = t42 * graddotp3(gx2, gy2, gz2, x2, y2, z2);
  }

  float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
  float t23, t43;
  if (t3 < 0.0f)
    n3 = t3 = t23 = t43 = gx3 = gy3 = gz3 = 0.0f;
  else {
    gradrot3(perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]], sin_t, cos_t, &gx3,
             &gy3, &gz3);
    t23 = t3 * t3;
    t43 = t23 * t23;
    n3  = t43 * graddotp3(gx3, gy3, gz3, x3, y3, z3);
  }

  /*  Add contributions from each corner to get the final noise value.
   * The result is scaled to return values in the range [-1,1] */
  noise = 28.0f * (n0 + n1 + n2 + n3);

  /* Compute derivative, if requested by supplying non-null pointers
   * for the last three arguments */
  float dnoise_dx, dnoise_dy, dnoise_dz;

  /*  A straight, unoptimised calculation would be like:
   *     *dnoise_dx = -8.0f * t20 * t0 * x0 * graddotp3(gx0, gy0, gz0,
   * x0, y0, z0) + t40 * gx0;
   *    *dnoise_dy = -8.0f * t20 * t0 * y0 * graddotp3(gx0, gy0, gz0,
   * x0, y0, z0) + t40 * gy0;
   *    *dnoise_dz = -8.0f * t20 * t0 * z0 * graddotp3(gx0, gy0, gz0,
   * x0, y0, z0) + t40 * gz0;
   *    *dnoise_dx += -8.0f * t21 * t1 * x1 * graddotp3(gx1, gy1, gz1,
   * x1, y1, z1) + t41 * gx1;
   *    *dnoise_dy += -8.0f * t21 * t1 * y1 * graddotp3(gx1, gy1, gz1,
   * x1, y1, z1) + t41 * gy1;
   *    *dnoise_dz += -8.0f * t21 * t1 * z1 * graddotp3(gx1, gy1, gz1,
   * x1, y1, z1) + t41 * gz1;
   *    *dnoise_dx += -8.0f * t22 * t2 * x2 * graddotp3(gx2, gy2, gz2,
   * x2, y2, z2) + t42 * gx2;
   *    *dnoise_dy += -8.0f * t22 * t2 * y2 * graddotp3(gx2, gy2, gz2,
   * x2, y2, z2) + t42 * gy2;
   *    *dnoise_dz += -8.0f * t22 * t2 * z2 * graddotp3(gx2, gy2, gz2,
   * x2, y2, z2) + t42 * gz2;
   *    *dnoise_dx += -8.0f * t23 * t3 * x3 * graddotp3(gx3, gy3, gz3,
   * x3, y3, z3) + t43 * gx3;
   *    *dnoise_dy += -8.0f * t23 * t3 * y3 * graddotp3(gx3, gy3, gz3,
   * x3, y3, z3) + t43 * gy3;
   *    *dnoise_dz += -8.0f * t23 * t3 * z3 * graddotp3(gx3, gy3, gz3,
   * x3, y3, z3) + t43 * gz3;
   */
  float temp0 = t20 * t0 * graddotp3(gx0, gy0, gz0, x0, y0, z0);
  dnoise_dx   = temp0 * x0;
  dnoise_dy   = temp0 * y0;
  dnoise_dz   = temp0 * z0;
  float temp1 = t21 * t1 * graddotp3(gx1, gy1, gz1, x1, y1, z1);
  dnoise_dx += temp1 * x1;
  dnoise_dy += temp1 * y1;
  dnoise_dz += temp1 * z1;
  float temp2 = t22 * t2 * graddotp3(gx2, gy2, gz2, x2, y2, z2);
  dnoise_dx += temp2 * x2;
  dnoise_dy += temp2 * y2;
  dnoise_dz += temp2 * z2;
  float temp3 = t23 * t3 * graddotp3(gx3, gy3, gz3, x3, y3, z3);
  dnoise_dx += temp3 * x3;
  dnoise_dy += temp3 * y3;
  dnoise_dz += temp3 * z3;
  dnoise_dx *= -8.0f;
  dnoise_dy *= -8.0f;
  dnoise_dz *= -8.0f;
  /* This corrects a bug in the original implementation */
  dnoise_dx += t40 * gx0 + t41 * gx1 + t42 * gx2 + t43 * gx3;
  dnoise_dy += t40 * gy0 + t41 * gy1 + t42 * gy2 + t43 * gy3;
  dnoise_dz += t40 * gz0 + t41 * gz1 + t42 * gz2 + t43 * gz3;
  dnoise_dx *= 28.0f; /* Scale derivative to match the noise scaling */
  dnoise_dy *= 28.0f;
  dnoise_dz *= 28.0f;

  return Vector4(noise, dnoise_dx, dnoise_dy, dnoise_dz);
}

// -----------------------------------------------------------------------------

Vector2 SimplexNoise::curlNoise(const Vector2& v)
{
  const Vector3 derivative = dnoise(v);
  return Vector2(derivative.z, -derivative.y);
}

Vector2 SimplexNoise::curlNoise(const Vector2& v, float t)
{
  const Vector3 derivative = dFlowNoise(v, t);
  return Vector2(derivative.z, -derivative.y);
}

Vector2 SimplexNoise::curlNoise(const Vector2& v, uint8_t octaves,
                                float lacunarity, float gain)
{
  const Vector3 derivative = dfBm(v, octaves, lacunarity, gain);
  return Vector2(derivative.z, -derivative.y);
}

Vector3 SimplexNoise::curlNoise(const Vector3& v)
{
  const Vector4 derivX = dnoise(v);
  const Vector4 derivY = dnoise(v + Vector3(123.456f, 789.012f, 345.678f));
  const Vector4 derivZ = dnoise(v + Vector3(901.234f, 567.891f, 234.567f));
  return Vector3(derivZ.z - derivY.w, derivX.w - derivZ.y, derivY.y - derivX.z);
}

Vector3 SimplexNoise::curlNoise(const Vector3& v, float t)
{
  const Vector4 derivX = dFlowNoise(v, t);
  const Vector4 derivY
    = dFlowNoise(v + Vector3(123.456f, 789.012f, 345.678f), t);
  const Vector4 derivZ
    = dFlowNoise(v + Vector3(901.234f, 567.891f, 234.567f), t);
  return Vector3(derivZ.z - derivY.w, derivX.w - derivZ.y, derivY.y - derivX.z);
}

Vector3 SimplexNoise::curlNoise(const Vector3& v, uint8_t octaves,
                                float lacunarity, float gain)
{
  const Vector4 derivX = dfBm(v, octaves, lacunarity, gain);
  const Vector4 derivY = dfBm(v + Vector3(123.456f, 789.012f, 345.678f),
                              octaves, lacunarity, gain);
  const Vector4 derivZ = dfBm(v + Vector3(901.234f, 567.891f, 234.567f),
                              octaves, lacunarity, gain);
  return Vector3(derivZ.z - derivY.w, derivX.w - derivZ.y, derivY.y - derivX.z);
}

// -----------------------------------------------------------------------------

Vector2
SimplexNoise::curl(const Vector2& v,
                   const std::function<float(const Vector2&)>& potential,
                   float delta)
{
  const Vector2 deltaX = Vector2(delta, 0.0f);
  const Vector2 deltaY = Vector2(0.0f, delta);
  return Vector2(-(potential(v + deltaY) - potential(v - deltaY)),
                 (potential(v + deltaX) - potential(v - deltaX)))
         / (2.0f * delta);
}

Vector3
SimplexNoise::curl(const Vector3& v,
                   const std::function<Vector3(const Vector3&)>& potential,
                   float delta)
{
  const Vector3 deltaX = Vector3(delta, 0.0f, 0.0f);
  const Vector3 deltaY = Vector3(0.0f, delta, 0.0f);
  const Vector3 deltaZ = Vector3(0.0f, 0.0f, delta);
  return Vector3(((potential(v + deltaY).z - potential(v - deltaY).z)
                  - (potential(v + deltaZ).y - potential(v - deltaZ).y)),
                 ((potential(v + deltaZ).x - potential(v - deltaZ).x)
                  - (potential(v + deltaX).z - potential(v - deltaX).z)),
                 ((potential(v + deltaX).y - potential(v - deltaX).y)
                  - (potential(v + deltaY).x - potential(v - deltaY).x)))
         / (2.0f * delta);
}

// -----------------------------------------------------------------------------

float SimplexNoise::fBm(float x, uint8_t octaves, float lacunarity, float gain)
{
  return fBm_t(x, octaves, lacunarity, gain);
}

float SimplexNoise::fBm(const Vector2& v, uint8_t octaves, float lacunarity,
                        float gain)
{
  return fBm_t(v, octaves, lacunarity, gain);
}

float SimplexNoise::fBm(const Vector3& v, uint8_t octaves, float lacunarity,
                        float gain)
{
  return fBm_t(v, octaves, lacunarity, gain);
}

float SimplexNoise::fBm(const Vector4& v, uint8_t octaves, float lacunarity,
                        float gain)
{
  return fBm_t(v, octaves, lacunarity, gain);
}

// -----------------------------------------------------------------------------

Vector2 SimplexNoise::dfBm(float x, uint8_t octaves, float lacunarity,
                           float gain)
{
  Vector2 sum = Vector2(0.0f);
  float freq  = 1.0f;
  float amp   = 0.5f;

  for (uint8_t i = 0; i < octaves; i++) {
    Vector2 n = dnoise(x * freq);
    sum += n * amp;
    freq *= lacunarity;
    amp *= gain;
  }

  return sum;
}
Vector3 SimplexNoise::dfBm(const Vector2& v, uint8_t octaves, float lacunarity,
                           float gain)
{
  Vector3 sum = Vector3(0.0f);
  float freq  = 1.0f;
  float amp   = 0.5f;

  for (uint8_t i = 0; i < octaves; i++) {
    Vector3 n = dnoise(v * freq);
    sum += n * amp;
    freq *= lacunarity;
    amp *= gain;
  }

  return sum;
}
Vector4 SimplexNoise::dfBm(const Vector3& v, uint8_t octaves, float lacunarity,
                           float gain)
{
  Vector4 sum = Vector4(0.0f);
  float freq  = 1.0f;
  float amp   = 0.5f;

  for (uint8_t i = 0; i < octaves; i++) {
    Vector4 n = dnoise(v * freq);
    sum += n * amp;
    freq *= lacunarity;
    amp *= gain;
  }

  return sum;
}

SimplexNoise::vec5 SimplexNoise::dfBm(const Vector4& v, uint8_t octaves,
                                      float lacunarity, float gain)
{
  vec5 sum   = {{0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
  float freq = 1.0f;
  float amp  = 0.5f;

  for (uint8_t i = 0; i < octaves; i++) {
    vec5 n = dnoise(v * freq);
    sum[0] += n[0] * amp;
    sum[1] += n[1] * amp;
    sum[2] += n[2] * amp;
    sum[3] += n[3] * amp;
    sum[4] += n[4] * amp;
    freq *= lacunarity;
    amp *= gain;
  }

  return sum;
}

// -----------------------------------------------------------------------------

float SimplexNoise::ridgedMF(float x, float ridgeOffset, uint8_t octaves,
                             float lacunarity, float gain)
{
  return ridgedMF_t(x, ridgeOffset, octaves, lacunarity, gain);
}

float SimplexNoise::ridgedMF(const Vector2& v, float ridgeOffset,
                             uint8_t octaves, float lacunarity, float gain)
{
  return ridgedMF_t(v, ridgeOffset, octaves, lacunarity, gain);
}

float SimplexNoise::ridgedMF(const Vector3& v, float ridgeOffset,
                             uint8_t octaves, float lacunarity, float gain)
{
  return ridgedMF_t(v, ridgeOffset, octaves, lacunarity, gain);
}

float SimplexNoise::ridgedMF(const Vector4& v, float ridgeOffset,
                             uint8_t octaves, float lacunarity, float gain)
{
  return ridgedMF_t(v, ridgeOffset, octaves, lacunarity, gain);
}

// -----------------------------------------------------------------------------

float SimplexNoise::iqfBm(const Vector2& v, uint8_t octaves, float lacunarity,
                          float gain)
{
  float sum  = 0.0f;
  float amp  = 0.5f;
  float dx   = 0.0f;
  float dy   = 0.0f;
  float freq = 1.0f;
  for (uint8_t i = 0; i < octaves; i++) {
    Vector3 d = dnoise(v * freq);
    dx += d.y;
    dy += d.y;
    sum += amp * d.x / (1.f + dx * dx + dy * dy);
    freq *= lacunarity;
    amp *= gain;
  }

  return sum;
}
float SimplexNoise::iqfBm(const Vector3& v, uint8_t octaves, float lacunarity,
                          float gain)
{
  float sum  = 0.0;
  float amp  = 0.5;
  float dx   = 0.0;
  float dy   = 0.0;
  float dz   = 0.0;
  float freq = 1.0;
  for (uint8_t i = 0; i < octaves; i++) {
    Vector4 d = dnoise(v * freq);
    dx += d.y;
    dy += d.y;
    dz += d.z;
    sum += amp * d.x / (1.f + dx * dx + dy * dy + d.z * d.z);
    freq *= lacunarity;
    amp *= gain;
  }

  return sum;
}

// -----------------------------------------------------------------------------

void SimplexNoise::seed(uint32_t s)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  gen.seed(s);
  std::uniform_int_distribution<> distribution(1, 255);
  for (size_t i = 0; i < 256; ++i) {
    perm[i] = perm[i + 256] = distribution(gen);
  }
}

// -----------------------------------------------------------------------------

float SimplexNoise::grad(int hash, float x) const
{
  int h      = hash & 15;
  float grad = 1.0f + (h & 7); // Gradient value 1.0, 2.0, ..., 8.0
  if (h & 8)
    grad = -grad;    // Set a random sign for the gradient
  return (grad * x); // Multiply the gradient with the distance
}

float SimplexNoise::grad(int hash, float x, float y) const
{
  int h   = hash & 7;      // Convert low 3 bits of hash code
  float u = h < 4 ? x : y; // into 8 simple gradient directions,
  float v = h < 4 ? y : x; // and compute the dot product with (x,y).
  return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

float SimplexNoise::grad(int hash, float x, float y, float z) const
{
  int h   = hash & 15;     // Convert low 4 bits of hash code into 12 simple
  float u = h < 8 ? x : y; // gradient directions, and compute dot product.
  float v
    = h < 4 ? y : h == 12 || h == 14 ? x : z; // Fix repeats at h = 12 to 15
  return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

float SimplexNoise::grad(int hash, float x, float y, float z, float t) const
{
  int h   = hash & 31;      // Convert low 5 bits of hash code into 32 simple
  float u = h < 24 ? x : y; // gradient directions, and compute dot product.
  float v = h < 16 ? y : z;
  float w = h < 8 ? z : t;
  return ((h & 1) ? -u : u) + ((h & 2) ? -v : v) + ((h & 4) ? -w : w);
}

void SimplexNoise::grad1(int hash, float* gx)
{
  int h = hash & 15;
  *gx   = 1.0f + (h & 7); // Gradient value is one of 1.0, 2.0, ..., 8.0
  if (h & 8) {
    *gx = -*gx; // Make half of the gradients negative
  }
}

void SimplexNoise::grad2(int hash, float* gx, float* gy)
{
  unsigned int h = hash & 7;
  *gx            = grad2lut[h][0];
  *gy            = grad2lut[h][1];
  return;
}

void SimplexNoise::grad3(int hash, float* gx, float* gy, float* gz)
{
  unsigned int h = hash & 15;
  *gx            = grad3lut[h][0];
  *gy            = grad3lut[h][1];
  *gz            = grad3lut[h][2];
  return;
}

void SimplexNoise::grad4(int hash, float* gx, float* gy, float* gz, float* gw)
{
  unsigned int h = hash & 31;
  *gx            = grad4lut[h][0];
  *gy            = grad4lut[h][1];
  *gz            = grad4lut[h][2];
  *gw            = grad4lut[h][3];
  return;
}

void SimplexNoise::gradrot2(int hash, float sin_t, float cos_t, float* gx,
                            float* gy)
{
  unsigned int h = hash & 7;
  float gx0      = grad2lut[h][0];
  float gy0      = grad2lut[h][1];
  *gx            = cos_t * gx0 - sin_t * gy0;
  *gy            = sin_t * gx0 + cos_t * gy0;
  return;
}

void SimplexNoise::gradrot3(int hash, float sin_t, float cos_t, float* gx,
                            float* gy, float* gz)
{
  unsigned int h = hash & 15;
  float gux      = grad3u[h][0];
  float guy      = grad3u[h][1];
  float guz      = grad3u[h][2];
  float gvx      = grad3v[h][0];
  float gvy      = grad3v[h][1];
  float gvz      = grad3v[h][2];
  *gx            = cos_t * gux + sin_t * gvx;
  *gy            = cos_t * guy + sin_t * gvy;
  *gz            = cos_t * guz + sin_t * gvz;
  return;
}

float SimplexNoise::graddotp2(float gx, float gy, float x, float y) const
{
  return gx * x + gy * y;
}

float SimplexNoise::graddotp3(float gx, float gy, float gz, float x, float y,
                              float z) const
{
  return gx * x + gy * y + gz * z;
}

} // end of namespace Extensions
} // end of namespace BABYLON
