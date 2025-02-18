#ifndef _SHELL_HPP_
#define _SHELL_HPP_

#include "../Bullet.hpp"
#include "../../Animation/Animation.hpp"

class Shell : public Bullet
{
private:
	Animation animExploding;                  //炮弹爆炸后的动画

public:
	Shell();
	~Shell() = default;

	void OnUpdate(double) override;           //重写补充爆炸后的更新逻辑
	void OnRender(SDL_Renderer*) override;    //重写补充爆炸后的渲染
	void OnCollide(Enemy*) override;
};

#endif
