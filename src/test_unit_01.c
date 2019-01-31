#include <stdio.h>
#include <math.h>
#include "tools.h"
#include "tl_malloc.h"
#include "vmath.h"
#include "ex.h"
#include "evt.h"
//#################          ##################
//#pragma comment (lib, "MyDll.lib")
//_declspec(dllimport) int jjqadd(int a,int b);
//##############################################################
static void* _floor;//地板对象
static void* _horse;//模型对象
static void* _target;//目标物

struct Vec3* pos1;
static float n1Len;

struct Vec3 axis;

struct Vec3 outDirection;

struct Vec3 normalpos1;

struct Vec3 normalpos2;
//##############################################################

///**
//    * 求两个向量的单位方向向量
//    * s: 起始向量 
//    * e: 结束向量
//    */
//   float convertNormalVec(s: egret.Point, e: egret.Point): egret.Point {
//       let sub: egret.Point = new egret.Point(e.x - s.x, e.y - s.y)
//       let d: number = egret.Point.distance(s, e)//Math.sqrt(e.x)
//       sub.x /= d;
//       sub.y /= d;
//       return sub
//   }
#define  _ARROW_OBJ_ "myBox"

static void
f_print_vec(char* key,struct Vec3* p){
	printf("%s p = %.3f\t%.3f\t%.3f\n",key,p->x,p->y,p->z);
}
static double
f_toAngle(double value){
	return value * 180.0f / PI;
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
//拾取坐标
static void
box_rayPick(int evtId,void* data){
	struct HitResultObject* hit = (struct HitResultObject*)data;
	//printf("%s\n",hit->name);

	struct HeadInfo* base =  base_get(ex_find(_ARROW_OBJ_));
	base->rz=vec_rotateAngle(hit->x, hit->y, 1.0f, 0.0f);
	updateMat4x4(base);
}

//地板点击触发
static void
floor_rayPick(int evtId,void* data){
	struct HitResultObject* hit = (struct HitResultObject*)data;
	struct HeadInfo* ptrHorse = (struct HeadInfo*)_horse;
	//horse->x
	//vec3Sub()
	{
		struct Vec3 pos;
		float x = hit->x - ptrHorse->x;
		float y = hit->y - ptrHorse->y;
		float z = hit->z - ptrHorse->z;
		pos.x = x;
		pos.y = y;
		pos.z = z;

		vec3Normalize(&pos);//求单位向量
		printf("************* %s,%.3f,%.3f,%.3f\t,%.3f,%.3f,%.3f\n", hit->name,hit->x,hit->y,hit->z,pos.x,pos.y,pos.z);
		ptrHorse->ry = vec_rotateAngle(pos.x, pos.z, 1.0f, 0.0f);//设置角色的朝向
		updateMat4x4(ptrHorse);//更新角色矩阵

		base_setPos(_target, hit->x, hit->y, hit->z);//设置拾取小盒子的位置
	}
}

static int _initStat = 0;

static void f_drawLineEvt(int evtId,void* data){
	f_call();
}

struct HeadInfo* obj1Base;
REG_test_unit_01(lua_State *L){
	float value = lua_tonumber(L,1);
	float len = 1.0;
	Quat4_t s;
	Quat4_t e;
	Quat4_t o;
	
	
	struct HeadInfo* base;
	
	struct Vec3 subpos1;
	struct Vec3 down;
	
	if(!_initStat){
		//初始化

		//添加一个3D渲染回调
		evt_on(ex_getInstance(),EVENT_ENGINE_RENDER_3D,f_drawLineEvt);


		if(!obj1Base){
			obj1Base =  base_get(ex_find("myObj1"));
			//添加对象拾取监听事件
			evt_on(obj1Base,EVENT_RAY_PICK,box_rayPick);
		}

		//_floor = ex_load_model("floor","\\resource\\obj\\teapot.obj",E_RenderModeVBO);
		//base_set_scale(_floor,1.0f);
		//setv(_floor,FLAGS_VISIBLE);
		_floor = base_get(ex_find("_floor"));
		evt_on(_floor,EVENT_RAY_PICK,floor_rayPick);

		_horse =  base_get(ex_find("_horse"));

		// x,z		基于y轴旋转
		_target = base_get(ex_find("_target"));
		


		_initStat = 1;
	}
	

	//测试旋转对象
	base = base_get(ex_find(_ARROW_OBJ_));
	

	
	/*if(!base->pPickCallBack){
		base->pPickCallBack = box_rayPick;
	}*/

	if(!pos1){
		pos1 = tl_malloc(sizeof(struct Vec3));
		pos1->x = -0.25;
		pos1->y = -0.35;
		pos1->z = 0;
		n1Len = vec3Length(pos1);
	}
	vec3Set(&normalpos1,pos1-> x,pos1->y,pos1->z);
	vec3Normalize(&normalpos1);//求得单位向量

	s[W] = 0;	
	//s[X] = 0;s[Y] = -1;
	s[X] = 1;s[Y] = 0;
	s[Z] = 0;//起始四元数


	e[W] = 1;	
	//e[X] =  -1;e[Y] = 0;
	e[X] =  0;e[Y] = -1;
	e[Z] = 0;//结束四元数
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
	


	//=====================================================
	{
		double _ang;
		float angle;
		struct Vec3 oa;
		//struct Vec3 oc;
		//struct Vec3 od;
		//struct Vec3 oe;
		vec3Set(&oa,s[X],s[Y],s[Z]);

		//base_set_position(base_get2(n1),&normalpos1);
		//f_print_vec("normalpos1",&normalpos1);
		
		//方向向量outDirection基于x水平轴1,0的角度
		_ang = vec_rotateAngle(outDirection.x, outDirection.y, 1.0f, 0.0f);
		angle=f_toAngle(_ang);
		if(base){

			base->rz = _ang;
			updateMat4x4(base);
		}
		printf("outDirection:\t%.3f\t%.3f\t%.3f\nnormalpos1:\t%.3f\t%.3f\t%.3f angle=%.2f \t _ang=%.2f\n\n",outDirection.x,outDirection.y,outDirection.z,normalpos1.x,normalpos1.y,normalpos1.z,angle,_ang);
	}
	return 0;
}
