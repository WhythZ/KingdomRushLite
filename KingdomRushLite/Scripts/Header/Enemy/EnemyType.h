#ifndef _ENEMY_TYPE_H_
#define _ENEMY_TYPE_H_

#include <iostream>

enum class EnemyType
{
	None,
	Slime,
	SlimeKing,
	Skeleton,
	Goblin,
	GoblinPriest
};

std::ostream& operator<<(std::ostream&, const EnemyType&);

#endif