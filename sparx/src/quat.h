#ifndef __QUAT_H__
#define __QUAT_H__

#include "vec.h"

/* Quaternion (x, y, z, w) */
typedef float quat4_t[4];

/**
 * Quaternion prototypes
 */
void QuatComputeW (quat4_t q);
void QuatNormalize (quat4_t q);
void QuatMultQuat (const quat4_t qa, const quat4_t qb, quat4_t out);
void QuatMultVec (const quat4_t q, const vec3_t v, quat4_t out);
void QuatRotatePoint (const quat4_t q, const vec3_t in, vec3_t out);
float QuatDotProduct (const quat4_t qa, const quat4_t qb);
void QuatSlerp (const quat4_t qa, const quat4_t qb, float t, quat4_t out);

#endif
