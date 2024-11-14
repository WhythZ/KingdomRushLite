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

//怪物减速丢失的速度（单位：每格瓦片）
#define SLOW_DOWN_SPEED_LOST 0.5
//怪物被减速的持续时间
#define SLOW_DOWN_DURATION 1

class Enemy
{
public:
	//定义怪物使用技能的回调函数，接收一个技能释放者的指针参数
	typedef std::function<void(Enemy*)> SkillCallBack;

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
	Timer skillRecoverCooldowndTimer;           //恢复技能释放冷却的计时器
	#pragma endregion

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

	#pragma region SkillStats
	double skillRecoverCooldown = 0;            //恢复技能的冷却
	double skillRecoverRange = 0;               //恢复技能的范围
	double skillRecoverIntensity = 0;           //恢复技能的强度
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
	const Vector2& GetSpriteSize() const;       //获取贴图（碰撞箱）尺寸
	const Vector2& GetPosition() const;         //获取怪物位置
	const Vector2& GetTargetPosition() const;   //获取怪物的目标位置
	const Vector2& GetVelocity() const;         //获取怪物速度矢量
	double GetAttackDamage() const;             //获取怪物能对家造成的伤害
	double GetCoinRatio() const;                //获取金币掉率

	double GetSkillRecoverCooldown() const;     //获取恢复技能的冷却
	double GetSkillRecoverRadius() const;       //获取恢复技能的范围半径
	double GetSkillRecoverIntensity() const;    //获取恢复技能的强度

private:
	void RefreshTargetTile();                   //更新目标
};

Enemy::Enemy()
{
	//怪物速度损失时，通过该计时器控制其在计时结束时恢复初始移速，是单次触发
	speedRestoreTimer.SetOneShot(true);
	speedRestoreTimer.SetTimeOutTrigger([&]() {speedCurrent = speedMaximum; });

	#pragma region Animation
	//初始化动画
	animCurrent = &animDown;

	//受击动画触发一次即可
	sketchTimer.SetOneShot(true);
	//设置受击闪白的持续时间，短短的就好
	sketchTimer.SetWaitTime(0.1);
	//设置回调函数
	sketchTimer.SetTimeOutTrigger([&]() {isShowSketch = false; });
	#pragma endregion

	#pragma region Skill
	//恢复技能冷却一结束就会再次释放，所以并非单次触发
	skillRecoverCooldowndTimer.SetOneShot(false);
	//冷却结束就释放恢复技能（技能的释放者为自己），即传入了包装其对应回调函数的匿名函数
	skillRecoverCooldowndTimer.SetTimeOutTrigger([&]() {skillRecoverTrigger(this); });
	#pragma endregion
}

void Enemy::SetRecoverSkillTrigger(Enemy::SkillCallBack _callback)
{
	this->skillRecoverTrigger = _callback;
}

void Enemy::SetRoute(const Route* _route)
{
	//std::cout << "Init Route\n";
	route = _route;
	//更新目标点
	RefreshTargetTile();
}

void Enemy::SetPosition(const Vector2& _position)
{
	position = _position;
}

