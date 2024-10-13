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

//������ٶ�ʧ���ٶȣ���λ��ÿ����Ƭ��
#define SLOW_DOWN_SPEED_LOST 0.5
//���ﱻ���ٵĳ���ʱ��
#define SLOW_DOWN_DURATION 1

class Enemy
{
public:
	//�������ʹ�ü��ܵĻص�����������һ�������ͷ��ߵ�ָ�����
	typedef std::function<void(Enemy*)> SkillCallBack;

private:
	Vector2 position;                           //����λ��
	Vector2 velocity;                           //�ٶ�ʸ��
	Vector2 direction;                          //�н�����

	bool isAlive = true;                        //��¼�����Ƿ���

	Timer speedRestoreTimer;                    //���ƹ���ָ���ʼ����

	#pragma region Animation
	Animation* animCurrent;                     //ָ��ǰ���õĶ���
	bool isShowSketch;                          //�Ƿ����ܻ���ף������Ƽ�Ӱ����״̬
	Timer sketchTimer;                          //�����ܻ������Ч����ʱ��
	#pragma endregion

	#pragma region Route
	const Route* route = nullptr;               //�ù����н�����Ƭ·��
	int currentTileIdx = 0;                     //��ǰ���ﴦ��·���ϵ���Ƭ������
	Vector2 targetTilePosition;                 //��Ҫʻ�����һ����Ƭ������λ��
	#pragma endregion

	#pragma region Skill
	SkillCallBack skillRecoverTrigger;          //�ͷŻָ����ܵĻص�����
	Timer skillRecoverCooldowndTimer;           //�ָ������ͷ���ȴ�ļ�ʱ��
	#pragma endregion

protected:
	EnemyType type;                             //���˵�����

	#pragma region Animation
	Vector2 spriteSize;                         //����ͼƬ�Ķ�ά�ߴ��С
	Animation animUp;                           //���Ϸ���Ķ���
	Animation animDown;                         //���·���Ķ���
	Animation animLeft;                         //������Ķ���
	Animation animRight;                        //���ҷ���Ķ���
	Animation animUpSketch;                     //���Ϸ�����ܻ���Ӱ����
	Animation animDownSketch;                   //���·�����ܻ���Ӱ����
	Animation animLeftSketch;                   //��������ܻ���Ӱ����
	Animation animRightSketch;                  //���ҷ�����ܻ���Ӱ����
	#pragma endregion
	
	#pragma region BasicStats
	double healthMaximum = 0;                   //�������ֵ����ȡ�����ļ���
	double healthCurrent = 0;                   //��ǰ����ֵ����̬���м�¼��

	double speedMaximum = 0;                    //����ƶ����ʱ�������ȡ�����ļ���
	double speedCurrent = 0;                    //��ǰ�ƶ����ʱ�������̬���м�¼��

	double attackDamage = 0;                    //���˿��ԶԷ��ص�λ���ң���ɵ��˺�

	double coinRatio = 0;                       //�����������
	#pragma endregion

	#pragma region SkillStats
	double skillRecoverCooldown = 0;            //�ָ����ܵ���ȴ
	double skillRecoverRange = 0;               //�ָ����ܵķ�Χ
	double skillRecoverIntensity = 0;           //�ָ����ܵ�ǿ��
	#pragma endregion

public:
	Enemy();
	~Enemy() = default;
	void SetSkillTrigger(SkillCallBack);        //���ü��ܻص�����
	void SetRoute(const Route*);                //�����н�·��
	void SetPosition(const Vector2&);           //���ó�ʼλ��

	void OnUpdate(double);                      //֡���º���
	void OnRender(SDL_Renderer*);               //��Ⱦ��ͼ����

	void IncreaseHealthBy(double);              //��������ֵ
	void DecreaseHealthBy(double);              //��������ֵ
	void SlowDown();                            //���ټ���
	void Kill();                                //��Ч���ù���

	bool IsAlive() const;                       //��ȡ������״̬
	double GetRouteProcess() const;             //��ȡ�����н�״��

