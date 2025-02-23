#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <functional>
#include "../Infra/Animation.h"
#include "../Infra/Vector2.h"
#include "../Map/Route.h"
#include "EnemyType.h"

//怪物减速丢失的速度（单位：每格瓦片）
#define SLOW_DOWN_SPEED_LOST 0.3
//怪物被减速的持续时间
#define SLOW_DOWN_DURATION 1

class Enemy
{
public:
	//定义怪物使用技能的回调函数，接收一个技能释放者的指针参数
	typedef std::function<void(Enemy*)> SkillCallBack;

protected:
	EnemyType type = EnemyType::None;           //敌人的种类

	#pragma region Animation
	Vector2 spriteSize;                         //敌人图片的二维尺寸大小
	Animation animUp;                           //朝上方向的动画
	Animation animDown;                         //朝下方向的动画
	Animation animLeft;                         //朝左方向的动画
	Animation animRight;                        //朝右方向的动画
	Animation animUpSketch;                     //朝上方向的受击剪影动画
	Animation animDownSketch;                   //朝下方向的受击剪影动画
	Animation animLeftSketch;                   //朝左方向的受击剪影动画
	Animation animRightSketch;                  //朝右方向的受击剪影动画
	#pragma endregion

	#pragma region BasicStats
	double healthMaximum = 0;                   //最大生命值（读取配置文件）
	double healthCurrent = 0;                   //当前生命值（动态进行记录）

	double speedMaximum = 0;                    //最大移动速率标量（读取配置文件）
	double speedCurrent = 0;                    //当前移动速率标量（动态进行记录）

	double attackDamage = 0;                    //敌人可以对防守单位（家）造成的伤害

	double coinRatio = 0;                       //奖励掉落概率
	#pragma endregion

	#pragma region Skill
	double skillRecoverCooldown = 0;            //恢复技能的冷却
	double skillRecoverRadius = 0;              //恢复技能的范围半径
	double skillRecoverIntensity = 0;           //恢复技能的强度

	Timer skillRecoverCooldowndTimer;           //恢复技能释放冷却的计时器
	#pragma endregion

private:
	Vector2 position;                           //怪物位置
	Vector2 velocity;                           //速度矢量
	Vector2 direction;                          //行进朝向

	bool isAlive = true;                        //记录怪物是否存活

	Timer speedRestoreTimer;                    //控制怪物恢复初始移速

	#pragma region Animation
	Animation* animCurrent;                     //指向当前调用的动画
	bool isShowSketch = false;                  //是否处于受击变白（即绘制剪影）的状态
	Timer sketchTimer;                          //管理受击变白特效持续时长
	#pragma endregion

	#pragma region Route
	const Route* route = nullptr;               //该怪物行进的瓦片路径
	size_t targetTileIdx = 0;                   //怪物的目标瓦片索引
	Vector2 targetTilePosition;                 //将要驶入的下一个瓦片的坐标位置
	#pragma endregion

	#pragma region Skill
	SkillCallBack skillRecoverTrigger;          //释放恢复技能的回调函数
	#pragma endregion

public:
	Enemy();
	~Enemy() = default;
	void SetRecoverSkillTrigger(SkillCallBack); //设置回复技能的回调函数
	void SetRoute(const Route*);                //设置行进路径
	void SetPosition(const Vector2&);           //设置初始位置

	void OnUpdate(double);                      //帧更新函数
	void OnRender(SDL_Renderer*);               //渲染绘图更新

	void IncreaseHealthBy(double);              //增加生命值
	void DecreaseHealthBy(double);              //减少生命值
	void SlowDown();                            //移速减缓
	void Kill();                                //无效化该怪物

	bool IsAlive() const;                       //获取怪物存活状态
	double GetRouteProcess() const;             //获取怪物行进状况

	double GetHealth() const;                   //获取当前血量
	const Vector2& GetSize() const;             //获取贴图（碰撞箱）尺寸
	const Vector2& GetPosition() const;         //获取怪物位置
	const Vector2& GetTargetPosition() const;   //获取怪物的目标位置
	const Vector2& GetVelocity() const;         //获取怪物速度矢量
	double GetAttackDamage() const;             //获取怪物能对家造成的伤害

	double GetSkillRecoverCooldown() const;     //获取恢复技能的冷却
	double GetSkillRecoverRadius() const;       //获取恢复技能的范围半径
	double GetSkillRecoverIntensity() const;    //获取恢复技能的强度

private:
	void RefreshTargetTile();                   //更新目标
};

#endif
