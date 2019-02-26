//#define BUILDING_DLL

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define BUILDING_DLL

#include "tools.h"
#include "vmath.h"


//矩阵处理

void mat4x4_zero(Matrix44f M){
	int i;
	for(i = 0; i < 16; i++)
		M[i] =0;
}

void mat4x4_2t1(Matrix44f M,int m,int n,float value)
{
	M[4 * m + n] = value;
}

/**
 * 矩阵行列转置
 * Transpose the values of a mat4
 *
 * @param {mat4} out the receiving matrix
 * @param {mat4} a the source matrix
 * @returns {mat4} out
 */
void mat4x4_transpose(Matrix44f a) 
{
	Matrix44f out;
	out[0] = a[0];
	out[1] = a[4];
	out[2] = a[8];
	out[3] = a[12];
	out[4] = a[1];
	out[5] = a[5];
	out[6] = a[9];
	out[7] = a[13];
	out[8] = a[2];
	out[9] = a[6];
	out[10] = a[10];
	out[11] = a[14];
	out[12] = a[3];
	out[13] = a[7];
	out[14] = a[11];
	out[15] = a[15];
	memcpy(a,out,sizeof(Matrix44f));
}

void 
mat4x4_lookAt(Matrix44f out,struct Vec3* eye,struct Vec3* center,struct Vec3* up) 
{
	float x0 =0,x1=0 ,
		x2 =0,
		y0 =0,
		y1 =0,
		y2 =0,
		z0 =0,
		z1 =0,
		z2 =0,
		len =0;
	float eyex = eye->x;
	float eyey = eye->y;
	float eyez = eye->z;
	float upx = up->x;
	float upy = up->y;
	float upz = up->z;
	float centerx = center->x; 
	float centery = center->y;
	float centerz = center->z;

	if (fabs(eyex - centerx) < EPSILON && 
		fabs(eyey - centery) < EPSILON &&
		fabs(eyez - centerz) < EPSILON) 
	{
		mat4x4_identity(out);
		return;
	}

	z0 = eyex - centerx;
	z1 = eyey - centery;
	z2 = eyez - centerz;

	len = 1 / (float)sqrt(z0 * z0 + z1 * z1 + z2 * z2);
	z0 *= len;
	z1 *= len;
	z2 *= len;

	x0 = upy * z2 - upz * z1;
	x1 = upz * z0 - upx * z2;
	x2 = upx * z1 - upy * z0;
	len = (float)sqrt(x0 * x0 + x1 * x1 + x2 * x2);
	if (!len) {
		x0 = 0;
		x1 = 0;
		x2 = 0;
	} else {
		len = 1 / len;
		x0 *= len;
		x1 *= len;
		x2 *= len;
	}

	y0 = z1 * x2 - z2 * x1;
	y1 = z2 * x0 - z0 * x2;
	y2 = z0 * x1 - z1 * x0;

	len = (float)sqrt(y0 * y0 + y1 * y1 + y2 * y2);
	if (!len) {
		y0 = 0;
		y1 = 0;
		y2 = 0;
	} else {
		len = 1 / len;
		y0 *= len;
		y1 *= len;
		y2 *= len;
	}

	out[0] = x0;
	out[1] = y0;
	out[2] = z0;
	out[3] = 0;
	out[4] = x1;
	out[5] = y1;
	out[6] = z1;
	out[7] = 0;
	out[8] = x2;
	out[9] = y2;
	out[10] = z2;
	out[11] = 0;
	out[12] = -(x0 * eyex + x1 * eyey + x2 * eyez);
	out[13] = -(y0 * eyex + y1 * eyey + y2 * eyez);
	out[14] = -(z0 * eyex + z1 * eyey + z2 * eyez);
	out[15] = 1;
}


/////////////////////////////////////////////////////////////////////
//
void mat4x4_translate(Matrix44f M, float x, float y, float z)
{
	//clearMat(M);
    M[3]  =x;
    M[7]  =y;
    M[11] =z;
    M[15] =1.0f;
}
/*
 *矩阵的逆
 */
