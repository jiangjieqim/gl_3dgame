#include "tools.h"
#include "gettime.h"

#include "ex.h"
#include "tmat.h"

#include "md5.h"
#include "jgl.h"
#include "atlas.h"

#include "sprite.h"

#include "obj_vbo.h"

#include "node.h"
#include "animtor.h"
#include "tl_malloc.h"
//#################      加载MyDll.lib库      ##################
#pragma comment (lib, "MyDll.lib")
_declspec(dllimport) int jjqadd(int a,int b);
//##############################################################
static struct Vec3* pos1;
static float n1Len;

static struct Vec3 axis;

struct Vec3 outDirection;

struct Vec3 normalpos1;//目标向量的单位向量

struct Vec3 normalpos2;
//static struct Vec3 start;
//axis.x = 0;
//axis.y = 0;
//axis.z = 1;
//pos1.x = -0.45;
//pos1.y = -0.75;
//pos1.z = 0;

///************************************************************************/
///*  m:旋转轴    
//	n:目标轴
//	o:垂直轴
///************************************************************************/
//static void
//f_get_direction(struct Vec3* n,struct Vec3* m,struct Vec3* o){
//	
//}

//打印向量
static void
f_print_vec(char* key,struct Vec3* p){
	printf("%s p = %.3f\t%.3f\t%.3f\n",key,p->x,p->y,p->z);
}
static void
f_call(){
	glBegin(GL_LINES); 
	glVertex3f(0.0, 0.0f ,0.0f);
	glVertex3f(normalpos1.x, normalpos1.y ,normalpos1.z);
	//glVertex3f(-1.0, -1.0f ,0.0f);
	glEnd();

	glBegin(GL_LINES); 
	glVertex3f(0.0, 0.0f ,0.0f);
	glVertex3f(normalpos2.x, normalpos2.y ,normalpos2.z);
	glEnd();
}
//四元数测试方法
int
REG_test_unit_01(lua_State *L){

	//const char* name=lua_tostring(L,1);
	float value = lua_tonumber(L,1);
	//const char* name1=lua_tostring(L,3);

	//int n = (int)ex_find_node(name);

	//int n1 = (int)ex_find_node(name1);
	//struct HeadInfo* b =  base_get2(n1);
	Matrix44f m;//目标矩阵

	//半径长
	float len = 1.0;
	
	Quat4_t s;
	Quat4_t e;
	Quat4_t o;
	
	struct Vec3 subpos1;
	struct Vec3 down;
	//down.x = 0;
	//down.y = -1;
	//down.z = 0;
	
	{

		printf("%d\n",jjqadd(2,9));

		if(!ex_getInstance()->drawLine_callBack){
			ex_getInstance()->drawLine_callBack = f_call;
		}
	}

	if(!pos1){
		
		//初始化pos1坐标
		pos1 = malloc(sizeof(struct Vec3));
		pos1->x = -0.25;
		pos1->y = -0.35;
		pos1->z = 0;

		n1Len = vec3Length(pos1);
	}
	vec3Set(&normalpos1,pos1-> x,pos1->y,pos1->z);
	vec3Normalize(&normalpos1);
	//求pos1的标量

	s[W] = 0;
	s[X] = 0;
	s[Y] = -1;
	s[Z] = 0;
	/*
	//旋转90度
	e[W] = 1;
	e[X] = 0;
	e[Y] = 1;
	e[Z] = 0;
	*/
	/*
	//旋转45度
	e[W] = 1;
	e[X] = 0.707;
	e[Y] = 0.707;
	e[Z] = 0;
	*/
	/*
	e[W] = 1;
	e[X] =  0.707;
	e[Y] = -0.707;
	e[Z] = 0;
	*/
	e[W] = 1;
	e[X] =  -1;
	e[Y] = 0;
	e[Z] = 0;
	Quat_slerp(s,e,value,o);//四元数球面平滑插值
	
	
	//printf("t = %.2f\tw = %.2f\tx = %.2f\ty = %.2f\tz = %.2f\n",value,o[W],o[X],o[Y],o[Z]);
	
	//转化为矩阵
	
	//Quat_to_matrrix(o,m);

	//打印矩阵
	//mat4x4_printf("四元数矩阵",m);
	
	//base_setPos(base_get2(n),o[X] * len,o[Y] * len,o[Z] * len);//设置其坐标
	normalpos2.x = o[X];normalpos2.y = o[Y];normalpos2.z = o[Z];

	//vec3Sub()
	subpos1.x = o[X] - normalpos1.x;
	subpos1.y = o[Y] - normalpos1.y;
	subpos1.z = o[Z] - normalpos1.z;


	//=====================================================
	//设置轴
	axis.x = axis.y = 0;axis.z = 1.0;

	vec3Set(&down,o[X],o[Y],o[Z]);      

	//获取垂直轴
	vec3Cross(&down,&axis,&outDirection);//叉乘获取垂直向量
	//f_print_vec(&normalpos1);
	f_print_vec("outDirection",&outDirection);
	//printf("\n");


	//=====================================================
	{
		 struct Vec3 oa;
		 struct Vec3 oc;
		 struct Vec3 od;
		 struct Vec3 oe;
		 vec3Set(&oa,s[X],s[Y],s[Z]);
		 
		 //base_set_position(base_get2(n1),&normalpos1);
		 f_print_vec("normalpos1",&normalpos1);
		
	}
	
	return 0;
}
