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
	//释放技能阶段不应当接收任何输入
}

void PlayerSkillState::OnUpdate(double _delta)
{
	PlayerState::OnUpdate(_delta);

	//释放技能时不动
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