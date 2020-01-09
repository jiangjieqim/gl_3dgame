#include <assert.h>
#include <math.h>
#include <string.h>
#include "vmath.h"
#include "quat.h"

//=======================================================================================

//四元数数学库=============================================================
/**
* Basic quaternion operations.
*/

void Quat_computeW (Quat4_t q)
{
	float t = 1.0f - (q[X] * q[X]) - (q[Y] * q[Y]) - (q[Z] * q[Z]);

	if (t < 0.0f)
		q[W] = 0.0f;
	else
		q[W] = -(float)sqrt (t);
}

void Quat_normalize (Quat4_t q)
{
	/* compute magnitude of the quaternion */
	float mag = (float)sqrt ((q[X] * q[X]) + (q[Y] * q[Y])
		+ (q[Z] * q[Z]) + (q[W] * q[W]));

	/* check for bogus length, to protect against divide by zero */
	if (mag > 0.0f)
	{
		/* normalize it */
		float oneOverMag = 1.0f / mag;

		q[X] *= oneOverMag;
		q[Y] *= oneOverMag;
		q[Z] *= oneOverMag;
		q[W] *= oneOverMag;
	}
}

void Quat_multQuat (const Quat4_t qa, const Quat4_t qb, Quat4_t out)
{
	out[W] = (qa[W] * qb[W]) - (qa[X] * qb[X]) - (qa[Y] * qb[Y]) - (qa[Z] * qb[Z]);
	out[X] = (qa[X] * qb[W]) + (qa[W] * qb[X]) + (qa[Y] * qb[Z]) - (qa[Z] * qb[Y]);
	out[Y] = (qa[Y] * qb[W]) + (qa[W] * qb[Y]) + (qa[Z] * qb[X]) - (qa[X] * qb[Z]);
	out[Z] = (qa[Z] * qb[W]) + (qa[W] * qb[Z]) + (qa[X] * qb[Y]) - (qa[Y] * qb[X]);
}

void Quat_multVec (const Quat4_t q, const Vec3_t v, Quat4_t out){
	out[W] = - (q[X] * v[X]) - (q[Y] * v[Y]) - (q[Z] * v[Z]);
	out[X] =   (q[W] * v[X]) + (q[Y] * v[Z]) - (q[Z] * v[Y]);
	out[Y] =   (q[W] * v[Y]) + (q[Z] * v[X]) - (q[X] * v[Z]);
	out[Z] =   (q[W] * v[Z]) + (q[X] * v[Y]) - (q[Y] * v[X]);
}

void Quat_rotatePoint (const Quat4_t q, const Vec3_t in, Vec3_t out){
	Quat4_t tmp, inv, final;

	inv[X] = -q[X]; inv[Y] = -q[Y];
	inv[Z] = -q[Z]; inv[W] =  q[W];

	Quat_normalize (inv);

	Quat_multVec (q, in, tmp);
	Quat_multQuat (tmp, inv, final);

	out[X] = final[X];
	out[Y] = final[Y];
	out[Z] = final[Z];
}
/**
* More quaternion operations for skeletal animation.
*/
float Quat_dotProduct (const Quat4_t qa, const Quat4_t qb)
{
	return ((qa[X] * qb[X]) + (qa[Y] * qb[Y]) + (qa[Z] * qb[Z]) + (qa[W] * qb[W]));
}

/*
*	将四元数转化为矩阵
*/
void
Quat_to_matrrix(const Quat4_t q,Matrix44f m){
	m[0] = 2*(q[X]*q[X] + q[W]*q[W]) - 1;
	m[1] = 2*(q[X] * q[Y] + q[Z] * q[W]);
	m[2] = 2*(q[X]*q[Z] - q[Y]*q[W]);
	m[3] = 0;

	m[4] = 2 * (q[X]*q[Y] - q[Z]*q[W]);
	m[5] = 2 * (q[Y]*q[Y] + q[W]*q[W]) - 1;
	m[6] = 2 * (q[Y]*q[Z] + q[X]*q[W]);
	m[7] = 0;

	m[8] = 2 * (q[X]*q[Z] + q[Y]*q[W]);
	m[9] = 2 * (q[Y]*q[Z] - q[X]*q[W]);
	m[10]= 2 * (q[Z]*q[Z] + q[W]*q[W]) - 1;
	m[11]= 0;

	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
}

