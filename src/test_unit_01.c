#include <stdio.h>
#include "tools.h"
#include "ex.h"
//#################          ##################
//#pragma comment (lib, "MyDll.lib")
//_declspec(dllimport) int jjqadd(int a,int b);
//##############################################################
struct Vec3* pos1;
static float n1Len;

struct Vec3 axis;

struct Vec3 outDirection;

struct Vec3 normalpos1;

struct Vec3 normalpos2;

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

int
REG_test_unit_01(lua_State *L){
	float value = lua_tonumber(L,1);
	float len = 1.0;
	Quat4_t s;
	Quat4_t e;
	Quat4_t o;
	
	struct Vec3 subpos1;
	struct Vec3 down;
	{

		//printf("%d\n",jjqadd(2,9));

		if(!ex_getInstance()->drawLine_callBack){
			ex_getInstance()->drawLine_callBack = f_call;
		}
	}

	if(!pos1){
		pos1 = malloc(sizeof(struct Vec3));
		pos1->x = -0.25;
		pos1->y = -0.35;
		pos1->z = 0;
		n1Len = vec3Length(pos1);
	}
	vec3Set(&normalpos1,pos1-> x,pos1->y,pos1->z);
	vec3Normalize(&normalpos1);//求得单位向量
	
	s[W] = 0;s[X] = 0;s[Y] = -1;s[Z] = 0;//起始四元数
	e[W] = 1;e[X] =  -1;e[Y] = 0;e[Z] = 0;//结束四元数
	Quat_slerp(s,e,value,o);//s 转化到 e 输出到o
	
	normalpos2.x = o[X];normalpos2.y = o[Y];normalpos2.z = o[Z];

	//vec3Sub()
	subpos1.x = o[X] - normalpos1.x;
	subpos1.y = o[Y] - normalpos1.y;
	subpos1.z = o[Z] - normalpos1.z;


	//=====================================================
	
	axis.x = axis.y = 0;axis.z = 1.0;

	vec3Set(&down,o[X],o[Y],o[Z]);      

	vec3Cross(&down,&axis,&outDirection);
	//f_print_vec(&normalpos1);
	//f_print_vec("outDirection",&outDirection);
	//printf("\n");


	//=====================================================
	{
		 struct Vec3 oa;
		 //struct Vec3 oc;
		 //struct Vec3 od;
		 //struct Vec3 oe;
		 vec3Set(&oa,s[X],s[Y],s[Z]);
		 
		 //base_set_position(base_get2(n1),&normalpos1);
		 //f_print_vec("normalpos1",&normalpos1);
		 printf("outDirection:\t%f\t%f\t%f\nnormalpos1:\t%f\t%f\t%f\n\n",outDirection.x,outDirection.y,outDirection.z,normalpos1.x,normalpos1.y,normalpos1.z);
	}
	return 0;
}