void mat4x4_invert(Matrix44f out,Matrix44f a) {
	int i;
	float a00 = a[0],
		a01 = a[1],
		a02 = a[2],
		a03 = a[3];
	float a10 = a[4],
		a11 = a[5],
		a12 = a[6],
		a13 = a[7];
	float a20 = a[8],
		a21 = a[9],
		a22 = a[10],
		a23 = a[11];
	float a30 = a[12],
		a31 = a[13],
		a32 = a[14],
		a33 = a[15];

	float b00 = a00 * a11 - a01 * a10;
	float b01 = a00 * a12 - a02 * a10;
	float b02 = a00 * a13 - a03 * a10;
	float b03 = a01 * a12 - a02 * a11;
	float b04 = a01 * a13 - a03 * a11;
	float b05 = a02 * a13 - a03 * a12;
	float b06 = a20 * a31 - a21 * a30;
	float b07 = a20 * a32 - a22 * a30;
	float b08 = a20 * a33 - a23 * a30;
	float b09 = a21 * a32 - a22 * a31;
	float b10 = a21 * a33 - a23 * a31;
	float b11 = a22 * a33 - a23 * a32;

	// Calculate the determinant
	float det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

	for(i = 0;i < 16;i++)
	{
		out[i] = 0;
	}
	if (!det) {
		return;
	}
	det = (float)(1.0 / det);

	out[0] = (a11 * b11 - a12 * b10 + a13 * b09) * det;
	out[1] = (a02 * b10 - a01 * b11 - a03 * b09) * det;
	out[2] = (a31 * b05 - a32 * b04 + a33 * b03) * det;
	out[3] = (a22 * b04 - a21 * b05 - a23 * b03) * det;
	out[4] = (a12 * b08 - a10 * b11 - a13 * b07) * det;
	out[5] = (a00 * b11 - a02 * b08 + a03 * b07) * det;
	out[6] = (a32 * b02 - a30 * b05 - a33 * b01) * det;
	out[7] = (a20 * b05 - a22 * b02 + a23 * b01) * det;
	out[8] = (a10 * b10 - a11 * b08 + a13 * b06) * det;
	out[9] = (a01 * b08 - a00 * b10 - a03 * b06) * det;
	out[10] = (a30 * b04 - a31 * b02 + a33 * b00) * det;
	out[11] = (a21 * b02 - a20 * b04 - a23 * b00) * det;
	out[12] = (a11 * b07 - a10 * b09 - a12 * b06) * det;
	out[13] = (a00 * b09 - a01 * b07 + a02 * b06) * det;
	out[14] = (a31 * b01 - a30 * b03 - a32 * b00) * det;
	out[15] = (a20 * b03 - a21 * b01 + a22 * b00) * det;
}

/**
 * Transforms the vec2 with a mat4
 * 3rd vector component is implicitly '0'
 * 4th vector component is implicitly '1'
 *
 * @param {vec2} out the receiving vector
 * @param {vec2} a the vector to transform
 * @param {mat4} m matrix to transform with
 * @returns {vec2} out
 */
//void mat4x4_transformMat4(float out[4],float a[4],Matrix44f m) {
//  float x = a[0];
//  float y = a[1];
//  out[0] = m[0] * x + m[4] * y + m[12];
//  out[1] = m[1] * x + m[5] * y + m[13];
//}

void mat4x4_transformMat4(float out[4],float a[4],Matrix44f m) {
	float x = a[0],
		y = a[1],
		z = a[2];
	float w = m[3] * x + m[7] * y + m[11] * z + m[15];
	w = (float)(w || 1.0);
	out[0] = (m[0] * x + m[4] * y + m[8] * z + m[12]) / w;
	out[1] = (m[1] * x + m[5] * y + m[9] * z + m[13]) / w;
	out[2] = (m[2] * x + m[6] * y + m[10] * z + m[14]) / w;
}

