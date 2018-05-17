/*
 * Copyright (c) 2005-2007 David HENRY
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __MD5MODEL_H__
#define __MD5MODEL_H__

#include "quat.h"
#include "vec.h"

/* Joint */
struct md5_joint_t
{
  char name[64];
  int parent;

  vec3_t pos;
  quat4_t orient;
};

/* Vertex */
struct md5_vertex_t
{
  vec2_t st;

  int start; /* start weight */
  int count; /* weight count */
};

/* Triangle */
struct md5_triangle_t
{
  int index[3];
};

/* Weight */
struct md5_weight_t
{
  int joint;
  float bias;

  vec3_t pos;
};

/* Bounding box */
struct md5_bbox_t
{
  vec3_t min;
  vec3_t max;
};

/* MD5 mesh */
struct md5_mesh_t
{
  struct md5_vertex_t *vertices;
  struct md5_triangle_t *triangles;
  struct md5_weight_t *weights;

  int num_verts;
  int num_tris;
  int num_weights;

  char shader[256];
};

/* MD5 model structure */
struct md5_model_t
{
  struct md5_joint_t *baseSkel;
  struct md5_mesh_t *meshes;

  int num_joints;
  int num_meshes;
};

/* Animation data */
struct md5_anim_t
{
  int num_frames;
  int num_joints;
  int frameRate;

  struct md5_joint_t **skelFrames;
  struct md5_bbox_t *bboxes;
};

/* Animation info */
struct anim_info_t
{
  int curr_frame;
  int next_frame;

  double last_time;
  double max_time;
};

vec3_t *MD5GetVertexArray();
vec2_t *MD5GetTexCoordArray();
unsigned int *MD5GetIndicesArray();

/**
 * md5mesh prototypes
 */
int MD5ModelRead (const char *filename, struct md5_model_t *mdl);
void MD5ModelFree (struct md5_model_t *mdl);
void MD5MeshPrepare (const struct md5_mesh_t *mesh,
		  const struct md5_joint_t *skeleton);
void MD5ArraysAlloc ();
void MD5ArraysFree ();
//void MD5DrawSkeleton (const struct md5_joint_t *skeleton, int num_joints);

/**
 * md5anim prototypes
 */
int MD5AnimCheckValidity (const struct md5_model_t *mdl,
		       const struct md5_anim_t *anim);
int MD5AnimRead (const char *filename, struct md5_anim_t *anim);
void MD5AnimFree (struct md5_anim_t *anim);
void MD5SkeletonsInterpolate (const struct md5_joint_t *skelA,
			   const struct md5_joint_t *skelB,
			   int num_joints, float interp,
			   struct md5_joint_t *out);
void MD5Animate (const struct md5_anim_t *anim,
	      struct anim_info_t *animInfo, double dt);

#endif /* __MD5MODEL_H__ */
