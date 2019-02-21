#ifndef _FRAME_H_
#define _FRAME_H_
/*
*	动作管理结构体
*	用于切换关键帧,播放动作
*/
typedef struct FrameAnim
{
	/*
	 *	当前动作的起始关键帧和结束关键帧索引号(从0开始)
	 */
	int frameStart;
	/*
	 *	结束帧索引,该索引从0开始
	 */
	int frameEnd;
	
	/*
	 * 每一帧的间隔时间
	 * fps = 60;
	 * fpsInterval = 1000 / fps = 16.6666;
	 */
	long fpsInterval;

	/*
	 * 当前的插值
	 */
	long _subTick;

	/*
	*	当前关键帧索引号(从0开始)
	*/
	int curFrame;

	
	/*
	*当前动作
	*/
	char curAnim[G_BUFFER_16_SIZE];
	/*
	*前一次动作
	*/
	char oldAnim[G_BUFFER_16_SIZE];
	
	/*
	*	动作片段配置
	*	"stand,0,39|run,40,45|"
	*/
	char animConfig[G_BUFFER_128_SIZE];

	/*
	*	播放一次动作回调
	*/
	//CallBackFun playOnceCallBack;
	void (*playOnce)(struct FrameAnim*);
	

}FrameAnim;
/*
*	计算关键帧
*/
void frame_run(struct FrameAnim* frameAnim);
/*
*	该接口只播放一次动作,播放完一次循环后,恢复上一个动作
*/
void frame_play(struct FrameAnim* frameAnim,const char* anim);
/*
*	设置动作
*/
void frame_set(struct FrameAnim* frameAnim,const char* anim);

#endif