//void mat4x4_getTranslate(Matrix44f M,struct Vec3* pos)
//{
//	pos->x = M[3];
//	pos->y = M[7];
//	pos->z = M[11];
//}
void mat4x4_scale(Matrix44f M, float x, float y, float z)
{
	//clearMat(M);
    M[0]  =x;
    M[5]  =y;
    M[10] =z;
    M[15] =1.0;
}
void mat4x4_identity(Matrix44f M){
	mat4x4_zero(M);
	M[0] =1.0;
    M[5]  =1.0;
    M[10] =1.0;
    M[15] =1.0;
}
//void matx4x4_zore(Matrix44f M){
//	mat4x4_clearMat(M);
//	//mat4x4_identity(M);
//}
void mat4x4_printf(const char* name,Matrix44f M)
{
	int i,n=0;
	
	printf("矩阵%s:=\n",name);
	for(i = 0;i < 16;i++){
		if(n >= 4){
			printf("\n");
			n=0;
		}
		printf("[%d]	%.3f\t\t",i,M[i]);//保留小数点3位
		n++;
	}
	printf("\n****************************************************\n");
}
void mat4x4_double_printf(const char* name,double m[16])
{
	int i,n=0;

	printf("矩阵%s:=\n",name);
	for(i=0;i<16;i++){
		if(n>=4){
			printf("\n");
			n=0;
		}
		printf("[%d]	%f\t\t",i,m[i]);
		n++;
	}
	printf("\n****************************************************\n");
}
///////////////////////////////////////////////////////////////////////////
////
void mat4x4_rotateX(Matrix44f M, float radian)
{
	//clearMat(M);

    M[0]  =1.0;
    M[5]  =(float)cos(radian);
    M[6]  =(float)sin(radian);
    M[9]  =-(float)sin(radian);
    M[10] =(float)cos(radian);
    M[15] =1.0;
}
void mat4x4_rotateY(Matrix44f M, float radian)
{
	//clearMat(M);
    M[0]  =(float)cos(radian);
    M[2]  =-(float)sin(radian);
    M[5]  =1.0;
    M[8]  =(float)sin(radian);
    M[10] =(float)cos(radian);
    M[15] =1.0;
	//printf("%f %f\n",(float)cos(radian),cos(radian));
}

void mat4x4_copy(Matrix44f M,Matrix44f out)
{
	memcpy(out,M,sizeof(Matrix44f));
}
void mat4x4_rotateZ(Matrix44f M, float radian)
{
	//clearMat(M);
    M[0]  =(float)cos(radian);
    M[1]  =-(float)sin(radian);
    M[4]  =(float)sin(radian);
    M[5]  =(float)cos(radian);
    M[10] =1.0;
    M[15] =1.0;
}

void mat4x4_rotateXYZ(Matrix44f M, float radian, float x, float y, float z)
{
    int i;
     
 
    for(i= 0; i <16; i++)
        M[i] =0;
 
}
 
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
void perspectiveFrustum(Matrix44f M, float left, float right, float top, float bottom, float near, float far)
{	
	M[0]  =2*near/(right -left);
    M[2]  =(right +left)/(right -left);
    
	M[5]  = 2*near/(top -bottom);
	M[5]  = -2*near/(top -bottom);//这里翻转一下

    M[6]  =(top +bottom)/(top -bottom);
    M[10] =(near +far)/(near -far);

    M[11] =2*near*far/(near -far);
    M[14] =-1.0;

	/*M[11] =-1.0;
	M[14] =2*near*far/(near -far);*/
}
 