	double GetHealth() const;                   //��ȡ��ǰѪ��
	const Vector2& GetSpriteSize() const;       //��ȡ��ͼ����ײ�䣩�ߴ�
	const Vector2& GetPosition() const;         //��ȡ����λ��
	const Vector2& GetVelocity() const;         //��ȡ�����ٶ�ʸ��
	double GetAttackDamage() const;             //��ȡ�����ܶԼ���ɵ��˺�
	double GetCoinRatio() const;                //��ȡ��ҵ���

	double GetSkillRecoverCooldown() const;     //��ȡ�ָ����ܵ���ȴ
	double GetSkillRecoverRange() const;        //��ȡ�ָ����ܵķ�Χ�뾶
	double GetSkillRecoverIntensity() const;    //��ȡ�ָ����ܵ�ǿ��

private:
	void RefreshTargetTilePosition();           //����Ŀ����Ƭ��λ��
};

Enemy::Enemy()
{
	//�����ٶ���ʧʱ��ͨ���ü�ʱ���������ڼ�ʱ����ʱ�ָ���ʼ���٣��ǵ��δ���
	speedRestoreTimer.SetOneShot(true);
	speedRestoreTimer.SetTimeOutTrigger(
		[&]()
		{
			speedCurrent = speedMaximum;
		}
	);

	#pragma region Animation
	//�ܻ���������һ�μ���
	sketchTimer.SetOneShot(true);
	//�����ܻ����׵ĳ���ʱ�䣬�̶̵ľͺ�
	sketchTimer.SetWaitTime(0.1);
	//���ûص�����
	sketchTimer.SetTimeOutTrigger(
		[&]()
		{
			isShowSketch = false;
		}
	);
	#pragma endregion

	#pragma region Skill
	//�ָ�������ȴһ�����ͻ��ٴ��ͷţ����Բ��ǵ��δ���
	skillRecoverCooldowndTimer.SetOneShot(false);
	//��ȴ�������ͷŻָ����ܣ��������˰�װ���Ӧ�ص���������������
	skillRecoverCooldowndTimer.SetTimeOutTrigger(
		[&]()
		{
			//���ܵ��ͷ���Ϊ�Լ�
			skillRecoverTrigger(this);
		}
	);
	#pragma endregion
}

void Enemy::SetSkillTrigger(Enemy::SkillCallBack _callback)
{
	this->skillRecoverTrigger = _callback;
}

void Enemy::SetRoute(const Route* _route)
{
	route = _route;
	//����Ŀ���
	RefreshTargetTilePosition();
}

void Enemy::SetPosition(const Vector2& _position)
{
	position = _position;
}

