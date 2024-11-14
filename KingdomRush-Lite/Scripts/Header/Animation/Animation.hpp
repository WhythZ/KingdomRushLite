#ifndef _ANIMATION_HPP_
#define _ANIMATION_HPP_

#include <vector>
//引入回调函数
#include <functional>
//纹理支持
#include <SDL.h>
//引入计时器
#include "../Timer/Timer.hpp"

//动画播放的默认帧间隔
#define FRAME_INTERVAL 0.1

class Animation
{
private:
	Timer timer;                                     //计时器用于帧动画更新
	std::function<void()> trigger;                   //动画播放结束后触发的回调函数
	bool isLoop = true;                              //是否循环播放动画

	SDL_Texture* srcSpriteSheetTexture = nullptr;    //使用的源SpriteSheet图片
	std::vector<SDL_Rect> srcSpriteSheetRects;       //裁剪SpriteSheet获取所需的逐帧图片

	size_t frameIdx = 0;                             //动画播放到的帧的索引
	int frameWidth = 0, frameHeight = 0;             //每帧动画纹理的宽高
	SDL_Texture* dstAnimTexture = nullptr;           //每帧动画实际播放的纹理图片
	SDL_Rect* animRect = nullptr;                    //指定纹理图片渲染在窗口上的位置

public:
	Animation();
	~Animation() = default;

	//设置动画素材；即以第四参数传入的索引列表截取传入的源SpriteSheet图片（二三参数位定义了其宽高帧图片个数）中的若干部分矩形纹理，以用作动画各帧
	void SetAnimFrames(SDL_Texture*, int, int, const std::vector<int>&);
	void SetAnimEndTrigger(std::function<void()>);   //设置动画播放结束的回调函数
	void SetLoop(bool);                              //设置动画是否循环播放
	void SetFrameInterval(double);                   //设置每帧动画播放的时间间隔，即定时器的waitTime

	//设置动画播放；即传入渲染器、动画被渲染的位置（Rect的左上顶点）、动画的旋转角度（比如子弹动画会随方向旋转）
	void OnRender(SDL_Renderer*, const SDL_Point&, double);
	void OnUpdate(double);                           //动画的帧更新
	void Restart();                                  //重置动画播放
};

Animation::Animation()
{
	//利用计时器的间隔时间触发来更新每帧动画，所以必须可以多次触发
	timer.SetOneShot(false);

	//设置帧间隔，此处采用默认帧间隔
	this->SetFrameInterval(FRAME_INTERVAL);

	//使用匿名函数设置计时器使用的回调函数（每帧动画结束后都会触发该函数）
	//内部嵌入了每个Animation对象自身的回调函数（仅在动画最后一帧播放后触发该函数）
	timer.SetTimeOutTrigger(
		//[&]表以引用捕获外部变量；括号内是参数列表；花括号是函数体
		[&]()
		{
			//递增帧索引
			frameIdx++;
			//检测帧索引是否超过了最大可索引范围
			if (frameIdx >= srcSpriteSheetRects.size())
			{
				//若循环播放，则返回第一帧
				if (isLoop)
					frameIdx = 0;
				//反之则让其保持在最后一帧
				else
				{
					frameIdx = (srcSpriteSheetRects.size() - 1);
					//如果回调函数存在，就调用该函数
					if (trigger)
						trigger();
				}
			}
		}
	);
}

void Animation::SetAnimFrames(SDL_Texture* _spriteSheet, int _horizontalNum, int _verticalNum, const std::vector<int>& _idxList)
{
	//存储用于裁剪的源SpriteSheet
	srcSpriteSheetTexture = _spriteSheet;
	
	//查询并记录SpriteSheet的宽高
	int _srcWidth, _srcHeight;
	SDL_QueryTexture(srcSpriteSheetTexture, nullptr, nullptr, &_srcWidth, &_srcHeight);

	//计算帧动画图片的宽高
	frameWidth = _srcWidth / _horizontalNum;
	frameHeight = _srcHeight / _verticalNum;

	//通过传入的索引列表，确定用于截取SpriteSheet纹理的SDL_Rect列表
	//先赋予列表以相同的尺寸
	srcSpriteSheetRects.resize(_idxList.size());
	//按索引逐个定义用于切割纹理的矩形
	for (int i = 0; i < _idxList.size(); i++)
	{
		//假如SpriteSheet是3x3的，那么传入2表示第1行第3个，传入4表示第2行第2个
		int _idx = _idxList[i];
		int _x = _idx % _horizontalNum;
		int _y = _idx / _verticalNum;

		//获取裁剪区域的矩形帧纹理图片
		SDL_Rect _rect = {
			//对应每帧图片的左上顶点坐标
			_x * frameWidth,_y * frameHeight,
			//该帧图片的宽高
			frameWidth,frameHeight
		};

		//将获取的矩形赋到列表中去
		srcSpriteSheetRects[i] = _rect;
	}
}

void Animation::SetAnimEndTrigger(std::function<void()> _callBack)
{
	trigger = _callBack;
}

void Animation::SetLoop(bool _loop)
{
	isLoop = _loop;
}

void Animation::SetFrameInterval(double _time)
{
	timer.SetWaitTime(_time);
}

void Animation::OnRender(SDL_Renderer* _renderer, const SDL_Point& _dstPos, double _angle = 0)
{
	//对游戏窗口的裁切矩形，用于确定渲染的目标位置
	//这个Rect是静态的（由于OnRender被频繁调用，所以静态的局部变量有助于减少新建变量产生的开销）
	//任意Animation对象调用此函数时共享同一个Rect，所以我们需要确保实时更新（下面有反例）
	static SDL_Rect _dstPosRect;
	_dstPosRect =
	{
		_dstPos.x, _dstPos.y,
		frameWidth, frameHeight
	};

	//由于是静态的，这种写法只会在初始化的时候被赋值一次，往后都不会被更新，所以会导致怪物动画原地踏步
	//static SDL_Rect _dstPosRect =
	//{
	//	_dstPos.x,_dstPos.y,
	//	frameWidth,frameHeight
	//};
	//std::cout << "_dstPosRect.x/y=(" << _dstPosRect.x << "," << _dstPosRect.y << ")\n";

	//SDL_RenderCopy的参数进阶版：渲染器、源材质、源材质的裁切矩形、目标渲染位置（对主窗口的裁切矩形）、旋转角度、旋转的中心点（默认几何中心）、镜像翻转类型枚举
	SDL_RenderCopyEx(_renderer, srcSpriteSheetTexture, &srcSpriteSheetRects[frameIdx], &_dstPosRect, _angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
}

void Animation::OnUpdate(double _delta)
{
	timer.OnUpdate(_delta);
}
 
void Animation::Restart()
{
	//计时器重新开始计时
	timer.Restart();
	//帧索引归零
	frameIdx = 0;
}

#endif