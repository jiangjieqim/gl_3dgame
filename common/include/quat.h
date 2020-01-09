#ifndef _QUAT_H_
#define _QUAT_H_
#include "vmath.h"

//四元数数学库
/* Quaternion (x, y, z, w) */
typedef float Quat4_t[4];
/*
	四元数操作接口
*/

/*
*	将四元数转化为矩阵
*/
DLLIMPORT void 
Quat_to_matrrix(const Quat4_t qa,Matrix44f out);

DLLIMPORT void Quat_computeW (Quat4_t q);
DLLIMPORT void Quat_normalize (Quat4_t q);
DLLIMPORT void Quat_multQuat (const Quat4_t qa, const Quat4_t qb, Quat4_t out);
DLLIMPORT void Quat_multVec (const Quat4_t q, const Vec3_t v, Quat4_t out);
DLLIMPORT void Quat_rotatePoint (const Quat4_t q, const Vec3_t in, Vec3_t out);
/*点乘*/
DLLIMPORT float Quat_dotProduct (const Quat4_t qa, const Quat4_t qb);

DLLIMPORT void  Quat_slerp (const Quat4_t qa, const Quat4_t qb, float t, Quat4_t out);


/*
	四元数球面插值操作
	不支持角位移为反向向量,如(1,0,0)到(-1,0,0)球面插值
	输出向量	float* px,float* py,float* pz
*/
DLLIMPORT void quat_slerp_split(	float x0,float y0,float z0,
					float x1,float y1,float z1,
					float* px,float* py,float* pz, 
					float v);
#endif