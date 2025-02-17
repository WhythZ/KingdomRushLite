#ifndef _TOWER_HPP_
#define _TOWER_HPP_

#include "TowerType.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Timer.hpp"
#include "../Math/FacingDir.hpp"
#include "../Animation/Animation.hpp"
#include "../Manager/Concrete/BulletManager.hpp"

class Tower
{
protected:
	TowerType type = TowerType::None;          //防御塔的种类

	#pragma region Transform
	Vector2 size;                              //用于渲染与碰撞的尺寸
	#pragma endregion

	#pragma region Animation
	Animation animIdleUp;              //待机朝上的动画
	Animation animIdleDown;            //待机朝下的动画
	Animation animIdleLeft;            //待机朝左的动画
	Animation animIdleRight;           //待机朝右的动画

	Animation animFireUp;              //攻击朝上的动画
	Animation animFireDown;            //攻击朝下的动画
	Animation animFireLeft;            //攻击朝左的动画
	Animation animFireRight;           //攻击朝右的动画
	#pragma endregion

	#pragma region FireProperties
	BulletType bulletType = BulletType::None;  //发射的子弹类型
	float fireSpeed = 0;                       //发射子弹的速度
	#pragma endregion

private:
	Vector2 position;                          //防御塔所在位置
	
	Animation* animCurrent = &animIdleDown;    //防御塔默认动画
	FacingDir direction = FacingDir::Down;     //防御塔默认动画的朝向

	Timer fireTimer;                           //控制子弹发射间隔的定时器
	bool canFire = true;                       //决定是否可以发射子弹

public:
	Tower();
	~Tower() = default;
	void SetPosition(const Vector2&);          //设置防御塔的位置

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	const Vector2& GetSize() const;            //获取防御塔的尺寸
	const Vector2& GetPosition() const;        //获取防御塔的位置

private:
	void UpdateFacingDir();                    //依照朝向更新当前动画
	void OnFire();                             //发射子弹的逻辑
};

Tower::Tower()
{
	#pragma region Animation
	//初始化动画的属性（纹理在子类中设置）
	animIdleUp.SetLoop(true);
	animIdleDown.SetLoop(true);
	animIdleLeft.SetLoop(true);
	animIdleRight.SetLoop(true);
	animFireUp.SetLoop(false);
	animFireDown.SetLoop(false);
	animFireLeft.SetLoop(false);
	animFireRight.SetLoop(false);

	//只有在防御塔完成一次攻击后，才能更新防御塔朝向
	animFireUp.SetOnAnimFinished(
		[&]()
		{
			UpdateFacingDir();
		}
	);
	animFireDown.SetOnAnimFinished(
		[&]()
		{
			UpdateFacingDir();
		}
	);
	animFireLeft.SetOnAnimFinished(
		[&]()
		{
			UpdateFacingDir();
		}
	);
	animFireRight.SetOnAnimFinished(
		[&]()
		{
			UpdateFacingDir();
		}
	);
	#pragma endregion

	//初始化开火定时器
	fireTimer.SetTimeOutTrigger(
		[&]()
		{
			//恢复到可发射子弹的状态
			canFire = true;
		}
	);
}

void Tower::SetPosition(const Vector2& _position)
{
	position = _position;
}

void Tower::OnUpdate(double _delta)
{
	fireTimer.OnUpdate(_delta);
	animCurrent->OnUpdate(_delta);

	//如果可以开火则调用开火
	if (canFire)
		OnFire();
}

void Tower::OnRender(SDL_Renderer* _renderer)
{
	//防御塔贴图的左上角顶点坐标
	static SDL_Point _point;
	_point.x = (int)(position.x - size.x / 2);
	_point.y = (int)(position.y - size.y / 2);
	//更新当前动画
	animCurrent->OnRender(_renderer, _point);
}

const Vector2& Tower::GetSize() const
{
	return size;
}

const Vector2& Tower::GetPosition() const
{
	return position;
}

void Tower::UpdateFacingDir()
{
	//此处使用对应方向的闲置状态动画进行更新
	switch (direction)
	{
	case FacingDir::Up:
		animCurrent = &animIdleUp;
		break;
	case FacingDir::Down:
		animCurrent = &animIdleDown;
		break;
	case FacingDir::Left:
		animCurrent = &animIdleLeft;
		break;
	case FacingDir::Right:
		animCurrent = &animIdleRight;
		break;
	default:
		break;
	}
}

void Tower::OnFire()
{

}

#endif