void orthoPerspect(Matrix44f M,  float left, float right, float top, float bottom, float near, float far)
{
	//clearMat(M);
    M[0]  =2/(right -left);
    M[3]  =(left +right)/(left -right);
    M[5]  =2/(top -bottom);
    M[7]  =(bottom +top)/(bottom -top);
    M[10] =2/(near -far);
    M[11] =(near +far)/(far -near);
    M[15] =1.0;
}
//
//void matrix_m(float **a_matrix, const float **b_matrix, const float **c_matrix,
//			  int krow, int kline, int kmiddle, int ktrl)
//			  ////////////////////////////////////////////////////////////////////////////
//			  //	a_matrix=b_matrix*c_matrix
//			  //	krow  :行数
//			  //	kline :列数
//			  //	ktrl  :	大于0:两个正数矩阵相乘 不大于0:正数矩阵乘以负数矩阵
//			  ////////////////////////////////////////////////////////////////////////////
//{
//	int k, k2, k4;
//	float stmp;
//
//	for (k = 0; k < krow; k++)     
//	{
//		for (k2 = 0; k2 < kline; k2++)   
//		{
//			stmp = 0.0;
//			for (k4 = 0; k4 < kmiddle; k4++)  
//			{
//				stmp += b_matrix[k][k4] * c_matrix[k4][k2];
//			}
//			a_matrix[k][k2] = stmp;
//		}
//	}
//	if (ktrl <= 0)   
//	{
//		for (k = 0; k < krow; k++)
//		{
//			for (k2 = 0; k2 < kline; k2++)
//			{
//				a_matrix[k][k2] = -a_matrix[k][k2];
//			}
//		}
//	}
//}


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
//void tl_set_vec(Vec3_t vec,float x,float y,float z)
//{
//	vec[X] = x;
//	vec[Y] = y;
//	vec[Z] = z;
//}
void tl_add_vec(Vec3_t a,Vec3_t b,Vec3_t vec)
{
	vec[X] = a[X]+b[X];
	vec[Y] = a[Y]+b[Y];
	vec[Z] = a[Z]+b[Z];
}

void tl_offset_vec(Vec3_t pos,float x,float y,float z)
{
	pos[X] += x;
	pos[Y] += y;
	pos[Z] += z;
}

void tl_scale_vec(Vec3_t vec,Vec3_t res,float scale){
	res[X] = vec[X] * scale;
	res[Y] = vec[Y] * scale;
	res[Z] = vec[Z] * scale;
}
//void tl_set_vec(Vec3_t res,float x,float y,float z){
//	res[X] = x;
//	res[Y] = y;
//	res[Z] = y;
//}

void vec3Set(struct Vec3* v,float x,float y,float z){
	v->x = x;
	v->y = y;
	v->z = z;
}
void 
vec3Reverse(Vec3* v){
	//v->x*=-1; v->y*=-1; v->z*=-1;
	vec3Mult(v,-1);
}
void vec3Add(struct Vec3* a,struct Vec3* b,struct Vec3* out){
	out->x = a->x+b->x;
	out->y = a->y+b->y;
	out->z = a->z+b->z;
}
//out = a - b    
void vec3Sub(struct Vec3* a,struct Vec3* b,struct Vec3* out){
	out->x = a->x-b->x;
	out->y = a->y-b->y;
	out->z = a->z-b->z;
}
void vec3Cross(struct Vec3* a,struct Vec3* v,struct Vec3* out){
	out->x = a->y*v->z - a->z*v->y;
	out->y = a->z*v->x - a->x*v->z;
	out->z = a->x*v->y - a->y*v->x;
}
void vec3Mult(struct Vec3* a,float value){
	a->x *= value;
	a->y *= value;
	a->z *= value;
}

