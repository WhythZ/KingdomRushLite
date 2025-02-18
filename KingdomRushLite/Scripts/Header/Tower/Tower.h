#ifndef _TOWER_H_
#define _TOWER_H_

#include "TowerType.h"
#include "../Math/Vector2.h"
#include "../Math/Timer.h"
#include "../Math/FacingDir.h"
#include "../Animation/Animation.h"
#include "../Bullet/BulletType.h"
#include "../Enemy/Enemy.h"

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

	#pragma region Properties
	double fireCooldown = 0;                   //每次发射的间隔
	double fireRadius = 0;                     //攻击范围半径
	BulletType bulletType = BulletType::None;  //子弹的类型
	double bulletDamage = 0;                   //子弹的伤害
	double bulletSpeed = 0;                    //子弹的速度
	#pragma endregion

private:
	Vector2 position;                          //防御塔所在位置
	
	Animation* animCurrent = &animIdleDown;    //防御塔默认动画
	FacingDir facingDir = FacingDir::Down;     //防御塔默认动画的朝向

	bool canFire = true;                       //决定是否可以发射子弹
	Timer fireTimer;                           //控制子弹发射间隔的定时器

public:
	Tower();
	~Tower() = default;
	void SetPosition(const Vector2&);          //设置防御塔的位置

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	const Vector2& GetSize() const;            //获取防御塔的尺寸
	const Vector2& GetPosition() const;        //获取防御塔的位置
		
protected:
	void virtual OnFireBullet();               //子弹发射时的逻辑

private:
	void UpdateFacingDir(const Vector2&);      //更新防御塔朝向
	void UpdateIdleAnim();                     //依照朝向更新当前动画为对应闲置动画
	void UpdateFireAnim();                     //依照朝向更新当前动画为对应开火动画
	Enemy* FindTargetEnemy();                  //子弹索敌的逻辑
};

#endif
