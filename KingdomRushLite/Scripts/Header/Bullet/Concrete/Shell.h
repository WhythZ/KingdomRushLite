#ifndef _SHELL_H_
#define _SHELL_H_

#include "../Bullet.h"
#include "../../Animation/Animation.h"

class Shell : public Bullet
{
private:
	Vector2 explodingSize;                    //�ڵ���ը��Ķ����ĳߴ�
	Animation animExploding;                  //�ڵ���ը��Ķ���

public:
	Shell();
	~Shell() = default;

	void OnUpdate(double) override;           //��д���䱬ը��ĸ����߼�
	void OnRender(SDL_Renderer*) override;    //��д���䱬ը�����Ⱦ
	void OnCollide(Enemy*) override;
};

#endif
