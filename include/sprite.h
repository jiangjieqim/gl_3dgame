#ifndef _SPRITE_H_
#define _SPRITE_H_
#include "vmath.h"
#include "spritehit.h"

//#include <stdarg.h>
//#define USE_VBO_SPRITE	//�Ƿ�ʹ��VBOģʽ��Sprite
/************************************************************************/
/* �Ź���ṹ��                                                                     */
/************************************************************************/
struct Grid9Node{
	float left,right,top,bottom,sx,sy,width,height;
};
//2d��������
struct Sprite{

	void* base;
	float pos_x;		//��ʾ������ͶӰ����ϵ(���½�Ϊ����ԭ��)�����꣬
	float pos_y;
	float pos_z;		//�ô�ֵ����ʶsprite�Ƿ���ǰ��һ��s
	
	//z�������
	float zScale;

	//��Ļ����,�����Ͻǿ�ʼ����0,0
	float screenX;
	float screenY;

	float mWidth;		//sprite����Ļ���ص�λ
	float mHeight;
	int m_bPressed;

	int mouseDownX,mouseDownY;//����������µ�����

	void* vertexs;//GLfloat*���㻺������,ʹ����֮��Ҫɾ��tl_free
	int	vertLen;

	void* material;//����

	struct Atals* atals;//ͼ��
	
	/************************************************************************/
	/* ��������                                                    */
	/************************************************************************/
	//void* material;
	
	Matrix44f mat4x4;
	
	/*
		Sprite����ص�
	*/
	void (*clickCallBack)(struct Sprite* ptrSprite,int localX,int localY);

	int parseType;
	//int useVBO;//�Ƿ�ʹ��VBOģʽ
	struct Obj_vbo_model* vbo;

	//�Ӷ����б�
	struct LStackNode *childs;
	//������
	void* parent;
	//�ֲ�����,����ڸ�����,�������stage�Ͼ�Ϊ0,0
	int localx,localy;

	struct SpriteHit* pHit;
};

#define SPRITE_Z 0
#define SPRITE_SELF_XY 1		
#define SPRITE_GRID 2	//�Ź���
#define SPRITE_ATALS 3	//ͼ��
#define SPRITE_2DCAM 4	//����camera2d
#define SPRITE_WIDTH  5	
#define SPRITE_HEIGHT 6
#define SPRITE_DRAG 7	//���ÿ���ק�ķ�Χ
#define SPRITE_RX 8
#define SPRITE_RY 9
#define SPRITE_RZ 10
#define SPRITE_SCALE_Z 11//��������ֵ
#define SPRITE_IS_ENABLE 12//�Ƿ��Ǽ���״̬
#define SPRITE_MOUSE_ENABLE 13//������¼��Ƿ����
#define SPRITE_MATERIAL 14//����
#define SPRITE_UV 15//����UV
#define SPRITE_HIT_RECT 16//���õ����������
//material

//��ȡ����
int sprite_get(void* ptr,int flag);

//��������
void sprite_set(void* sprite,int flag,...);

/*,
 *Ϊspr���һ���Ӷ���
 **/
void sprite_addChild(void* spr,void* child);
/*�Ƴ��Ӷ���*/
void sprite_removeChild(void* spr,void* child);

//���õ���ص�
void
sprite_set_clickHandle(void* p,void (*clickCallBack)(void* ,int ,int ));

/* 
	��ʼ����ť

	void (*clickCallBack)(struct Sprite* ,int ,int )	����ص�����,���δ���ûص������ǲ������õ��ʰȡ�¼���
*/
struct Sprite* sprite_create(
	char* _spriteName,
	int x,int y,
	int width,int height,
	void (*clickCallBack)(struct Sprite* ,int ,int ),void* _2dCam);

/*
	����һ��sprite 
*/
void sprite_drawRender(int data);

/*
	��ť�¼���Ч
*/
void sprite_action(const int data);

/*
	��Ⱦ����ui�ڵ�
*/
void sprite_updatePos(int data);
/*
	ɾ��sprite�ڵ�
*/
void sprite_dipose(struct Sprite* spr);

/*
	����ͼ���е�key����λͼ,atlas��ʵ��һ������
*/
void sprite_texName(struct Sprite* spr,const char* texName,struct AtlasNode* ptr);
/*
	��������
*/
void sprite_setpos(struct Sprite* spr,int x,int y);

/*
	�������������קֻ�øýӿڸ���sprite����
*/
void sprite_mouseMove(int data);

/*
	����sprite�ĳߴ�
*/
void sprite_resize(struct Sprite* spr,int w,int h);

//����һ��û�и���ͼ��2dSprite
void*
sprite_createEmptyTex(int texW,int texH,void* _2dCam);

#endif