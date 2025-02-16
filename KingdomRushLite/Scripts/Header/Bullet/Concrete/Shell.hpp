#ifndef _SHELL_HPP_
#define _SHELL_HPP_

#include "../Bullet.hpp"

class Shell : public Bullet
{
public:
	Shell();
	~Shell() = default;

	void OnCollide(Enemy*) override;
};

Shell::Shell()
{
}

void Shell::OnCollide(Enemy*)
{
}

#endif
