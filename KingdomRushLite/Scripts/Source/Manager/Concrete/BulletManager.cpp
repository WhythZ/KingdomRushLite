#include "../../../Header/Manager/Concrete/BulletManager.hpp"
#include "../../../Header/Bullet/Concrete/Arrow.hpp"
#include "../../../Header/Bullet/Concrete/Axe.hpp"
#include "../../../Header/Bullet/Concrete/Shell.hpp"

BulletManager::~BulletManager()
{
	for (Bullet* _bullet : bulletList)
		delete _bullet;
}

void BulletManager::OnUpdate(double _delta)
{
	for (Bullet* _bullet : bulletList)
		_bullet->OnUpdate(_delta);

	//�����Ч���ӵ����ӳ������Ƴ�
	RemoveInvalidBullets();
}

void BulletManager::OnRender(SDL_Renderer* _renderer)
{
	for (Bullet* _bullet : bulletList)
		_bullet->OnRender(_renderer);
}

void BulletManager::FireBullet(BulletType _type, const Vector2& _position, const Vector2& _velocity, double _damager)
{
	Bullet* _bullet = nullptr;
	switch (_type)
	{
	case BulletType::Arrow:
		_bullet = new Arrow();
		break;
	case BulletType::Axe:
		_bullet = new Axe();
		break;
	case BulletType::Shell:
		_bullet = new Shell();
		break;
	default:
		break;
	}
	if (_bullet == nullptr)
		return;

	//�����ӵ���ʼ����λ�á��ٶȡ��˺�
	_bullet->SetPosition(_position);
	_bullet->SetVelocity(_velocity);
	_bullet->SetDamage(_damager);

	//�������ɵ��ӵ������б�
	bulletList.push_back(_bullet);
}

const BulletManager::BulletList& BulletManager::GetBulletList() const
{
	return bulletList;
}

void BulletManager::RemoveInvalidBullets()
{
	//����remove_if�����б�����Lambda�ķ��ص�bool����true��Ԫ��ͳͳ�����б�������ĩβ����������һ��ָ���һ��true��Ԫ�صĵ�����
	auto _begin = std::remove_if(bulletList.begin(), bulletList.end(),
		[](const Bullet* _bullet)
		{
			if (!_bullet->IsValid())
			{
				delete _bullet;
				return true;
			}
			return false;
		});

	//ɾ��������Ч���ӵ�����ʱ���б���remove_if�������£�������Ч���ӵ�ָ������б�ĩβ
	bulletList.erase(_begin, bulletList.end());
}
