#include "../../Header/Enemy/Enemy.h"
#include "../../Header/Manager/Concrete/ProcessManager.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/DropManager.h"

Enemy::Enemy()
{
	//�����ٶ���ʧʱ��ͨ���ü�ʱ���������ڼ�ʱ����ʱ�ָ���ʼ���٣��ǵ��δ���
	speedRestoreTimer.SetOneShot(true);
	speedRestoreTimer.SetTimeOutTrigger([&]() {speedCurrent = speedMaximum; });

	#pragma region Animation
	//��ʼ������
	animCurrent = &animDown;

	//�ܻ���������һ�μ���
	sketchTimer.SetOneShot(true);
	//�����ܻ����׵ĳ���ʱ�䣬�̶̵ľͺ�
	sketchTimer.SetWaitTime(0.1);
	//���ûص�����
	sketchTimer.SetTimeOutTrigger([&]() {isShowSketch = false; });
	#pragma endregion

	#pragma region Skill
	//�ָ�������ȴһ�����ͻ��ٴ��ͷţ����Բ��ǵ��δ���
	skillRecoverCooldownTimer.SetOneShot(false);
	//��ȴ�������ͷŻָ����ܣ����ܵ��ͷ���Ϊ�Լ������������˰�װ���Ӧ�ص���������������
	skillRecoverCooldownTimer.SetTimeOutTrigger(
		[&]()
		{
			skillRecoverTrigger(this);
		}
	);
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
	//����Ŀ���
	RefreshTargetTile();
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
	skillRecoverCooldownTimer.OnUpdate(_delta);
	speedRestoreTimer.OnUpdate(_delta);

	#pragma region RefreshAfterMoving
	//����ǰһ֡�������µ�_deltaʱ���ڣ������й��������ƶ��ľ��룬�Լ�������һ��Ŀ��ש��ľ���
	Vector2 _moveDistanceVec = velocity * _delta;
	Vector2 _targetDistanceVec = targetTilePosition - position;
	//��ǰ֡ʵ���ƶ��ľ��벻Ӧ�����ڵ���ǰĿ���ľ���
	position += (_moveDistanceVec < _targetDistanceVec) ? _moveDistanceVec : _targetDistanceVec;

	//�����ǰλ�þ���Ŀ��ص�ľ��볤�ȣ��ڱ궨�߶��£�������0����˵�������˸�Ŀ��ص㣬��ʱ��Ҫ���µ�ǰλ�ã�����ȡ�µ�Ŀ��λ��
	if (_targetDistanceVec.ApproxZero())
	{
		//std::cout << "Refresh\n";

		//����Ŀ��λ��
		targetTileIdx++;
		RefreshTargetTile();
		//�����н�����
		direction = (targetTilePosition - position).Normalized();
	}

	//�����ٶ�ʸ������Ϊspeed�ĵ�λ�ǵ�Ԫ��ÿ�룬���������ٶ�Ҫ���ϵ�Ԫ��ı߳�
	velocity.x = direction.x * speedCurrent * TILE_SIZE;
	velocity.y = direction.y * speedCurrent * TILE_SIZE;
	#pragma endregion

	#pragma region AnimationChange
	//����velocity.x��velocity.y���ڸ�������Ե�ʣ������ñ�׼�ĵ��������оݣ������ñȽ�ˮƽ��ֱ�ٶȴ�С�ķ�ʽ��ȷ������ˮƽ����������ֱ����
	bool _isShowAnimHorizontal = abs(velocity.x) >= abs(velocity.y);

	//������ˮƽ����������ˮƽ�ٶ������ж����ң���ֱ�������ƣ��������Ƿ����ܻ�״̬�жϲ����ĸ��汾
	if (_isShowAnimHorizontal)
	{
		if (!isShowSketch)
			animCurrent = (velocity.x > 0) ? &animRight : &animLeft;
		else
			animCurrent = (velocity.x > 0) ? &animRightSketch : &animLeftSketch;
	}
	else
	{
		if (!isShowSketch)
			animCurrent = (velocity.y > 0) ? &animDown : &animUp;
		else
			animCurrent = (velocity.y > 0) ? &animDownSketch : &animUpSketch;
	}
	#pragma endregion
}

