#ifndef _AXE_H_
#define _AXE_H_

#include "../Bullet.h"

class Axe : public Bullet
{
public:
	Axe();
	~Axe() = default;

	void OnCollide(Enemy*) override;
};

#endif