float vec3Distance(struct Vec3* a,struct Vec3* b){
	return	(float)sqrt(pow(a->x - b->x,2)+pow(a->y - b->y,2)+pow(a->z - b->z,2));
}
void vec3Div(struct Vec3* o,float v){
	o->x /=v;
	o->y /=v;
	o->z /=v;
}
float vec3Length(struct Vec3* v){
	//平方根x*x + y*y + z*z
	return	(float)sqrt(v->x*v->x + v->y*v->y + v->z*v->z );
}
int 
vec3IsZero(struct Vec3* v){
	if(v->x==0 && v->y==0 && v->z==0){
		return 1;
	}
	return 0;
}
void 
vec3Normalize(struct Vec3* v){
	float length = vec3Length(v);
	v->x = v->x / length;
	v->y = v->y / length;
	v->z = v->z / length;
}
void 
vec3CalculateNormal(struct Vec3* a,struct Vec3* b,struct Vec3* c,struct Vec3* res)
{
	struct Vec3 out1,out2;
	vec3Sub(a,b,&out1);
	vec3Sub(a,c,&out2);
	vec3Cross(&out1,&out2,res);
}

/*
	射线检测三角形角点
	triangle:	三个顶点,表示一个三角形
*/
int f_testHit(struct Vec3* base,struct Vec3* nearPoint,struct Vec3* farPoint,float* t,float* out){
	//struct HeadInfo* base =(struct HeadInfo*)_md2->head;
	float orig[3]= {nearPoint->x,nearPoint->y,nearPoint->z};//射线坐标
	float target[3] = {farPoint->x,farPoint->y,farPoint->z};//目标点

	float dir[3] = {orig[0]-target[0],orig[1]-target[1],orig[2]-target[2]};//射线向量-目标目标向量=方向向量

	float vert0[3] = {t[0] + base->x,t[1]+ base->y,t[2] + base->z};
	float vert1[3] = {t[3] + base->x,t[4]+ base->y,t[5] + base->z};
	float vert2[3] = {t[6] + base->x,t[7]+ base->y,t[8] + base->z};
	
	int hit;


	tl_hitTriangle(orig,dir,vert0,vert1,vert2,out,&hit);

//#define _HIT_LOG_	//是否打印日志

#ifdef _HIT_LOG_
	int i;
	if(GL_TRUE){
		printf("是否有交点 %d,交点坐标 x:%f y:%f z:%f  检测坐标:",hit,out[0],out[1],out[2]);
		
		for(i = 0;i<3;i++){
			printf("%f,",vert0[i]);
		}

		for(i = 0;i<3;i++){
			printf("%f,",vert1[i]);
		}

		for(i = 0;i<3;i++){
			printf("%f,",vert2[i]);
		}

		printf("\n");
	}
#endif
#undef _HIT_LOG_
	return hit;

	
}

void 
tl_pickTriangle(float* tri,int dataCount,
	struct Vec3* nearPoint,struct Vec3* farPoint,struct Vec3* fpos,
	struct HitResultObject* pResult)
{
	int i;
	float out[3];
	struct Vec3 hitVec;
	struct Vec3 _tpos;
	struct Vec3* pos = &_tpos;
	struct Vec3* pHitVec=&hitVec;

	//一个三角形需要的float个数,代表3个顶点
	#define _VERT_FLOAT_COUNT_ 9
	
	int count = _VERT_FLOAT_COUNT_;				
	int len = dataCount / count;	//三角形个数
	float v[_VERT_FLOAT_COUNT_];
	
	#undef _VERT_FLOAT_COUNT_

	//printf("-------------->开始检测一个模型的包围盒!\n");
	float _distance = INF;
	int size = len*count;

	if(tri==NULL){
		//没有顶点数据,不存在包围盒数据
		return;
	}
	
	for(i = 0;i < size;i+=count){
		
		pos->x = fpos->x;
		pos->y = fpos->y;
		pos->z = fpos->z;



		//对v进行赋值(v长度为9 一个三角形需要的float个数为9 描述其坐标)
		memcpy(v,tri+i,sizeof(float)*count);
		
		

		if(f_testHit(pos,nearPoint,farPoint,v,out)){

			//交点与摄影机的距离
			float _temp;

			struct Vec3 hit;
			
			//对hit进行赋值
			memcpy(&hit,out,sizeof(float)*3);

			//找到多个交点的时候,取距离射线起点最近的交点
			_temp=vec3Distance(&hit,nearPoint);

			if(_temp < _distance){
				_distance = _temp;

				//对pHitVec进行赋值
				memcpy(pHitVec,out,sizeof(float)*3);
			}
			//printf("=====================>检测成功! 射线与三角面的相交点:%.3f,%.3f,%.3f 三角形索引号 %d 距离射线起点的距离 %.3f\n",out[0],out[1],out[2],i,_temp);
			
			/*printf("三角形数据:");
			for(k = 0; k < count;k++){
				printf("%.3f  ",v[k]);
			}
			printf("\n");*/
			//可以将所有的交点点放到列表中
		}
		else
		{
			
		}
	}

