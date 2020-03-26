#pragma once
#include "SceneNode.h"

class Damageable : public SceneNode
{
public:
	Damageable();

	virtual bool applyDamage(int amount) = 0;
};