void Enemy::OnUpdate(double _delta)
{
	//更新当前动画
	animCurrent->OnUpdate(_delta);

	//更新各计时器
	sketchTimer.OnUpdate(_delta);
	skillRecoverCooldowndTimer.OnUpdate(_delta);
	speedRestoreTimer.OnUpdate(_delta);

	#pragma region RefreshAfterMoving
	//计算前一帧（即更新的_delta时间内）过程中怪物期望移动的距离，以及距离下一个目标砖块的距离
	Vector2 _moveDistanceVec = velocity * _delta;
	Vector2 _targetDistanceVec = targetTilePosition - position;
	//当前帧实际移动的距离不应当大于到当前目标点的距离
	position += (_moveDistanceVec < _targetDistanceVec) ? _moveDistanceVec : _targetDistanceVec;

	//如果当前位置距离目标地点的距离长度（在标定尺度下）近似于0，则说明到达了该目标地点，这时就要更新当前位置，并获取新的目标位置
	if (_targetDistanceVec.ApproxZero())
	{
		//std::cout << "Refresh\n";

		//更新目标位置
		targetTileIdx++;
		RefreshTargetTile();
		//更新行进方向
		direction = (targetTilePosition - position).Normalized();
	}

	//更新速度矢量，因为speed的单位是单元格每秒，所以最终速度要乘上单元格的边长
	velocity.x = direction.x * speedCurrent * TILE_SIZE;
	velocity.y = direction.y * speedCurrent * TILE_SIZE;
	#pragma endregion

	#pragma region AnimationChange
	//由于velocity.x与velocity.y由于浮点数的缘故，不能用标准的等于零作判据，所以用比较水平竖直速度大小的方式来确定播放水平动画还是竖直动画
	bool _isShowAnimHorizontal = abs(velocity.x) >= abs(velocity.y);
	
	//是否处于受击状态
	if (!isShowSketch)
	{
		//若播放水平动画，依据水平速度正负判断左右；播放竖直动画亦然
		if (_isShowAnimHorizontal)
			animCurrent = (velocity.x > 0) ? &animRight : &animLeft;
		else
			animCurrent = (velocity.y > 0) ? &animUp : &animDown;
	}
	else
	{
		if (_isShowAnimHorizontal)
			animCurrent = (velocity.x > 0) ? &animRightSketch : &animLeftSketch;
		else
			animCurrent = (velocity.y > 0) ? &animUpSketch : &animDownSketch;
	}
	#pragma endregion
}

void Enemy::OnRender(SDL_Renderer* _renderer)
{
	#pragma region SpriteAnimation
	//怪物贴图的左上角顶点坐标，position是瓦片中心点的世界坐标，注意减去的不是TILE_SIZE/2
	static SDL_Point _point;
	_point.x = (int)(position.x - spriteSize.x / 2);
	_point.y = (int)(position.y - spriteSize.y / 2);

	//调用当前动画的渲染
	animCurrent->OnRender(_renderer, _point);
	#pragma endregion

	#pragma region HealthBar
	//血条的定位Rect
	static SDL_Rect _healthBarRect;
	//血条的常量尺寸大小
	static const Vector2 _healthBarSize = { 40,8 };
	//血条偏移贴图正上方的偏移量（为了让血条不要紧贴着怪物贴图的顶边）
	static const float _healthBarVerticalOffset = 3;
	//血条的内容与边框的颜色，R-G-B-Alpha
	static const SDL_Color _healthBarColorContent = { 225,255,195,255 };
	static const SDL_Color _healthBarColorBorder = { 115,185,125,255 };

	//当怪物生命值不满时，绘制血条
	if (healthCurrent < healthMaximum)
	{
		//让血条的中心点水平上对齐怪物贴图的中心
		_healthBarRect.x = (int)(position.x - _healthBarSize.x / 2);
		//竖直上在怪物贴图的正上方，注意是减法（坐标轴y轴正方向朝下的缘故）
		_healthBarRect.y = (int)(position.y - spriteSize.y / 2 - _healthBarSize.y - _healthBarVerticalOffset);
		//宽度表示生命条的长度
		_healthBarRect.w = (int)(_healthBarSize.x * (healthCurrent / healthMaximum));
		//高度不变
		_healthBarRect.h = (int)(_healthBarSize.y);

		//设置血条内容绘制颜色
		SDL_SetRenderDrawColor(_renderer, _healthBarColorContent.r, _healthBarColorContent.g, _healthBarColorContent.b, _healthBarColorContent.a);
		//以上述颜色绘制血条内容
		SDL_RenderFillRect(_renderer, &_healthBarRect);

		//为了绘制血条边框，将宽度变回原来的宽度
		_healthBarRect.w = (int)(_healthBarSize.x);
		SDL_SetRenderDrawColor(_renderer, _healthBarColorBorder.r, _healthBarColorBorder.g, _healthBarColorBorder.b, _healthBarColorBorder.a);
		SDL_RenderFillRect(_renderer, &_healthBarRect);
	}
	#pragma endregion
}

void Enemy::IncreaseHealthBy(double _incre)
{
	//增加生命值且不超过上限
	healthCurrent = ((healthCurrent + _incre) < healthMaximum) ? (healthCurrent + _incre) : healthMaximum;
}

