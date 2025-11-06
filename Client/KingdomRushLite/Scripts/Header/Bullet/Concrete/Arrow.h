#ifndef _ARROW_H_
#define _ARROW_H_

#include "../Bullet.h"

class Arrow : public Bullet
{
public:
	Arrow();
	~Arrow() = default;

	void OnCollide(Enemy*) override;
};

#endif