void Enemy::OnUpdate(double _delta)
{
	//���µ�ǰ����
	animCurrent->OnUpdate(_delta);

	//���¸���ʱ��
	sketchTimer.OnUpdate(_delta);
	skillRecoverCooldowndTimer.OnUpdate(_delta);
	speedRestoreTimer.OnUpdate(_delta);

	#pragma region MoveOnDistance
	//�����֡�������µ�_deltaʱ���ڣ������й��������ƶ��ľ���
	Vector2 _moveDistance = velocity * _delta;
	//���������һ��Ŀ��ש��ľ��룬��Ŀ��λ���뵱ǰλ�õĲ�
	Vector2 _targetDistance = targetTilePosition - position;
	//��һ֡ʵ���ƶ��ľ��벻Ӧ�����ڵ���ǰĿ���ľ��루���ٶȽ�С����ܳ���С�ڵ���������޷����������֡���º����ܵ���Ŀ�괦�ģ�
	position += (_moveDistance < _targetDistance) ? _moveDistance : _targetDistance;
	#pragma endregion

	#pragma region RefreshEnemyState
	//�����ǰλ�þ���Ŀ��ص�ľ��볤�ȣ��ڱ궨�߶��£�������0����˵�������˸�Ŀ��ص㣬��ʱ��Ҫ���µ�ǰλ�ã�����ȡ�µ�Ŀ��λ��
	if (_targetDistance.ApproxZero())
	{
		//����λ����Ŀ��
		currentTileIdx++;
		RefreshTargetTilePosition();

		//�µķ���Ϊһ����λ�������ӵ�ǰλ��ָ��Ŀ��λ��
		direction = (targetTilePosition - position).Normalized();
	}

	//�����ٶ�ʸ������Ϊspeed�ĵ�λ�ǵ�Ԫ��ÿ�룬���������ٶ�Ҫ���ϵ�Ԫ��ı߳�
	velocity.x = direction.x * speedCurrent * TILE_SIZE;
	velocity.y = direction.y * speedCurrent * TILE_SIZE;
	#pragma endregion

	#pragma region Animation
	//����velocity.x��velocity.y���ڸ�������Ե�ʣ������ñ�׼�ĵ��������оݣ������ñȽ�ˮƽ��ֱ�ٶȴ�С�ķ�ʽ��ȷ������ˮƽ����������ֱ����
	bool _isShowAnimHorizontal = abs(velocity.x) >= abs(velocity.y);
	
	//�Ƿ����ܻ�״̬
	if (!isShowSketch)
	{
		//������ˮƽ����������ˮƽ�ٶ������ж����ң�������ֱ������Ȼ
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
	//������ͼ�����ϽǶ�������
	static SDL_Point rectPoint;
	rectPoint.x = (int)(position.x - spriteSize.x / 2);
	rectPoint.y = (int)(position.y - spriteSize.y / 2);

	//���õ�ǰ��������Ⱦ
	animCurrent->OnRender(_renderer, rectPoint);
	#pragma endregion

	#pragma region HealthBar
	//Ѫ���Ķ�λRect
	static SDL_Rect healthBarRect;
	//Ѫ���ĳ����ߴ��С
	static const Vector2 healthBarSize = { 40,8 };
	//Ѫ��ƫ����ͼ���Ϸ���ƫ������Ϊ����Ѫ����Ҫ�����Ź�����ͼ�Ķ��ߣ�
	static const float healthBarVerticalOffset = 3;
	//Ѫ����������߿����ɫ��R-G-B-Alpha
	static const SDL_Color healthBarColorContent = { 225,255,195,255 };
	static const SDL_Color healthBarColorBorder = { 115,185,125,255 };

	//����������ֵ����ʱ������Ѫ��
	if (healthCurrent < healthMaximum)
	{
		//��Ѫ�������ĵ�ˮƽ�϶��������ͼ������
		healthBarRect.x = (int)(position.x - healthBarSize.x / 2);
		//��ֱ���ڹ�����ͼ�����Ϸ���ע���Ǽ�����������y���������µ�Ե�ʣ�
		healthBarRect.y = (int)(position.y - spriteSize.y / 2 - healthBarSize.y - healthBarVerticalOffset);
		//��ȱ�ʾ�������ĳ���
		healthBarRect.w = (int)(healthBarSize.x * (healthCurrent / healthMaximum));
		//�߶Ȳ���
		healthBarRect.h = (int)(healthBarSize.y);

		//����Ѫ�����ݻ�����ɫ
		SDL_SetRenderDrawColor(_renderer, healthBarColorContent.r, healthBarColorContent.g, healthBarColorContent.b, healthBarColorContent.a);
		//��������ɫ����Ѫ������
		SDL_RenderFillRect(_renderer, &healthBarRect);

		//Ϊ�˻���Ѫ���߿򣬽���ȱ��ԭ���Ŀ��
		healthBarRect.w = (int)(healthBarSize.x);
		SDL_SetRenderDrawColor(_renderer, healthBarColorBorder.r, healthBarColorBorder.g, healthBarColorBorder.b, healthBarColorBorder.a);
		SDL_RenderFillRect(_renderer, &healthBarRect);
	}
	#pragma endregion
}

void Enemy::IncreaseHealthBy(double _incre)
{
	//��������ֵ�Ҳ���������
	healthCurrent = ((healthCurrent + _incre) < healthMaximum) ? (healthCurrent + _incre) : healthMaximum;
}

void Enemy::DecreaseHealthBy(double _decre)
{
	//��������ֵ�Ҳ�������
	healthCurrent = ((healthCurrent - _decre) > 0) ? (healthCurrent - _decre) : 0;
	//�ж��Ƿ�����
	if (healthCurrent <= 0)
		isAlive = false;

	//ÿ���ܵ�������Ҫ�����ܻ���Ӱ���׶������������׼�ʱ����
	isShowSketch = true;
	sketchTimer.Restart();
}

void Enemy::SlowDown()
{
	//ע�ⲻ��speedCurrent -= SLOW_DOWN_SPEED_LOST�����߻ᵼ���ٶ���ʧֱ��Ϊ�㣬���ǲ��Ե�
	speedCurrent = speedMaximum - SLOW_DOWN_SPEED_LOST;
	//����һ��ʱ����ָ���ʼ�������
	speedRestoreTimer.SetWaitTime(SLOW_DOWN_DURATION);
	speedRestoreTimer.Restart();
}

void Enemy::Kill()
{
	//�����˴������ҵ�ʱ�򣬼�ʹѪ��δ���㣬ҲҪ��������Ч��������
	isAlive = false;
}

bool Enemy::IsAlive() const
{
	return IsAlive;
}

double Enemy::GetRouteProcess() const
{
	//�洢·������
	int _routeSize = route->GetTilePointList().size();

	//����·����ɽ���
	return (double)((currentTileIdx + 1) / _routeSize);
}

double Enemy::GetHealth() const
{
	return healthCurrent;
}

const Vector2& Enemy::GetSpriteSize() const
{
	//������ײ����ʱ�򣬾���Ҫ��ȡ��ײ��ߴ磨�˴�����Ϊ��ͼ�ߴ磩
	return spriteSize;
}

const Vector2& Enemy::GetPosition() const
{
	return position;
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

double Enemy::GetSkillRecoverRange() const
{
	//�����ļ��й��ڳ��ȵ����ݵĵ�λ����[ÿ��Ԫ��]Ϊ��λ������Ҫ������Ƭ����
	return skillRecoverRange * TILE_SIZE;
}

double Enemy::GetSkillRecoverIntensity() const
{
	return skillRecoverIntensity;
}

void Enemy::RefreshTargetTilePosition()
{
	//��ȡ·����ά��Ƭ�����б�
	const Route::TilePointList& _tilePointList = route->GetTilePointList();
	
	//Ŀ����Ƭ���������ǵ�ǰ������Ƭ��������һ��
	int _targetTileIdx = currentTileIdx + 1;

	//�����ǰû�г���·������Ƭ�������������Ѱ����һ��Ŀ��������Ļ�ϵ�λ��
	if (_targetTileIdx < _tilePointList.size())
	{
		//��ȡĿ����Ƭ�Ķ�άSDL_Point���꣬����Vector2��ͬ��ǰ������ɢ�ģ�����TILE_SIZE���ܱ�ʾʵ�ʾ��룩�����꣬�����������ľ����������
		const SDL_Point& _targetTilePoint = _tilePointList[_targetTileIdx];

		//��ȡ��̬��������Ƭ��ͼ��Ⱦ����Ϸ�����е�λ��Rect�����ڶ�λ·���ϵ�Ŀ��������Ϸ���ڵ�λ��
		static const SDL_Rect& _mapRect = ConfigManager::GetInstance()->mapRect;

		//�ӵ�ͼ���Ͻǿ�ʼ��������ά��Ƭ�����ɢ����Ѱ�ҵ���·���ϵģ�Ŀ����Ƭ���϶���ľ������꣬�ټ��ϰ��TILE_SIZE�õ�Ŀ����Ƭ��������Ϸ�����ϵ�����
		position.x = _mapRect.x + (_targetTilePoint.x * TILE_SIZE) + TILE_SIZE / 2;
		position.y = _mapRect.y + (_targetTilePoint.y * TILE_SIZE) + TILE_SIZE / 2;
	}
}

#endif
