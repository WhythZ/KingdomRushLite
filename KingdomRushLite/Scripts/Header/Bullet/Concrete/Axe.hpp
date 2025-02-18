#ifndef _AXE_HPP_
#define _AXE_HPP_

#include "../Bullet.hpp"

class Axe : public Bullet
{
public:
	Axe();
	~Axe() = default;

	void OnCollide(Enemy*) override;
};

#endif
