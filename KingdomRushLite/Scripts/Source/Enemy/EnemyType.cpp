#include "../../Header/Enemy/EnemyType.h"

std::ostream& operator<<(std::ostream& _cout, const EnemyType& _type)
{
	switch (_type)
	{
	case EnemyType::Slime:
		_cout << "Slime";
		break;
	case EnemyType::SlimeKing:
		_cout << "SlimeKing";
		break;
	case EnemyType::Skeleton:
		_cout << "Skeleton";
		break;
	case EnemyType::Goblin:
		_cout << "Goblin";
		break;
	case EnemyType::GoblinPriest:
		_cout << "GoblinPriest";
		break;
	}
	return _cout;
}