void Enemy::OnRender(SDL_Renderer* _renderer)
{
	#pragma region SpriteAnimation
	//������ͼ�����ϽǶ������꣬position����Ƭ���ĵ���������꣬ע���ȥ�Ĳ���TILE_SIZE / 2
	static SDL_Point _point;
	_point.x = (int)(position.x - size.x / 2);
	_point.y = (int)(position.y - size.y / 2);

	//���õ�ǰ��������Ⱦ
	animCurrent->OnRender(_renderer, _point);
	#pragma endregion

	#pragma region Debug
	collideOutline.DrawBoxOutline(_renderer, { 255,0,0,255 },
		{ (int)position.x,(int)position.y }, size, 0);
	#pragma endregion

	#pragma region HealthBar
	//Ѫ���Ķ�λRect
	static SDL_Rect _healthBarRect;
	//Ѫ���ĳ����ߴ��С
	static const Vector2 _healthBarSize = { 40,8 };
	//Ѫ��ƫ����ͼ���Ϸ���ƫ������Ϊ����Ѫ����Ҫ�����Ź�����ͼ�Ķ��ߣ�
	static const float _healthBarVerticalOffset = 3;
	//Ѫ����������߿����ɫ��R-G-B-Alpha
	static const SDL_Color _colorContent = { 225,255,195,255 };
	static const SDL_Color _colorBorder = { 115,185,125,255 };

	//����������ֵ����ʱ������Ѫ��
	if (healthCurrent < healthMaximum)
	{
		//��Ѫ�������ĵ�ˮƽ�϶��������ͼ������
		_healthBarRect.x = (int)(position.x - _healthBarSize.x / 2);
		//��ֱ���ڹ�����ͼ�����Ϸ���ע���Ǽ�����������y���������µ�Ե�ʣ�
		_healthBarRect.y = (int)(position.y - size.y / 2 - _healthBarVerticalOffset - _healthBarSize.y);
		//��ȱ�ʾ�������ĳ���
		_healthBarRect.w = (int)(_healthBarSize.x * (healthCurrent / healthMaximum));
		//�߶Ȳ���
		_healthBarRect.h = (int)(_healthBarSize.y);

		//����Ѫ�����ݻ�����ɫ
		SDL_SetRenderDrawColor(_renderer, _colorContent.r, _colorContent.g, _colorContent.b, _colorContent.a);
		//��������ɫ��������Ѫ������
		SDL_RenderFillRect(_renderer, &_healthBarRect);

		//Ϊ�˻���Ѫ���߿򣬽���ȱ��ԭ���Ŀ��
		_healthBarRect.w = (int)(_healthBarSize.x);
		SDL_SetRenderDrawColor(_renderer, _colorBorder.r, _colorBorder.g, _colorBorder.b, _colorBorder.a);
		//��������ɫ����Ѫ���߿�ע����Draw������Fill��
		SDL_RenderDrawRect(_renderer, &_healthBarRect);
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
	{
		//��������״̬
		isAlive = false;
		
		//�и��ʱ����
		DropManager::Instance()->TrySpawnDrop(DropType::Coin, position, coinRatio);
	}

	//ÿ���ܵ�������Ҫ�����ܻ���Ӱ���׶������������׼�ʱ����
	isShowSketch = true;
	sketchTimer.Restart();
}

void Enemy::SlowDownBy(double _speedLossPercentage, double _lastDuration)
{
	//�޶�����ֵ�ںϷ���Χ�ں���һ���İٷֱȼ��������ٶ�
	double _percentage = (_speedLossPercentage < 0) ? 0 : _speedLossPercentage;
	_percentage = (_percentage >= 1) ? 0.99 : _percentage;
	speedCurrent = speedMaximum * (1 - _percentage);

	//����һ��ʱ����ָ���ʼ�������
	speedRestoreTimer.SetWaitTime(_lastDuration);
	speedRestoreTimer.Restart();
}

void Enemy::Kill()
{
	//�����˴������ҵ�ʱ�򣬼�ʹѪ��δ���㣬ҲҪ��������Ч��������
	isAlive = false;
}

bool Enemy::IsAlive() const
{
	return isAlive;
}

double Enemy::GetRouteProcess() const
{
	//����·����ɽ���
	return (double)targetTileIdx / (double)route->GetTilePointList().size();
}

double Enemy::GetHealth() const
{
	return healthCurrent;
}

const SDL_Point& Enemy::GetSize() const
{
	//������ײ����ʱ�򣬾���Ҫ��ȡ��ײ��ߴ磨�˴�����Ϊ��ͼ�ߴ磩
	return size;
}

const Vector2& Enemy::GetPosition() const
{
	return position;
}

double Enemy::GetAttackDamage() const
{
	return attackDamage;
}

double Enemy::GetSkillRecoverCooldown() const
{
	return skillRecoverCooldown;
}

double Enemy::GetSkillRecoverRadius() const
{
	//�����ļ��й��ڳ��ȵ����ݵĵ�λ����[ÿ��Ԫ��]Ϊ��λ������Ҫ������Ƭ����
	return skillRecoverRadius;
}

double Enemy::GetSkillRecoverIntensity() const
{
	return skillRecoverIntensity;
}

void Enemy::RefreshTargetTile()
{
	//��ȡ·����ά��Ƭ�����б�
	const Route::TilePointList& _tilePointList = route->GetTilePointList();
	//std::cout << "_tilePointList.size()=" << _tilePointList.size() << "\n";

	//�����ǰ��Ŀ���û�г���·������Ƭ�������������Ѱ����һ��Ŀ��������Ļ�ϵ�λ��
	if (targetTileIdx < _tilePointList.size())
	{
		//��ȡ��ǰ��Ŀ����Ƭ�Ķ�άSDL_Point���꣬����Vector2��ͬ��ǰ������ɢ�ģ�����TILE_SIZE���ܱ�ʾʵ�ʾ��룩�����꣬�����������ľ����������
		const SDL_Point& _targetTilePoint = _tilePointList[targetTileIdx];

		//std::cout << "TargetTilePoint=" << "(" << _targetTilePoint.x << "," << _targetTilePoint.y << ")\n";

		//��ȡ��̬��������Ƭ��ͼ��Ⱦ����Ϸ�����е�λ��Rect�����ڶ�λ·���ϵ�Ŀ��������Ϸ���ڵ�λ��
		static const SDL_Rect& _mapRect = ProcessManager::Instance()->mapRect;
		//�ӵ�ͼ���Ͻǿ�ʼ��������ά��Ƭ�����ɢ����Ѱ�ҵ���·���ϵģ�Ŀ����Ƭ���϶���ľ������꣬�ټ��ϰ��TILE_SIZE�õ�Ŀ����Ƭ��������Ϸ�����ϵ�����
		targetTilePosition.x = _mapRect.x + (_targetTilePoint.x * TILE_SIZE) + TILE_SIZE / 2;
		targetTilePosition.y = _mapRect.y + (_targetTilePoint.y * TILE_SIZE) + TILE_SIZE / 2;

		//std::cout << "TargetTilePosition=" << targetTilePosition << ")\n";
	}
}