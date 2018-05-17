#include "vec.h"

struct point_t
{
    union
    {
     vec3_t vec;
     struct
     {
       float x;
       float y;
       float z;    
     };
     };
};

struct bezier_curve_t
{
	vec3_t a;
	vec3_t b;
	vec3_t c;
	vec3_t d;
};

float Bezierf(float A,  // Start value
              float B,  // First control value
              float C,  // Second control value
              float D,  // Ending value
              float t)  // Parameter 0 <= t <= 1
{
    float s = 1.0f - t;
    float AB = A*s + B*t;
    float BC = B*s + C*t;
    float CD = C*s + D*t;
    float ABC = AB*s + CD*t;
    float BCD = BC*s + CD*t;
    return ABC*s + BCD*t;
}

void BezierCurveCalcPoint(struct bezier_curve_t* curve, float t, struct point_t* output)
{
	output->x = Bezierf(curve->a[X],curve->b[X],curve->c[X],curve->d[X],t);
	output->y = Bezierf(curve->a[Y],curve->b[Y],curve->c[Y],curve->d[Y],t);
	output->z = Bezierf(curve->a[Z],curve->b[Z],curve->c[Z],curve->d[Z],t);
}

