#include "../../Header/Drop/Drop.h"
#include "../../Header/Map/Tile.h"

Drop::Drop()
{
	#pragma region Timer
	popTimer.SetOneShot(true);
	popTimer.SetWaitTime(popDuration);
	popTimer.SetTimeOutTrigger(
		[&]()
		{
			isPoping = false;
		}
	);

	disappearTimer.SetOneShot(true);
	disappearTimer.SetWaitTime(existDuration);
	disappearTimer.SetTimeOutTrigger(
		[&]()
		{
			Invalidate();
		}
	);
	#pragma endregion

	#pragma region Velocity
	//������������ʱ�������rand()%2�������0��1������ֵ������ˮƽ��������ҵ�ˮƽ�ٶȣ����ݱ���Ŀ�ٶȶ��������Ƭ�ߴ�
	velocity.x = (rand() % 2 ? 1 : -1) * initSpeedX * TILE_SIZE;
	//�ٸ����ʼ�����ϣ������и��ţ��������ٶ�
	velocity.y = -initSpeedY * TILE_SIZE;
	#pragma endregion
}

void Drop::SetPosition(const Vector2& _position)
{
	position = _position;
}

void Drop::OnUpdate(double _delta)
{
	popTimer.OnUpdate(_delta);
	disappearTimer.OnUpdate(_delta);

	animIdle.OnUpdate(_delta);

	//����õ������Ǹյ�������������ϵ�����״̬����������£�ע���ǼӺţ�ʩ���������ٶȣ�ģ�����������˶�
	if (isPoping)
		velocity.y += gravity * _delta;
	//�������崦�����״̬
	else
	{
		//��ֹˮƽ�����˶�
		velocity.x = 0;

		//�����Ǻ���ģ�⴦�ڵ����ϵ����²���״̬����ʾ��������Ա�����
		//SDL_GetTicks64()��¼��SDL����������ʱ�䣬����任�������Ա��������ȡֵ[-1,1]�����Ǻ���ֵ�������
		velocity.y = sin(SDL_GetTicks64() / 1000.0 * floatingFrequency) * floatingAmplitude;
	}

	//�����ٶ��������µ������λ��
	position += velocity * _delta;
}

void Drop::OnRender(SDL_Renderer* _renderer)
{
	static SDL_Point _point;
	_point.x = (int)(position.x - size.x / 2);
	_point.y = (int)(position.y - size.y / 2);

	animIdle.OnRender(_renderer, _point);
}

void Drop::OnCollide()
{
	//�������
	Invalidate();
}

void Drop::Invalidate()
{
	isValid = false;
}

bool Drop::IsValid() const
{
	return isValid;
}

const Vector2& Drop::GetPosition() const
{
	return position;
}

const Vector2& Drop::GetSize() const
{
	return size;
}