/*
*	四元数球面插值
*	t : 0 ~ 1.0f之间
*/
void Quat_slerp (const Quat4_t qa, const Quat4_t qb, float t, Quat4_t out)
{
	float q1w,q1x,q1y,q1z;
	float cosOmega;

	/* Compute interpolation fraction, checking for quaternions
	almost exactly the same */
	float k0, k1;

	/* Check for out-of range parameter and return edge points if so */
	if (t <= 0.0)
	{
		memcpy (out, qa, sizeof(Quat4_t));
		return;
	}

	if (t >= 1.0)
	{
		memcpy (out, qb, sizeof (Quat4_t));
		return;
	}

	/* Compute "cosine of angle between quaternions" using dot product */
	cosOmega = Quat_dotProduct (qa, qb);

	/* If negative dot, use -q1.  Two quaternions q and -q
	represent the same rotation, but may produce
	different slerp.  We chose q or -q to rotate using
	the acute angle. */
	q1w = qb[W];
	q1x = qb[X];
	q1y = qb[Y];
	q1z = qb[Z];

	if (cosOmega < 0.0f)
	{
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		cosOmega = -cosOmega;
	}

	/* We should have two unit quaternions, so dot should be <= 1.0 */
	assert (cosOmega < 1.1f);



	if (cosOmega > 0.9999f)
	{
		/* Very close - just use linear interpolation,
		which will protect againt a divide by zero */

		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		/* Compute the sin of the angle using the
		trig identity sin^2(omega) + cos^2(omega) = 1 */
		float sinOmega = (float)sqrt (1.0f - (cosOmega * cosOmega));

		/* Compute the angle from its sin and cosine */
		float omega = (float)atan2 (sinOmega, cosOmega);

		/* Compute inverse of denominator, so we only have
		to divide once */
		float oneOverSinOmega = 1.0f / sinOmega;

		/* Compute interpolation parameters */
		k0 = (float)sin ((1.0f - t) * omega) * oneOverSinOmega;
		k1 = (float)sin (t * omega) * oneOverSinOmega;
	}

	/* Interpolate and return new quaternion */
	out[W] = (k0 * qa[3]) + (k1 * q1w);
	out[X] = (k0 * qa[0]) + (k1 * q1x);
	out[Y] = (k0 * qa[1]) + (k1 * q1y);
	out[Z] = (k0 * qa[2]) + (k1 * q1z);
}
static f_get_vec(Vec3* vs,Vec3* ve,float v,float* px,float* py,float* pz){
	Quat4_t s;
	Quat4_t e;
	Quat4_t o;

	s[W] = 0;	s[X] = vs->x;	s[Y] = vs->y;	s[Z] = vs->z;
	e[W] = 1;	e[X] = ve->x;	e[Y] = ve->y;	e[Z] = ve->z;

	Quat_slerp(s,e,v,o);
	
	*px = o[X];
	*py = o[Y];
	*pz = o[Z];
}

void quat_slerp_split(	float x0,float y0,float z0,
					float x1,float y1,float z1,
					float* px,float* py,float* pz, 
					float v){

	Vec3 vs,ve;
	vs.x = x0;vs.y = y0;vs.z = z0;
	vec3Normalize(&vs);

	ve.x = x1;ve.y = y1;ve.z = z1;
	vec3Normalize(&ve);
	if(vs.x + ve.x == 0 && vs.y + ve.y == 0 && vs.z + ve.z == 0){
		assert(0);//互为反向向量,此接口不处理这种情况
		return;
	}else{
		//计算中间向量
		Vec3 vm;
		float v1;//中点的w值

		vec3Add(&vs,&ve,&vm);		
		vec3Normalize(&vm);
		
		if(v >= 0.0f && v< 0.5f){
			v1 = v / 0.5f;
			f_get_vec(&vs,&vm,v1,px,py,pz);
		}else{
			v1 = (v - 0.5f) / 0.5f;
			f_get_vec(&vm,&ve,v1,px,py,pz);
		}
	}
}