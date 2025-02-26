#ifndef _SHELL_H_
#define _SHELL_H_

#include "../Bullet.h"
#include "../../Infra/Animation.h"

class Shell : public Bullet
{
private:
	Vector2 explodeSize = { 96,96 };          //�ڵ���ը��Ķ����ĳߴ磬Ҳ���ڼ��㱬ը�˺��뾶

	Animation animExploding;                  //�ڵ���ը��Ķ���

public:
	Shell();
	~Shell() = default;

	void OnUpdate(double) override;           //��д���䱬ը��ĸ����߼�
	void OnRender(SDL_Renderer*) override;    //��д���䱬ը�����Ⱦ
	void OnCollide(Enemy*) override;
};

#endif
