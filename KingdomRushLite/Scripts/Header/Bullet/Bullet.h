#ifndef _BULLET_H_
#define _BULLET_H_

#include "BulletType.h"
#include "../Math/Vector2.h"
#include "../Animation/Animation.h"
#include "../Enemy/Enemy.h"

class Bullet
{
protected:
	BulletType type = BulletType::None;     //子弹的类型

	#pragma region Transform
	Vector2 size;                           //子弹的尺寸
	Vector2 position;                       //子弹的位置
	#pragma endregion

	#pragma region Properties
	Vector2 velocity;                       //子弹运动的速度
	double damage = 0;                      //子弹能对敌人造成的伤害
	double damageRadius = -1;               //子弹造成伤害的AOE范围半径
	#pragma endregion

	#pragma region Animation
	Animation animFlying;                   //子弹飞行时的动画
	bool canRotate = false;                 //子弹是否可旋转对动画有影响
	#pragma endregion

private:
	bool isValid = true;                    //决定子弹是否应当继续存在
	bool canCollide = true;                 //子弹是否可以被碰撞（伤害检定仅发生在碰撞那一帧，后续播放碰撞后动画时的帧不应继续检定）
	double rotationAngle = 0;               //描述子弹瞄准方向的欧拉角度，用于动画的处理

public:
	Bullet() = default;
	~Bullet() = default;
	void SetPosition(const Vector2&);       //设定子弹的发射位置
	void SetVelocity(const Vector2&);       //设定子弹发射的速度向量
	void SetDamage(double);                 //设定子弹命中敌人的伤害

	virtual void OnUpdate(double);
	virtual void OnRender(SDL_Renderer*);
	virtual void OnCollide(Enemy*);         //子弹碰撞检测的函数

	void DisableCollide();                  //取消子弹的可碰撞，以造成瞬时伤害而非持续伤害
	void Invalidate();                      //使得子弹消失

	const Vector2& GetSize() const;         //获取子弹尺寸，用于命中碰撞检测
	const Vector2& GetPosition() const;     //获取子弹位置
	double GetDamage() const;               //获取子弹伤害
	double GetDamageRadius() const;         //获取子弹伤害范围，用于伤害碰撞检测
	bool CanCollide() const;                //查询子弹的可碰撞与否
	bool IsValid() const;                   //查询子弹的失效与否
};

#endif
