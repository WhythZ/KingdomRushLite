#include "../../../Header/Manager/Concrete/DropManager.h"
#include "../../../Header/Drop/Concrete/Coin.h"
#include "../../../Header/Manager/Concrete/PlayerManager.h"

DropManager::~DropManager()
{
	for (Drop* _drop : dropList)
		delete _drop;
}

void DropManager::OnUpdate(double _delta)
{
	//��������ҵ���ײ���
	UpdateCollisionPlayer();

	//�����Ч������
	for (Drop* _drop : dropList)
		_drop->OnUpdate(_delta);

	RemoveInvalidDrops();
}

void DropManager::OnRender(SDL_Renderer* _renderer)
{
	for (Drop* _drop : dropList)
		_drop->OnRender(_renderer);
}

void DropManager::SpawnDrop(DropType _type, const Vector2& _position)
{
	Drop* _drop = nullptr;
	switch (_type)
	{
	case DropType::Coin:
		_drop = new Coin();
		break;
	default:
		break;
	}
	if (_drop == nullptr)
		return;

	_drop->SetPosition(_position);

	//�������ɵĵ���������б�
	dropList.push_back(_drop);
}

void DropManager::TrySpawnDrop(DropType _type, const Vector2& _position, double _possibility)
{
	if ((double)(rand() % 100) / 100.0 < _possibility)
		SpawnDrop(_type, _position);
}

const DropManager::DropList& DropManager::GetDropList() const
{
	return dropList;
}

void DropManager::UpdateCollisionPlayer()
{
	static Player* _player = PlayerManager::Instance()->player;
	Vector2 _pSize = _player->GetSize();
	Vector2 _pPosition = _player->GetPosition();

	for (Drop* _drop : dropList)
	{
		if (_pPosition.x + _pSize.x / 2 >= _drop->GetPosition().x - _drop->GetSize().x / 2
			&& _pPosition.x - _pSize.x / 2 <= _drop->GetPosition().x + _drop->GetSize().x / 2
			&& _pPosition.y + _pSize.y / 2 >= _drop->GetPosition().y - _drop->GetSize().y / 2
			&& _pPosition.y - _pSize.y / 2 <= _drop->GetPosition().y + _drop->GetSize().y / 2)
			_drop->OnCollide();
	}
}

void DropManager::RemoveInvalidDrops()
{
	//����remove_if�����б�����Lambda�ķ��ص�bool����true��Ԫ��ͳͳ�����б�������ĩβ����������һ��ָ���һ��true��Ԫ�صĵ�����
	auto _begin = std::remove_if(dropList.begin(), dropList.end(),
		[](const Drop* _drop)
		{
			if (!_drop->IsValid())
			{
				delete _drop;
				return true;
			}
			return false;
		});

	//ɾ��������Ч�ĵ������ʱ���б���remove_if�������£�������Ч�ĵ�����ָ������б�ĩβ
	dropList.erase(_begin, dropList.end());
}