void Enemy::DecreaseHealthBy(double _decre)
{
	//减少生命值且不低于零
	healthCurrent = ((healthCurrent - _decre) > 0) ? (healthCurrent - _decre) : 0;
	//判断是否死亡
	if (healthCurrent <= 0)
		isAlive = false;

	//每次受到攻击都要播放受击剪影闪白动画（重置闪白计时器）
	isShowSketch = true;
	sketchTimer.Restart();
}

void Enemy::SlowDown()
{
	//注意不是speedCurrent -= SLOW_DOWN_SPEED_LOST，后者会导致速度损失直到为零，这是不对的
	speedCurrent = speedMaximum - SLOW_DOWN_SPEED_LOST;
	//经过一定时长后恢复初始最大移速
	speedRestoreTimer.SetWaitTime(SLOW_DOWN_DURATION);
	speedRestoreTimer.Restart();
}

void Enemy::Kill()
{
	//当敌人触碰到家的时候，即使血量未清零，也要处死（无效化）怪物
	isAlive = false;
}

bool Enemy::IsAlive() const
{
	return isAlive;
}

double Enemy::GetRouteProcess() const
{
	//存储路径长度
	size_t _routeSize = route->GetTilePointList().size();

	//返回路径完成进度
	return (double)(targetTileIdx / _routeSize);
}

double Enemy::GetHealth() const
{
	return healthCurrent;
}

const Vector2& Enemy::GetSpriteSize() const
{
	//比如碰撞检测的时候，就需要获取碰撞箱尺寸（此处简化其为贴图尺寸）
	return spriteSize;
}

const Vector2& Enemy::GetPosition() const
{
	return position;
}

const Vector2& Enemy::GetTargetPosition() const
{
	return targetTilePosition;
}

const Vector2& Enemy::GetVelocity() const
{
	return velocity;
}

double Enemy::GetAttackDamage() const
{
	return attackDamage;
}

double Enemy::GetCoinRatio() const
{
	return coinRatio;
}

double Enemy::GetSkillRecoverCooldown() const
{
	return skillRecoverCooldown;
}

double Enemy::GetSkillRecoverRadius() const
{
	//配置文件中关于长度的数据的单位均以[每单元格]为单位，所以要乘上瓦片长度
	return skillRecoverRange * TILE_SIZE;
}

double Enemy::GetSkillRecoverIntensity() const
{
	return skillRecoverIntensity;
}

void Enemy::RefreshTargetTile()
{
	//获取路径二维瓦片坐标列表
	const Route::TilePointList& _tilePointList = route->GetTilePointList();
	//std::cout << "_tilePointList.size()=" << _tilePointList.size() << "\n";

	//如果当前的目标点没有超出路径上瓦片的总数，则可以寻找下一个目标点的在屏幕上的位置
	if (targetTileIdx < _tilePointList.size())
	{
		//获取当前与目标瓦片的二维SDL_Point坐标，这与Vector2不同，前者是离散的（乘上TILE_SIZE才能表示实际距离）点坐标，后者是连续的具体距离向量
		const SDL_Point& _targetTilePoint = _tilePointList[targetTileIdx];

		//std::cout << "TargetTilePoint=" << "(" << _targetTilePoint.x << "," << _targetTilePoint.y << ")\n";

		//获取静态的整个瓦片地图渲染在游戏窗口中的位置Rect，用于定位路径上的目标点相对游戏窗口的位置
		static const SDL_Rect& _mapRect = ConfigManager::GetInstance()->mapRect;
		//从地图左上角开始，依靠二维瓦片点的离散坐标寻找到（路径上的）目标瓦片左上顶点的具体坐标，再加上半个TILE_SIZE得到目标瓦片中心在游戏窗口上的坐标
		targetTilePosition.x = _mapRect.x + (_targetTilePoint.x * TILE_SIZE) + TILE_SIZE / 2;
		targetTilePosition.y = _mapRect.y + (_targetTilePoint.y * TILE_SIZE) + TILE_SIZE / 2;

		//std::cout << "TargetTilePosition=" << targetTilePosition << ")\n";
	}
}

#endif
