#pragma once
#include "BaseClasses.h"
class Player :
	public Controllable
{
private:
	int money;
public:
	Player();
	~Player();

	int GetMoney();
	void SetMoney(int difference);

};

