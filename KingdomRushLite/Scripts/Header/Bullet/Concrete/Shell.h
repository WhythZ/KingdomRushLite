#ifndef _SHELL_H_
#define _SHELL_H_

#include "../Bullet.h"
#include "../../Infra/Animation.h"

class Shell : public Bullet
{
private:
	Vector2 explodeSize = { 96,96 };          //炮弹爆炸后的动画的尺寸，也用于计算爆炸伤害半径

	Animation animExploding;                  //炮弹爆炸后的动画

public:
	Shell();
	~Shell() = default;

	void OnUpdate(double) override;           //重写补充爆炸后的更新逻辑
	void OnRender(SDL_Renderer*) override;    //重写补充爆炸后的渲染
	void OnCollide(Enemy*) override;
};

#endif
