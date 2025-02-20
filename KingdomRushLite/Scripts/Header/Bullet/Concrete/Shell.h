#ifndef _SHELL_H_
#define _SHELL_H_

#include "../Bullet.h"
#include "../../Infra/Animation.h"

#define BULLET_SHELL_DAMAGE_RADIUS 72

class Shell : public Bullet
{
private:
	Vector2 explodingSize;                    //炮弹爆炸后的动画的尺寸
	Animation animExploding;                  //炮弹爆炸后的动画

public:
	Shell();
	~Shell() = default;

	void OnUpdate(double) override;           //重写补充爆炸后的更新逻辑
	void OnRender(SDL_Renderer*) override;    //重写补充爆炸后的渲染
	void OnCollide(Enemy*) override;
};

#endif
