#ifndef _SHELL_HPP_
#define _SHELL_HPP_

#include "../Bullet.hpp"
#include "../../Animation/Animation.hpp"

class Shell : public Bullet
{
private:
	Animation animExploding;                  //�ڵ���ը��Ķ���

public:
	Shell();
	~Shell() = default;

	void OnUpdate(double) override;           //��д���䱬ը��ĸ����߼�
	void OnRender(SDL_Renderer*) override;    //��д���䱬ը�����Ⱦ
	void OnCollide(Enemy*) override;
};

#endif
