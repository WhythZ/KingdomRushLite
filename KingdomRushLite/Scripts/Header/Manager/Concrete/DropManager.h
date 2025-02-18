#ifndef _DROP_MANAGER_H_
#define _DROP_MANAGER_H_

#include "../Manager.hpp"

class DropManager :public Manager<DropManager>
{
	friend class Manager<DropManager>;
};

#endif
