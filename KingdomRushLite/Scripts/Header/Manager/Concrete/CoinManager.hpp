#ifndef _COIN_MANAGER_HPP_
#define _COIN_MANAGER_HPP_

#include "../Manager.hpp"

class CoinManager :public Manager<CoinManager>
{
	friend Manager<CoinManager>;

private:
	double coinNum = 0;                //玩家持有的金币数量

public:
	void IncreaseCoinNumBy(double);    //增加金币数
	void DecreaseCoinNumBy(double);    //减少金币数

private:
	CoinManager() = default;
	~CoinManager() = default;
};

void CoinManager::IncreaseCoinNumBy(double _reward)
{
	coinNum += _reward;
}

void CoinManager::DecreaseCoinNumBy(double _cost)
{
	coinNum = (coinNum - _cost > 0) ? (coinNum - _cost) : 0;
}

#endif
