#include "../../../../Header/Player/State/Concrete/PlayerSkillState.h"
#include "../../../../Header/Manager/Concrete/PlayerManager.h"

PlayerSkillState::PlayerSkillState(bool* _flag) :PlayerState::PlayerState(_flag)
{
}

void PlayerSkillState::OnBegin()
{
	PlayerState::OnBegin();
	std::cout << "PlayerSkillState::OnBegin()\n";
}

void PlayerSkillState::OnInput(const SDL_Event& _event)
{
	//�ͷż��ܽ׶β�Ӧ�������κ�����
}

void PlayerSkillState::OnUpdate(double _delta)
{
	PlayerState::OnUpdate(_delta);

	//�ͷż���ʱ����
	PlayerManager::Instance()->player->SetVelocity({ 0,0 });
}

void PlayerSkillState::OnRender(SDL_Renderer* _renderer)
{
	PlayerState::OnRender(_renderer);
}

void PlayerSkillState::OnEnd()
{
	PlayerState::OnEnd();
	//std::cout << "PlayerSkillState::OnEnd()\n";
}