#ifndef _ARROW_HPP_
#define _ARROW_HPP_

#include "../Bullet.hpp"

class Arrow : public Bullet
{
public:
	Arrow();
	~Arrow() = default;

	void OnCollide(Enemy*) override;
};

#endif