	//printf("------------->检测结束!\n");
	if(_distance!=INF){
		pResult->isHit = TRUE;
		pResult->x = pHitVec->x;
		pResult->y = pHitVec->y;
		pResult->z = pHitVec->z;
		pResult->distance = _distance;

		//memcpy(pResult->pickTriangle,v,count*sizeof(float));
		//printf("***********************射线检测成功! 交点坐标:%.3f,%.3f,%.3f 距离射线起点: %.3f \n",pHitVec->x,pHitVec->y,pHitVec->z,pResult->distance);

	}else{
		//未拾取到三角形,点击到了空的区域
	}
	//return _distance;
}

/*
*	向量转化为角度
*/
double
vec_rotateAngle(double x1,double y1, double x2,double y2) {
	double epsilon = 0.000001;//1.0e-6;//1乘10的-6次幂,0.000001
	double nyPI = PI;//acos(-1.0);
	double dist, dot,angle;

	// normalize 单位向量
	dist = sqrt(x1*x1 + y1*y1);
	if(dist > 1.0) dist = 1.0f;

	x1 /= dist;
	y1 /= dist;
	dist =sqrt(x2*x2 + y2*y2);

	if(dist > 1.0) dist = 1.0f;

	x2 /= dist;
	y2 /= dist;
	// dot product	点乘
	dot = x1*x2+y1*y2;
	
	if (fabs(dot-1.0f) <= epsilon)
		angle = 0.0;
	else if (fabs(dot+1.0f)<=epsilon){
		angle = nyPI;
	}else {
		double cross;

		angle = acos(dot);
		//cross product
		cross = x1*y2 - x2*y1;
		// vector p2 is clockwise from vector p1 
		// with respect to the origin (0.0)
		if (cross < 0) {
			angle = 2 * nyPI - angle;
		}
	}
	
	//degree = angle*180.0f/nyPI;

	//return -angle + PI/2;//取负后加1/2的PI
	return -angle;
}



#define _Matrix44_Max_Size_ 16//最大支持的矩阵连乘个数
/************************************************************************/
/* 将arr中的矩阵相乘之后存储到o中                                       */
/************************************************************************/
static void 
f_get_mat4x4(int lim,int* arr,Matrix44f o){
	int i;
	Matrix44f s,t,r,m;
	mat4x4_identity(r);

	for(i = 0; i < lim; i++){
		mat4x4_copy(r,t);
		memcpy((void*)s,(const void*)arr[i],/*(16 * sizeof(float))*/sizeof(Matrix44f));
		mat4x4_zero(m);
		multi2(m,t,s);
		mat4x4_copy(m,r);
	}

	mat4x4_copy(r,o);
}
void 
mat4x4_mult(int lim,Matrix44f o,...){

	mat4x4_zero(o);

	if(lim>_Matrix44_Max_Size_){
		printf("mat4x4_mult长度超出%d\n",_Matrix44_Max_Size_);
		assert(0);
	}else{

		//矩阵地址列表
		int arr[_Matrix44_Max_Size_];
		int i;

		va_list ap;

		va_start(ap, o);

		for(i = 0; i < lim; i++){
			int p = va_arg(ap, int);			
			arr[i] = p;
		}
		va_end(ap);

		f_get_mat4x4(lim,arr,o);
	}
}