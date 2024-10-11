#ifndef _ENEMY_HPP_
#define _ENEMY_HPP_

#include <functional>
#include "../Animation/Animation.hpp"
#include "../Vector/Vector2.hpp"
#include "../Timer/Timer.hpp"
#include "../Map/Route.hpp"
#include "EnemyType.hpp"
#include "../Manager/ConfigManager.hpp"
#include "../Manager/ResourceManager.hpp"

class Enemy
{
public:
	//定义怪物使用技能的回调函数，接收一个技能释放者的指针参数
	typedef std::function<void(Enemy*)> SkillCallBack;

private:
	Vector2 position;                        //怪物位置
	Vector2 velocity;                        //速度矢量
	Vector2 direction;                       //行进朝向

	bool isAlive = true;                     //记录怪物是否存活

	#pragma region Animation
	Animation* animCurrent;                  //指向当前调用的动画
	bool isShowSketch;                       //是否处于受击变白（即绘制剪影）的状态
	Timer sketchTimer;                       //管理受击变白特效持续时长
	#pragma endregion

	#pragma region Route
	const Route* route = nullptr;            //该怪物行进的瓦片路径
	int currentTileIdx = 0;                  //当前怪物处于路径上的瓦片的索引
	Vector2 targetTilePosition;              //将要驶入的下一个瓦片的坐标位置
	#pragma endregion

	#pragma region Skill
	SkillCallBack skillRecoverTrigger;       //释放恢复技能的回调函数
	Timer skillRecoverCooldowndTimer;        //恢复技能释放冷却的计时器
	Timer skillRecoverFinishedTimer;         //控制释放恢复技能需要耗费多长时间
	#pragma endregion

protected:
	EnemyType type;                          //敌人的种类

	#pragma region Animation
	Vector2 size;                            //敌人图片的二维尺寸大小
	Animation animUp;                        //朝上方向的动画
	Animation animDown;                      //朝下方向的动画
	Animation animLeft;                      //朝左方向的动画
	Animation animRight;                     //朝右方向的动画
	Animation animUpSketch;                  //朝上方向的受击剪影动画
	Animation animDownSketch;                //朝下方向的受击剪影动画
	Animation animLeftSketch;                //朝左方向的受击剪影动画
	Animation animRightSketch;               //朝右方向的受击剪影动画
	#pragma endregion
	
	#pragma region BasicStats
	double healthMaximum = 0;                //最大生命值（读取配置文件）
	double healthCurrent = 0;                //当前生命值（动态进行记录）

	double speedMaximum = 0;                 //最大移动速率标量（读取配置文件）
	double speedCurrent = 0;                 //当前移动速率标量（动态进行记录）

	double damage = 0;                       //敌人可以对防守单位造成的伤害

	double rewardRatio = 0;                  //奖励掉落概率
	#pragma endregion

	#pragma region SkillStats
	double skillRecoverCooldown = 0;         //恢复技能的冷却
	double skillRecoverRange = 0;            //恢复技能的范围
	double skillRecoverIntensity = 0;        //恢复技能的强度
	#pragma endregion

public:
	Enemy();
	~Enemy() = default;

	void OnUpdate(double);                   //帧更新函数

private:
	void UpdateTargetTilePosition();         //更新目标瓦片的位置
};

Enemy::Enemy()
{
	#pragma region Animation
	//受击动画触发一次即可
	sketchTimer.SetOneShot(true);
	//设置受击闪白的持续时间，短短的就好
	sketchTimer.SetWaitTime(0.1);
	//设置回调函数
	sketchTimer.SetTimeOutTrigger(
		[&]()
		{
			isShowSketch = false;
		}
	);
	#pragma endregion
	
	#pragma region Skill
	//恢复技能冷却一结束就会再次释放，所以并非单次触发
	skillRecoverCooldowndTimer.SetOneShot(false);
	//冷却结束就释放恢复技能，即传入了包装其对应回调函数的匿名函数
	skillRecoverCooldowndTimer.SetTimeOutTrigger(
		[&]()
		{
			//技能的释放者为自己
			skillRecoverTrigger(this);
		}
	);

	//每当治愈技能被触发后，释放该技能的怪物会减速或停止，通过该计时器控制其何时恢复初始速度，所以是单次触发
	skillRecoverFinishedTimer.SetOneShot(true);
	skillRecoverFinishedTimer.SetTimeOutTrigger(
		[&]()
		{
			speedCurrent = speedMaximum;
		}
	);
	#pragma endregion
}

void Enemy::OnUpdate(double _delta)
{
	//更新当前动画
	animCurrent->OnUpdate(_delta);

	//更新各计时器
	sketchTimer.OnUpdate(_delta);
	skillRecoverCooldowndTimer.OnUpdate(_delta);
	skillRecoverFinishedTimer.OnUpdate(_delta);

	#pragma region MoveOnDistance
	//计算该帧（即更新的_delta时间内）过程中怪物期望移动的距离
	Vector2 _moveDistance = velocity * _delta;
	//计算距离下一个目标砖块的距离，即目标位置与当前位置的差
	Vector2 _targetDistance = targetTilePosition - position;
	//这一帧实际移动的距离不应当大于到当前目标点的距离（若速度较小则可能出现小于的情况，这无妨，经过多次帧更新后总能到达目标处的）
	position += (_moveDistance < _targetDistance) ? _moveDistance : _targetDistance;
	//如果当前位置距离目标地点的距离长度（在标定尺度下）近似于0，则说明到达了该目标地点，这时要更新目标位置
	if (_targetDistance.ApproxZero())
		UpdateTargetTilePosition();
	#pragma endregion
}

void Enemy::UpdateTargetTilePosition()
{
	//获取路径二维瓦片坐标列表
	const Route::TilePointList& _tilePointList = route->GetTilePointList();
	
	//目标瓦片的索引就是当前所在瓦片索引的下一个
	int _targetTileIdx = currentTileIdx + 1;

	//如果当前没有超出路径上瓦片的总数，则可以寻找下一个目标点的在屏幕上的位置
	if (_targetTileIdx < _tilePointList.size())
	{
		//获取目标瓦片的二维SDL_Point坐标，这与Vector2不同，前者是离散的（乘上TILE_SIZE才能表示实际距离）点坐标，后者是连续的具体距离向量
		const SDL_Point& _targetTilePoint = _tilePointList[_targetTileIdx];

		//获取静态的整个瓦片地图渲染在游戏窗口中的位置Rect，用于定位路径上的目标点相对游戏窗口的位置
		static const SDL_Rect& _mapRect = ConfigManager::GetInstance()->mapRect;

		//从地图左上角开始，依靠二维瓦片点的离散坐标寻找到（路径上的）目标瓦片左上顶点的具体坐标，再加上半个TILE_SIZE得到目标瓦片中心在游戏窗口上的坐标
		position.x = _mapRect.x + (_targetTilePoint.x * TILE_SIZE) + TILE_SIZE / 2;
		position.y = _mapRect.y + (_targetTilePoint.y * TILE_SIZE) + TILE_SIZE / 2;


	}
}

#endif
